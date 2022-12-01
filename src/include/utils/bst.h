#pragma once

#include <cstdint>
#include <memory>

namespace buzzdb{
namespace Tree{
struct Node {
  int data;
  Node* left;
  Node* right;
};
class Tree{
  public:
    Node* left;
    Node* right;
    int data;
      // Constructor
    Tree();
     Tree(int d);
    ~Tree();
      Node* search_node(Node *root, int key);
      Node* insert_node(Node *root, int data);
      void Inorder(Node *root);
      void Preorder(Node *root);
      void Postorder(Node *root);
      Node* del_node(Node *root, int value);
      Node *root; // pointer to root
};

}  // namespace utils
}  // namespace buzzdb
