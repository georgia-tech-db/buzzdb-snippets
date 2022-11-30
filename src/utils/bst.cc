#include "utils/bst.h"

#include <cstdio>

namespace buzzdb {
namespace BSTree {

#define UNUSED(p) ((void)(p))

BSTree::BSTree() {
  this->root = NULL;
}

BSTree::~BSTree() {
  // Inefficent delete, repeatedly delete the root till there are no values
  while (this->root != NULL) {
    Delete(root->value);
  }
}

bool BSTree::IsEmpty() {
  return this->root == NULL;
}

bool BSTree::Find(int value) {
  TreeNode *node = this->root;

  while (node != NULL) {
    if (node->value == value) {
      return true;
    } else if (node->value < value) {
      node = node->left;
    } else {
      node = node->right;
    }
  }

  // Node was not found
  return false;
}

bool BSTree::Insert(int value) {
  return RecursiveInsert(value, &this->root);
}

bool BSTree::RecursiveInsert(int value, TreeNode** node) {
  if (*node == NULL) {
    // Base case. Replace the value of node with the new node
    *node = new TreeNode;
    (*node)->value = value;
    (*node)->right = NULL;
    (*node)->left = NULL;
    return true;
  } else {
    // Continue searching for the correct place to insert the node
    if ((*node)->value == value) {
      return false;
    } else if ((*node)->value < value) {
      return RecursiveInsert(value, &(*node)->left);
    } else {
      return RecursiveInsert(value, &(*node)->right);
    }
  }
}

bool BSTree::Delete(int value) {
  return RecursiveDelete(value, &this->root);
}

bool BSTree::RecursiveDelete(int value, TreeNode** node) {
  if (node == NULL || *node == NULL) {
    // Node wasn't found
    return false;
  } else {
    // Continue searching for the correct place to insert the node
    if ((*node)->value == value) {
      // Delete the value
      TreeNode *left = (*node)->left;
      TreeNode *right = (*node)->right;

      if (left == NULL && right == NULL) {
        // Case 1. Neither side has values
        delete (*node);
        *node = NULL;
      } else if (left == NULL) {
        // Case 2. One child is null
        delete (*node);
        *node = right;
      } else if (right == NULL) {
        // Case 3. One child is null
        delete (*node);
        *node = left;
      } else {
        // Case 4. Both children are present
        // Find replacement value
        TreeNode *tNode = left;
        while (tNode->right != NULL) {
          tNode = tNode->right;
        }
        int replacement_value = tNode->value;

        // Delete original replacement node
        RecursiveDelete(replacement_value, &left);

        // Set node with replacement
        (*node)->value = replacement_value;
      }

      return true;
    } else if ((*node)->value < value) {
      return RecursiveDelete(value, &(*node)->left);
    } else {
      return RecursiveDelete(value, &(*node)->right);
    }
  }
}

}  // namespace utils
}  // namespace buzzdb
