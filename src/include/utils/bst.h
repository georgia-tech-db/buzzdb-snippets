namespace buzzdb{
namespace BSTree{

// An entry in the tree
struct TreeNode {
  // Value of this node
  int value;
  // Nodes with values less than this node
  TreeNode *left;
  // Nodes with values greater than this node
  TreeNode *right;
};

class BSTree
{
public:
  // Constructor
  BSTree();
  // Destructor
  ~BSTree();

  // Tree attributes
  bool IsEmpty();

  // Returns true if value not already found in tree
  bool Insert(int value);
  bool Delete(int value);

  // If the given value is present in the tree
  bool Find(int value);

private:
  // Inserts a new node
  bool RecursiveInsert(int value, TreeNode** node);

  // Deletes an existing node
  bool RecursiveDelete(int value, TreeNode** node);

  // Pointer to root of tree
  TreeNode *root;
};

}  // namespace BSTree
}  // namespace buzzdb