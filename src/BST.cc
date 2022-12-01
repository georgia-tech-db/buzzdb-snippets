#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include "src/include/BST.h"
using namespace std;

namespace buzzdb {
namespace utils {


#define UNUSED(p) ((void)(p))

BST::BST()
{
  root = NULL;
}

BST::~BST()
{
  deleteTree(root);
}

node* BST::getRoot()
{
  return root;
}

void BST::insert(int key)
{
    if (root == NULL) {
        root = new node;
        root->data = key;
        root->left = NULL;
        root->right = NULL;
    } else {
        insert(root, key);
    }
}

void BST::insert(node* leaf, int key)
{
  if (key < leaf->data){
    if (leaf->left != NULL) {
      insert(leaf->left, key);
    } else {
      leaf->left = new node;
      leaf->left->data = key;
      leaf->left->left = NULL;
      leaf->left->right = NULL;
    }
  } else {
    if (leaf->right != NULL) {
      insert(leaf->right, key);
    } else {
      leaf->right = new node;
      leaf->right->data = key;
      leaf->right->left = NULL;
      leaf->right->right = NULL;
    }
  }
}

node* BST::search(node* leaf, int key)
{
  if (leaf == NULL) {
    return NULL;
    } else if (key == leaf->data){
        return leaf;
    } else if (key > leaf->data){
        return search(leaf->right, key);
    } else {
        return search(leaf->left, key);
    }
}

node* BST::remove(int key)
{
  return remove(root, key);
}

node* BST::remove(node* tree, int key)
{
  node* temp_node;
  if (tree == NULL)
    return NULL;

  if (key < tree->data) {
    tree->left = remove(tree->left, key);
  } else if (key > tree->data) {
    tree->right = remove(tree->right, key); 
  } 
  else {
    if (tree != NULL && tree->left == NULL && tree->right == NULL) {
        delete tree;
        tree = NULL;
    } else if (tree->right && tree->left) {
        node* temp_node = getMin(tree->right);
        tree->data = temp_node->data;
        tree->right = remove(tree->right, tree->data);
    } else {
        temp_node = tree;
        if (tree->right == NULL) {
            tree = tree->left;
        } else if (tree->left == NULL) {
            tree = tree->right;
            delete temp_node;
        }
    }
  }
  return tree;
}

node* BST::getMin(node* tree)
{
  if (tree == NULL)
    return tree;
  else if (tree->left == NULL)
    return tree;
  else return getMin(tree->left);
}

void BST::deleteTree(node* leaf)
{
  if (leaf != NULL) {
    deleteTree(leaf->left);
    deleteTree(leaf->right);
  }
  delete leaf;
}

string BST::inOrder(node* tree,string &str)
{
  if (tree != NULL) {
    inOrder(tree->left, str);
    str = str + std::to_string(tree->data) + " ";
    inOrder(tree->right, str);
  }
  return str;
}

string BST::preOrder(node* tree, string &str)
{
  if (tree != NULL) {
    str = str + std::to_string(tree->data) + " ";
    preOrder(tree->left, str);
    preOrder(tree->right, str);
  }

  return str;
}

string BST::postOrder(node* tree, string &str)
{
  if (tree != NULL) {
    postOrder(tree->left, str);
    postOrder(tree->right, str);
    str = str + std::to_string(tree->data) + " ";
  }
  return str;
}

}  // namespace utils
}  // namespace buzzdb
