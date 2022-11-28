#include "utils/robinhood.h"

#define UNUSED(p) ((void)(p))

namespace buzzdb {
namespace utils {

    RobinHoodHashTable::RobinHoodHashTable(size_t capacity) {
        this->capacity = capacity;
        this->size = 0;
        items = std::vector<TableItem*>(capacity, NULL);
    }

    RobinHoodHashTable::~RobinHoodHashTable() {
        for (auto item : items) {
            if (item != NULL) {
                delete item->entry;
                delete item;
            }
        }
        items.clear();
    }
    

    bool RobinHoodHashTable::insert_item(int key, int value) {
        if (full()) {
            return false;
        }

        size_t hash_index = key_hash_index(key);;
        TableItem* t_item = new TableItem(hash_index, 0, new Entry(key, value));
        // If TableItem at hash_index is empty, insert
        if (items[hash_index] == NULL) {
            items[hash_index] = std::move(t_item);
            size++;
            return true;
        }   

        // else, probe for place to insert
        size_t index = t_item->hash_index;
        TableItem* temp;
        while(true) {
            if (items[index] == NULL ) {
                items[index] = t_item;
                size++;
                return true;
            } 

            if(items[index]->probe_dist < t_item->probe_dist) {
                temp = items[index];
                items[index] = t_item;
                t_item = temp;
            }

            t_item->probe_dist++;
            index = (index + 1) % capacity;
        }

        return false;
    }

    bool RobinHoodHashTable::delete_item(int key) {
        size_t index = key_hash_index(key);
        int probe_dist = 0;
        bool deleted = false;

        // Find and delete key
        while (true) {
            if (items[index] == NULL) {
                return false;
            } else if (items[index]->probe_dist < probe_dist) {
                return false;
            }

            if (items[index]->entry->key == key) {
                delete items[index]->entry;
                delete items[index];
                items[index] = NULL;
                deleted = true;
                size--;
                break;
            }

            index = (index + 1) % capacity;
            probe_dist++;
        }

        // Backshift items
        size_t next_index = (index + 1) % capacity;
        while(true) {
            if (items[next_index] == NULL) {
                break;
            } else if (items[next_index]->probe_dist == 0) {
                break;
            }

            items[index] = items[next_index];
            items[index]->probe_dist -= 1;
            items[next_index] = NULL;

            index = next_index;
            next_index = (next_index + 1) % capacity;
        }

        return deleted;
    }

    RobinHoodHashTable::Entry* RobinHoodHashTable::find_item(int key) {
        size_t index = key_hash_index(key);
        int probe_dist = 0;

        while (true) {
            if (items[index] == NULL) {
                return NULL;
            } else if (items[index]->probe_dist < probe_dist) {
                return NULL;
            }

            if (items[index]->entry->key == key) {
                return items[index]->entry;
            }

            index = (index + 1) % capacity;
            probe_dist++;
        }
        return NULL;
    }
    bool RobinHoodHashTable::empty() {
        return size <= 0;
    }
    bool RobinHoodHashTable::full() {
        return size == capacity;
    }

    std::ostream& operator<<(std::ostream& os, const RobinHoodHashTable& rh_table) {
        os << "RobinHood HashTable - capacity: " << rh_table.capacity << ", size: " << rh_table.size << "\n";
        
        for (size_t i = 0; i < rh_table.capacity; i++) {
            if (rh_table.items[i] == NULL) {
                os << "[" << i << "] -> NULL \n";
            } else {
                os << "[" << i << "] -> {probe_dist=" << rh_table.items[i]->probe_dist;
                os << ", hash_index=" << rh_table.items[i]->hash_index;
                os << ", key=" << rh_table.items[i]->entry->key;
                os << ", value=" << rh_table.items[i]->entry->value << "} \n";
            }
        }
        return os;
    }

    size_t RobinHoodHashTable::key_hash_index(int key) {
        return (size_t) (key % capacity);
    }



}
}