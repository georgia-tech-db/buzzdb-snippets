#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include "./utils/bst.h"
using namespace std;

namespace buzzdb {
namespace Tree {

#define UNUSED(p) ((void)(p))

Tree::Tree() {
  this->root = NULL;
}

    Tree :: Tree (int d) {
       data = d;
       right = NULL;
       left = NULL;
     };

  Tree::~Tree() {
  while (this->root != NULL) {
    del_node(root, root->data);
  }
}
     
 Node* search_node(Node *root, int key) {
     if ((root == NULL) || (root->data == key)) {
         return root;
     }

     if (root->data < key) {
         return search_node(root->right, key);
     }

     return search_node(root->left, key);
  }

  Node* insert_node(Node *root, int data) {
      if (root == NULL) {
          Node* newHead = new Node;
          return newHead;
      } else {
          Node* ptr;
          if (data <= root->data) {
              ptr = insert_node(root->left, data);
              root->left = ptr;
          } else {
               ptr = insert_node(root->right, data);
               root->right = ptr;
          }
          return root;
      }
  }

  void Inorder(Node *root) {
     if (root == NULL)
         return;
        
      Inorder(root->left);
      Inorder(root->right);
  }

   void Preorder(Node *root){
      if (root == NULL)
          return;
       cout << root->data << " ";
       Preorder(root->left);
       Preorder(root->right);
    }

    void Postorder(Node *root){
        if (root == NULL)
            return;
        
        Postorder(root->left);
        Postorder(root->right);
        cout << root->data << " ";
    }

   Node* del_node(Node *root, int value) { 
       if (root == NULL) { 
           return root;
       } 

       if (root->data > value){ 
           root->left = del_node(root->left, value); 
           return root; 
       } else if (root->data < value){ 
           root->right = del_node(root->right, value); 
           return root; 
       } 
  

       if (root->left == NULL) { 
           Node* temp = root->right;
           delete root; 
           return temp; 
       } else if (root->right == NULL) { 
          Node* temp = root->left; 
          delete root; 
          return temp; 
       } else {
          Node *Parent = root;
          Node *succ = root->right; 
          while (succ->left != NULL) { 
             Parent = succ; 
             succ = succ->left; 
          } 

          if (Parent != root) 
              Parent->left = succ->right; 
          else
              Parent->right = succ->right; 

          root->data = succ->data; 

          delete succ;          
          return root; 
       } 

}  // namespace bst

}  // namespace buzzdb
}