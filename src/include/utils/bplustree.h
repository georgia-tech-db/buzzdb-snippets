#include <iostream>
#include <math.h>
#include <shared_mutex>
#include <vector>

namespace buzzdb {

class Node {
    private:
        /* Private Members */
        int *keys;                /* Keys which are stored in this node in sorted order */
        int degree;               /* Maximum number of children of this node */
        Node **children;          /* Children of this node */
        int count;                /* Current count of keys */
        bool leaf;                /* Boolean indicating if the node is a leaf node */
        std::shared_mutex mutex;  /* Lock of this node */

        /* Private Methods */
        void acquireLock() {
            std::cout << "Acquiring Lock on " << this << std::endl;
            this->mutex.lock();
        }
        void releaseLock() {
            std::cout << "Releasing Lock on " << this << std::endl;
            this->mutex.unlock();
        }
        friend class BTree;

    public:
        /* Public Methods */
        ~Node();
        Node(int, bool);
        void display(std::vector<int>&);
        void insertNonFull(int, Node*);
        void splitChild(int, Node*);
        void cleanupChildren();
};

class BTree {
private:
    /* Private Members */
    Node *root;                 /* Holds the root node of the B+ Tree */

    int degree;                 /* Maximum number of children any node in this
                                   B+ Tree can hold */

    std::shared_mutex mutex;    /* Lock of the entire B+ Tree. To be used when
                                   the state of the B+ Tree itself changes
                                   (say the root) */

    /* Private Methods */
    void acquireLock() {this->mutex.lock();}
    void releaseLock() {this->mutex.unlock();}

public:
    /* Public Methods */
    BTree(int degree) {
        this->root = NULL;
        this->degree = ceil(degree / 2);
    }

    ~BTree();

    void display(std::vector<int>& inOrderKeys) {
        if (this->root != NULL) {
            this->root->display(inOrderKeys);
        }
    }

    void insert(int);
};

} // namespace buzzdb
