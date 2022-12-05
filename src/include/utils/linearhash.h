#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>

namespace buzzdb {
namespace utils {

class LinearHashTable {
public:

    size_t capacity;
    size_t size;

    LinearHashTable(size_t capacity);
    ~LinearHashTable();

    bool insert_item(int key, int value);
    bool delete_item(int key);
    int find_item(int key);
    void print();

private:
    struct TableItem {
        size_t hash_index;
        int key;
        int val;
        TableItem(size_t hash_index, int key, int val): hash_index(hash_index), key(key), val(val) {}
        TableItem():hash_index(0), key(-1), val(-1) {}
    };

    std::vector<TableItem*> items;
    void backshift(int);
};

}
}