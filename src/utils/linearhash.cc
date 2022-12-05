#include "utils/linearhash.h"

#define UNUSED(p) ((void)(p))

namespace buzzdb {
namespace utils {

    LinearHashTable::LinearHashTable(size_t capacity) {
        this->capacity = capacity;
        this->size = 0;
        items = std::vector<TableItem*>(capacity, NULL);
    }

    LinearHashTable::~LinearHashTable() {
        for (auto item : items) {
            if (item != NULL) {
                delete item;
            }
        }
        items.clear();
    }

    bool LinearHashTable::insert_item(int key, int value) {
        if (size == capacity) {
            return false;
        }

        size_t hash_index = key % capacity;
        TableItem* t_item = new TableItem(hash_index, key, value);

        size_t index = hash_index;
        while(true) {
            if (items[index] == NULL) {
                items[index] = t_item;
                size++;
                return true;
            } 

            index = (index + 1) % capacity;
        }

        return false;
    }

    bool LinearHashTable::delete_item(int key) {
        size_t index = key % capacity;
        bool deleted = false;

        while (true) {
            if (items[index] == NULL) {
                return false;
            }

            if (items[index]->key == key) {
                delete items[index];
                items[index] = NULL;
                deleted = true;
                size--;
                break;
            }

            index = (index + 1) % capacity;
        }

        backshift(index);

        return deleted;
    }

    void LinearHashTable::backshift(int index) {
        size_t next_index = (index + 1) % capacity;
        while(true) {
            if (items[next_index] == NULL or items[next_index]->key % capacity == next_index) {
                break;
            }

            items[index] = items[next_index];

            index = next_index;
            next_index = (next_index + 1) % capacity;
        }
        items[index] = NULL;
    }


    int LinearHashTable::find_item(int key) {
        size_t index = key % capacity;

        while (true) {
            if (items[index] == NULL) {
                return -1;
            }

            if (items[index]->key == key) {
                return items[index]->val;
            }

            index = (index + 1) % capacity;
        }
        return -1;
    }

    void LinearHashTable::print(){
        std::cout << "Capacity: " << size << "/" << capacity << "\n";

        for (size_t i = 0; i < capacity; i++) {
            if (items[i] == NULL) {
                std::cout << "[" << i << "] -> NULL \n";
            } 
            else {
                std::cout << "[" << items[i]->hash_index << "] -> ";
                std::cout << "key=" << items[i]->key;
                std::cout << " | value=" << items[i]->val << "\n";
            }
        }
    }

}
}