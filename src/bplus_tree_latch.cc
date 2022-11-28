#include <iostream>
#include <queue>
#include <cmath>

#include "bplus_tree_latch.h"

namespace buzzdb {
    void TreeNode::addChild(TreeNode* child) {
        this->children.push_back(child);
    }

    TreeNode::TreeNode(std::vector<uint64_t> values, bool is_leaf, uint64_t node_id) {
        this->values = values;
        this->is_leaf = is_leaf;
        this->is_locked = false;
        this->node_id = node_id;
    }

    void TreeNode::setLock(bool lock_state) {
        if (lock_state) {

        }

        std::cout << "-> Updating node " 
            << this->node_id 
            << "'s latch state: " 
            << (lock_state ? std::string("Locked") : std::string("Unlocked"))
            << std::endl;

        this->is_locked = lock_state;
    }

    BPlusTreeLatchDemo::BPlusTreeLatchDemo() {
        this->prepareDemoTree();
        this->printDemoTree();
    }

    // NOTE: This is hardcoded to work only when there's a fan-out of 3 children or lesser. 
    TreeNode* BPlusTreeLatchDemo::getNextChild(uint64_t value, TreeNode* curr) {
        if (value < curr->values.at(0))  {
            return curr->children.at(0);
        }
        else if ((value >= curr->values.at(0) && curr->values.size() <= 1) 
            || (value >= curr->values.at(0) && value < curr->values.at(1))) {
            return curr->children.at(1);
        } else {
            return curr->children.at(2);
        }
    }

    void BPlusTreeLatchDemo::runReadOp(uint64_t value) {
        std::cout << "Value to read: " << std::to_string(value) << std::endl;

        this->locked_nodes.clear();
        TreeNode* curr = this->root;
        this->root->setLock(true);
        this->locked_nodes.push_back(curr);

        while (!curr->is_leaf) {
            curr = getNextChild(value, curr);
            curr->setLock(true);

            for (TreeNode* node: locked_nodes) {
                node->setLock(false);
            }
            this->locked_nodes.clear();
            this->locked_nodes.push_back(curr);
        }

        std::cout << "Reached node with value to be read." << std::endl;
        curr->setLock(false);
        std::cout << std::endl;
    }

    void BPlusTreeLatchDemo::runDeleteOp(uint64_t value)  {
        std::cout << "Value to delete: " << std::to_string(value) << std::endl;     

        this->locked_nodes.clear();
        TreeNode* curr = this->root;
        this->root->setLock(true);
        this->locked_nodes.push_back(curr);

        while (!curr->is_leaf) {
            curr = getNextChild(value, curr);
            curr->setLock(true);

            if (curr->values.size() > 1) {
                std::cout << "Node " << std::to_string(curr->node_id) << " has enough elements to handle a deletion. " << std::endl;
                for (TreeNode* node: locked_nodes) {
                    node->setLock(false);
                }
                locked_nodes.clear();
            }
            
            locked_nodes.push_back(curr);   
        }

        std::cout << "Reached node with value to be deleted." << std::endl;
        for (TreeNode* node: locked_nodes) {
            node->setLock(false);
        }
        std::cout << std::endl;
    }

    void BPlusTreeLatchDemo::runInsertOp(uint64_t value) {
        std::cout << "Value to insert: " << std::to_string(value) << std::endl;     

        this->locked_nodes.clear();
        TreeNode* curr = this->root;
        this->root->setLock(true);
        this->locked_nodes.push_back(curr);

        while (!curr->is_leaf) {
            curr = getNextChild(value, curr);
            curr->setLock(true);

            if (curr->values.size() < max_children - 1) {
                std::cout << "Node " << std::to_string(curr->node_id) << " has free space to handle a new value. " << std::endl;
                for (TreeNode* node: locked_nodes) {
                    node->setLock(false);
                }
                locked_nodes.clear();
            }
            
            locked_nodes.push_back(curr);   
        }

        std::cout << "Reached node where insertion happens." << std::endl;
        for (TreeNode* node: locked_nodes) {
            node->setLock(false);
        }
        std::cout << std::endl;
    }

    std::string BPlusTreeLatchDemo::constructSpaces(uint64_t n_spaces) {
        std::string s;
        for (uint64_t i = 0; i < n_spaces; i++) {
            s += ' ';
        }

        return s;
    }

    void BPlusTreeLatchDemo::printDemoTree() {
        std::cout << "Reference tree structure: " << std::endl;

        std::vector<std::string> output;
        uint64_t curr_level = 0;
        std::queue<std::pair<uint64_t, TreeNode*>> visitQueue;

        visitQueue.push(std::pair<uint64_t, TreeNode*>(1, root));
        while (visitQueue.size() > 0) {
            uint64_t node_level = visitQueue.front().first;
            TreeNode* node_ref = visitQueue.front().second;
            visitQueue.pop();

            if (node_level > curr_level) {
                curr_level = node_level;
                output.push_back(this->constructSpaces(pow(max_children, ((depth+2)-curr_level))));
            } else {
                output.back() += this->constructSpaces(pow(max_children, ((depth+3)-curr_level)));
            }

            output.back() += "Node " + std::to_string(node_ref->node_id);
            output.back() += '(';
            for (uint64_t idx = 0; idx < max_children - 1; idx++)  {
                if (idx >= node_ref->values.size()) {
                    output.back() += '_';
                } else {
                    output.back() += std::to_string(node_ref->values.at(idx));
                }

                if (idx != max_children - 2) {
                    output.back() += ',';
                }
            }
            output.back() += ')';

            if (node_ref->is_leaf) {
                continue;
            }

            for (TreeNode* child: node_ref->children) {
                visitQueue.push(std::pair<uint64_t, TreeNode*>(node_level + 1, child));
            }
        }
        

        for (std::string line: output) {
            std::cout << line << std::endl << std::endl;
        }
    }

    void BPlusTreeLatchDemo::prepareDemoTree() {
        this->root = new TreeNode(std::vector<uint64_t>{20}, false, 0);
        TreeNode* leftChild = new TreeNode(std::vector<uint64_t>{10}, false, 1);
        TreeNode* rightChild = new TreeNode(std::vector<uint64_t>{20, 30}, false, 2);

        TreeNode* leftLeftChild = new TreeNode(std::vector<uint64_t>{5}, true, 3);
        TreeNode* leftRightChild = new TreeNode(std::vector<uint64_t>{10, 15}, true, 4);

        TreeNode* rightLeftChild = new TreeNode(std::vector<uint64_t>{}, true, 5);
        TreeNode* rightRightChild = new TreeNode(std::vector<uint64_t>{20}, true, 6);
        TreeNode* rightMostChild = new TreeNode(std::vector<uint64_t>{30, 35}, true, 7);

        root->addChild(leftChild);
        root->addChild(rightChild);

        leftChild->addChild(leftLeftChild);
        leftChild->addChild(leftRightChild);

        rightChild->addChild(rightLeftChild);
        rightChild->addChild(rightRightChild);
        rightChild->addChild(rightMostChild);

        this->depth = 2;
        this->max_children = 3;
    }
}