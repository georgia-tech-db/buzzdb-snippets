
#include <algorithm>
#include <fstream>
#include <iostream>
#include <utils/linear_hashing.h>

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))

size_t LinearHashTable::hash(int key) {
    return key % capacity;
}

size_t LinearHashTable::size() {
    return sz;
}

LinearHashTable::Entry *LinearHashTable::lookup_entry(int key) {
    size_t index = hash(key);
    Entry *e = &table[index];

    for (size_t i = 0; i < capacity; i++) {
        e = &table[(index + i) % capacity];
        
        if (!e->full) return nullptr;
        if (e->key == key) {
            if (e->tombstone) return nullptr;
            else break;
        }
    }
    if (e->key != key) return nullptr; // iterated through vector

    return e;
}

void LinearHashTable::insert(int key, int val) {
    size_t index = hash(key);
    Entry *e = &table[index];;

    for (size_t i = 0; i < capacity; i++) {
        e = &table[(index + i) % capacity];
        
        if (!e->full || e->key == key) {
            break;
        }
    }
    if (e->full && e->key != key) return; // vector full

    if (!e->full || e->tombstone) sz++;
    *e = Entry(key, val);
}

int LinearHashTable::erase(int key) {
    Entry *e = lookup_entry(key);

    if (e == nullptr) return -1;
    e->tombstone = true;
    sz--;

    return e->val;
}

int LinearHashTable::lookup(int key) {
    Entry *e = lookup_entry(key);
    return e == nullptr ? -1 : e->val;
}

}  // namespace utils
}  // namespace buzzdb
