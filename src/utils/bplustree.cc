#include "utils/bplustree.h"

namespace buzzdb {

/*****************************************************
 *              Methods of Node Class                *
 ****************************************************/

/* Constructor
 *
 * Initializes the state of node
 *
 * @param degree (int)   number of children this node can hold
 * @param leaf   (bool)  indicates if the node is a leaf
 *
 * @return void
 */
Node::Node(int degree, bool leaf) {
    this->degree = degree;
    this->leaf = leaf;

    this->keys = new int[2 * degree - 1];
    this->children = new Node *[2 * degree];

    for (int i=0; i<(2 * degree); i++)
        children[i] = NULL;

    for (int i=0; i<(2 * degree) - 1; i++)
        keys[i] = 0;

    this->count = 0;
}

/* Destructor: Cleans up the state of node
 *
 * @param void
 *
 * @return void
 */
Node::~Node() {
    delete[] keys;
    for (int i=0; i<this->count; i++) {
        delete children[i];
        children[i] = NULL;
    }
    delete[] children;
    this->count = 0;
}

/* Cleans up the children nodes
 * NOTE: Not Thread-safe
 *
 * @param void
 *
 * @return void
 */
void Node::cleanupChildren() {
    for (int i = 0; i <= this->count; i++) {
        if(this->children[i]->leaf) {
            // If child is a leaf -> just delete it
            delete children[i];
        } else {
            // Else recurse and then delete
            this->children[i]->cleanupChildren();
            delete this->children[i];
        }

        this->children[i] = NULL;
    }
}

/* Populates the parameter vector with sorted list from the B+ Tree
 * NOTE: Not Thread-safe
 *
 * @param inOrderKeys  (vector<int>)   List of sorted elements from B+ Tree
 *
 * @return void
 */
void Node::display(std::vector<int>& inOrderKeys) {
    int i;
    for (i = 0; i < count; i++) {
        if (this->leaf == false) {
            this->children[i]->display(inOrderKeys);
        }
        inOrderKeys.push_back(keys[i]);
    }

    if (this->leaf == false) {
        this->children[i]->display(inOrderKeys);
    }
}

/* Insert into the current node / children of the current node
 * NOTE: Thread-safe when called with appropriate protection from BTree
 * NOTE: Assumes that the current node in itself isn't full
 * NOTE: The caller need to make sure the first call to the function is made on a non-full node
 *
 * Algorithm:
 * If parentNode was passed -> releases its lock (We don't bubble split upwards)
 * If the currentNode is leaf -> inserts and releases lock on the current node
 * If the currentNode is an inner node:
 *     Finds the children who will hold the key
 *     If insertion of this key can overflow the child -> splits it post acquiring lock on the child
 *     Recursively call the function till a leafnode that is empty is reached
 *
 * @param k           (int)    key to insert
 * @param parentNode  (Node*)  Parent of the current node
 *
 * @return void
 */
void Node::insertNonFull(int k, Node* parentNode) {

    // If parent exists -> unlock it
    if (parentNode != NULL) {
        parentNode->releaseLock();
    }

    int i = count - 1;

    if (this->leaf == true) {
        // Find location to insert
        while (i >= 0 && this->keys[i] > k) {
            this->keys[i + 1] = this->keys[i];
            i--;
        }

        // Insert the key
        this->keys[i + 1] = k;
        this->count++;

        // Leaf node -> unlock this node
        this->releaseLock();
    } else {
        // Find children to go to
        while (i >= 0 && this->keys[i] > k) {
            i--;
        }

        // Check if child could overflow, if yes -> split it
        int splitIdx = i+1;
        this->children[splitIdx]->acquireLock();
        if (this->children[splitIdx]->count == 2 * this->degree - 1) {
            this->splitChild(splitIdx, this->children[splitIdx]);
            if (keys[splitIdx] < k) {
                i++;
            }
        }
        this->children[splitIdx]->releaseLock();

        // Insert into child
        this->children[i + 1]->acquireLock();
        this->children[i + 1]->insertNonFull(k, this);
    }
}

/*
 * Splits the parameter node into 2
 * NOTE: Not thread-safe. Lock needs to be acquired before calling
 *
 * Alogrithm:
 * Moves keys and children from leftNode to a newly created rightNode
 * Sets appropriate counts on leftNode, rightNode and current (parent) node
 *
 * @param i         (int)    Position of split
 * @param leftNode  (Node*)  The node to be split
 *
 * @return void
 */
void Node::splitChild(int i, Node *leftNode) {
    Node *rightNode = new Node(leftNode->degree, leftNode->leaf);
    rightNode->count = degree - 1;

    for (int j = 0; j < degree - 1; j++) {
        rightNode->keys[j] = leftNode->keys[j + degree];
    }

    if (leftNode->leaf == false) {
        for (int j = 0; j < degree; j++) {
            rightNode->children[j] = leftNode->children[j + degree];
        }
    }

    leftNode->count = degree - 1;
    for (int j = this->count; j >= i + 1; j--) {
        this->children[j + 1] = this->children[j];
    }
    this->children[i + 1] = rightNode;

    for (int j = this->count - 1; j >= i; j--) {
        this->keys[j + 1] = this->keys[j];
    }
    this->keys[i] = leftNode->keys[degree - 1];

    count++;
}

/*****************************************************
 *              Methods of BTree Class                *
 ****************************************************/

/*
 * Inserts the parameter key into the B+ Tree
 * NOTE: The implementation is thread-safe with concurrent calls to insert
 *
 * Algorithm:
 * Uses crab locking to lock pages to ensure minimal locking with safe progress
 * Starts by acquiring lock on the B+ tree to make sure nobody is concurrently updating the root node
 * If root node doesn't exists:
 *     Create root node
 *     insert key into root node (which would also be a leaf node)
 *     unlock the b+ tree
 *     return
 * If root node exists:
 *     Acquire lock on root node
 *     Check if an overflow can occur in root node
 *     If not, unlock the lock of B+ Tree (root can't split)
 *     Call insertNonFull on root node
 *     Node::insertNonFull would alterantively lock the immediate parent and child
 * If root node exists, but overflow could happen:
 *     Acquire lock on root node and hold on to B+ Tree's lock (root is going to change)
 *     Create a new node and make root node to be child of the newly created node
 *     Split root node
 *     Insert the key into the older root node or split node appropriately
 *     Unlock locks on root node and B+Tree and return
 *
 * @param k (int) key to insert
 *
 * @return void
 */
void BTree::insert(int k) {

    // Acquire exclusive lock on BTree to check / create / update root node if needed
    this->acquireLock();

    if (this->root == NULL) {
        Node* newRoot = new Node(this->degree, true);
        newRoot->keys[0] = k;
        newRoot->count = 1;
        this->root = newRoot;
        // Root is created -> Unlock BTree's lock
        this->releaseLock();
        return;
    }


    // Acquire lock on root Node -> to insert keys into root Node
    this->root->acquireLock();

    // No splitting is needed
    if (this->root->count != 2 * this->degree - 1) {
        // We are sure root wouldn't change -> Release BTree's lock
        this->releaseLock();

        // insertNonFull would release root's lock post operating
        this->root->insertNonFull(k, NULL);
        return;
    }

    // NOTE: We are still holding lock on root and BTree

    // New root needs to be created
    Node *newRoot = new Node(this->degree, false);

    newRoot->children[0] = this->root;
    newRoot->splitChild(0, this->root);

    int i = 0;
    if (newRoot->keys[0] < k) {
        i++;
    }
    if (newRoot->children[i] != root) {
        newRoot->children[i]->acquireLock();
    }
    newRoot->children[i]->insertNonFull(k, NULL);

    // Update root and release lock on old root and BTree
    Node* oldRoot = root;
    this->root = newRoot;
    if (newRoot->children[i] != oldRoot) {
        oldRoot->releaseLock();
    }
    this->releaseLock();
}

/* Deconstructor: Cleans up the entire tree
 * NOTE: Not Thread-safe
 *
 * @param void
 *
 * @return void
 */
BTree::~BTree() {
    if (this->root != NULL) {
        root->cleanupChildren();
        delete this->root;
        this->root = NULL;
    }
}
} // namespace buzzdb
