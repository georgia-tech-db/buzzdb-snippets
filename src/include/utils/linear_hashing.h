#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <iostream>

namespace buzzdb{
namespace utils{

class LinearHashTable {
    public:
        struct Entry {
            int key;
            int val;
            bool tombstone;
            bool full;

            Entry(int key, int val): key(key), val(val), tombstone(false), full(true) {};
            Entry(int key, int val, bool ts, bool full): key(key), val(val), tombstone(ts), full(full) {};
            ~Entry() {};
            void set_tombstone(bool ts) { this->tombstone = ts; };
            void set_full(bool full) { this->full = full; };
            bool operator==(const Entry& e) const {
                return key == e.key && val == e.val && tombstone == e.tombstone && full == e.full;
            };
            friend std::ostream& operator<<(std::ostream& os, const Entry& e) {
                os << "Entry(" << e.key << "," << e.val << "," << e.tombstone << "," << e.full << ")";
                return os;
            };
        };

        LinearHashTable(size_t capacity) {
            this->capacity = capacity;
            this->sz = 0;
            
            for (size_t i = 0; i < capacity; i++) {
                table.push_back(Entry(0, 0, false, false));
            }
        };
        ~LinearHashTable() {};
        void insert(int, int);
        int erase(int);
        int lookup(int);
        size_t size();
        std::vector<Entry> get_backing_vector() { return table; };

    private:
        size_t capacity;
        size_t sz;
        std::vector<Entry> table;

        size_t hash(int);
        Entry *lookup_entry(int);
};

}  // namespace utils
}  // namespace buzzdb