#include <vector>

namespace buzzdb {

    // A single node of the B+ tree. 
    class TreeNode {
    public:
        uint64_t node_id;
        bool is_leaf;
        bool is_locked;

        std::vector<uint64_t> values;
        std::vector<TreeNode*> children;

        TreeNode(std::vector<uint64_t> values, bool is_leaf, uint64_t node_id);
        void addChild(TreeNode* child);
        void setLock(bool lock_state);
    };

    // Demo class - creates a tree, prints it, and runs operations over the tree.
    class BPlusTreeLatchDemo {
        TreeNode* root;
        uint64_t depth;
        uint64_t max_children;
        std::vector<TreeNode*> locked_nodes;

        void prepareDemoTree();
        void printDemoTree();
        std::string constructSpaces(uint64_t n_spaces);
        TreeNode* getNextChild(uint64_t value, TreeNode* curr_node);
    public:
        BPlusTreeLatchDemo();
        void runReadOp(uint64_t value);
        void runDeleteOp(uint64_t value);
        void runInsertOp(uint64_t value);
    };
}