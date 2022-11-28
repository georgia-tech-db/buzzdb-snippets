#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>

namespace buzzdb {
namespace utils {

class RobinHoodHashTable {
public:
    struct Entry {
        int key;
        int value;
        Entry(int key, int value): 
            key(key), value(value){}
    };

    size_t capacity;
    size_t size;

    RobinHoodHashTable(size_t capacity);
    ~RobinHoodHashTable();

    bool insert_item(int key, int value);
    bool delete_item(int key);
    Entry* find_item(int key);
    bool empty();
    bool full();
    friend std::ostream& operator<<(std::ostream& os, const RobinHoodHashTable& rh_table);

private:
    struct TableItem {
        size_t hash_index;
        int probe_dist;
        struct Entry* entry;
        TableItem(size_t hash_index, int probe_dist, Entry* entry): 
            hash_index(hash_index), probe_dist(probe_dist), entry(entry) {}
        TableItem():hash_index(0), probe_dist(0), entry(NULL) {}
    };

    std::vector<TableItem*> items;

    size_t key_hash_index(int key);
};

}
}