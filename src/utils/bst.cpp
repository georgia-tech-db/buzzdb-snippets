/*
 * BST implementation in C++
 * Srishti Mendiratta
 */

#include <iostream>
using namespace std;

class Node {
 public:
     int data;
     Node *left;
     Node *right;
     Node (int d) {
       data = d;
       left = NULL;
       right = NULL;
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
          return new Node(data);
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
      cout << root->data << " ";
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
    }

};

int main() {
   Node bst(0);
   Node* root = NULL;
   root = bst.insert_node(root,55);
   root = bst.insert_node(root,65);
   root = bst.insert_node(root,45);
   root = bst.insert_node(root,85);
   root = bst.insert_node(root,35);
   root = bst.insert_node(root,95);
   root = bst.insert_node(root,25);
   root = bst.insert_node(root,75);
   root = bst.insert_node(root,15);
   bst.Inorder(root);
   cout << "\n";
   bst.Preorder(root);
   cout << "\n";
   bst.Postorder(root);
   cout << "\n";
   root = bst.del_node(root,35);
   bst.Inorder(root);
   cout << "\n";
   root = bst.del_node(root,15);
   bst.Inorder(root);
} 
