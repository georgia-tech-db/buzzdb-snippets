#pragma once

#include <cstdint>
#include <memory>

namespace buzzdb{
namespace utils{

struct node {
  int data;
  node* left;
  node* right;
};

class BST 
{
  node* root;
  node* getMin(node*);
  node* remove(node*, int);
  void deleteTree(node*);
  void insert(node*, int);

public:
  BST();
  ~BST();
  void insert(int);
  node* remove(int);
  node* search(node*, int);
  std::string preOrder(node*, std::string&);
  std::string postOrder(node*, std::string&);
  std::string inOrder(node*, std::string&);
  node* getRoot();
};

}  // namespace utils
}  // namespace buzzdb
