#include <math.h>
#include <algorithm>
#include "dynamic_hashing/dynamic_hash_table.h"
#include <iostream>

namespace buzzdb{
    namespace dynamic_hashing {
        
        #define UNUSED(p) ((void)(p))

        // the initial table size is 2^initial_bit_depth
        HashTable::HashTable(size_t bucket_size, size_t initial_bit_depth): bucket_size(bucket_size), num_values(0) {
            size_t size_init = 1 << initial_bit_depth;
            uint64_t initial_bit_mask = size_init - 1;
            size_t curr_id = 0;
            for (size_t i = 0; i < size_init; i++) {
                Bucket* new_bucket = new Bucket();
                new_bucket->id = curr_id++;
                new_bucket->bit_mask = initial_bit_mask;
                new_bucket->values = std::vector<TableValue>{};
                backing_table.push_back(new_bucket);
            }
            global_bucket_id = curr_id;
        }

        void HashTable::print_backing_table() {
            for (size_t i = 0; i < this->backing_table.size(); i++) {
                Bucket* b = this->backing_table[i];
                std::cout << i << ": {bit_mask: " << b->bit_mask << ", id: " << b->id << ", values: [";
                for (TableValue t: b->values) {
                    std::cout << "[" << t.key << ", " << t.value << "]";
                }
                std::cout << "]}" << std::endl;
            }
            std::cout << std::endl;
        }

        void HashTable::put(uint64_t key, uint64_t value) {
            size_t global_bit_mask = backing_table.size() - 1;
            Bucket* to_search = backing_table[key & global_bit_mask];
            // first, check to see if the key is already in the table. If it is, overwrite the current valueW
            for (size_t i = 0; i < to_search->values.size(); i++) {
                if (to_search->values[i].key == key) {
                    to_search->values[i].value = value;
                    return;
                }
            }
            // Otherwise, it must not be in the table, so we will insert this k/v pair
            TableValue new_value;
            new_value.key = key;
            new_value.value = value;
            // If the corresponding bucket is full, we have to split the table
            if (to_search->values.size() == bucket_size) {
                this->insert_with_expansion(to_search, new_value);
            } else {
                to_search->values.push_back(new_value);
            }
            num_values++;
        }

        void HashTable::insert_with_expansion(Bucket* split_bucket, TableValue new_value) {
            size_t new_local_bit_mask = (split_bucket->bit_mask << 1) + 1;
            bool found_new_value = false;
            // keep expanding the local bit mask until we know for sure all new buckets won't overflow
            while (!found_new_value) {
                uint64_t new_hash_value = split_bucket->values[0].key & new_local_bit_mask;
                for (TableValue t: split_bucket->values) {
                    if ((t.key & new_local_bit_mask) != new_hash_value) {
                        found_new_value = true;
                        break;
                    }
                }
                if ((new_value.key & new_local_bit_mask) != new_hash_value) {
                    found_new_value = true;
                }
                if (!found_new_value) {
                    new_local_bit_mask = (new_local_bit_mask << 1) + 1;
                }
            }
            // if the new local bit mask requires more buckets than available in the current table,
            // expand the current table
            bool expanding_table = new_local_bit_mask > backing_table.size() - 1;
            if (expanding_table) {
                size_t new_backing_table_size = new_local_bit_mask + 1;
                std::vector<Bucket*> new_backing_table;
                for (size_t i = 0; i < new_backing_table_size; i++) {
                    size_t old_bit_mask = backing_table.size() - 1;
                    Bucket* old_bucket = backing_table[i & old_bit_mask];
                    if (old_bucket->id != split_bucket->id) {
                        new_backing_table.push_back(old_bucket);
                    } else {
                        Bucket* new_bucket = new Bucket();
                        new_bucket->id = this->global_bucket_id++;
                        new_bucket->bit_mask = new_local_bit_mask;
                        new_bucket->values = std::vector<TableValue>{};
                        new_backing_table.push_back(new_bucket);
                    }
                }
                backing_table = new_backing_table;
                for (TableValue t: split_bucket->values) {
                    Bucket* corresponding_bucket = backing_table[t.key & new_local_bit_mask];
                    corresponding_bucket->values.push_back(t);
                }
                backing_table[new_value.key & new_local_bit_mask]->values.push_back(new_value);
                delete split_bucket;
            } else {
                // otherwise, split the current bucket into new buckets and update all references
                // in the table to ensure lookup is still O(1)
                std::vector<TableValue> values;
                for (TableValue t: split_bucket->values) {
                    values.push_back(t);
                }
                values.push_back(new_value);
                split_bucket->values.clear();
                for (TableValue t: values) {
                    Bucket* corresponding_bucket = backing_table[t.key & new_local_bit_mask];
                    if (corresponding_bucket->id == split_bucket->id) {
                        Bucket* new_bucket = new Bucket();
                        new_bucket->id = this->global_bucket_id++;
                        new_bucket->bit_mask = new_local_bit_mask;
                        new_bucket->values = std::vector<TableValue>{};
                        // begin hacky way of make sure we don't have memory leaks
                        std::vector<uint64_t> old_bucket_ids;
                        std::vector<Bucket*> old_buckets;
                        for (size_t i = 0; i < backing_table.size(); i++) {
                            if (std::count(old_bucket_ids.begin(), old_bucket_ids.end(), backing_table[i]->id) == 0) {
                                old_bucket_ids.push_back(backing_table[i]->id);
                                old_buckets.push_back(backing_table[i]);
                            }
                            if (((i & new_local_bit_mask) == (t.key & new_local_bit_mask)) && new_local_bit_mask > backing_table[i]->bit_mask) {
                                backing_table[i] = new_bucket;
                            }
                        }
                        std::vector<uint64_t> new_ids;
                        for (size_t i = 0; i < backing_table.size(); i++) {
                            new_ids.push_back(backing_table[i]->id);
                        }
                        for (Bucket* b: old_buckets) {
                            if (std::count(new_ids.begin(), new_ids.end(), b->id) == 0) {
                                delete b;
                            }
                        }
                        // end hacky memory leak code
                        corresponding_bucket = new_bucket;
                    }
                    corresponding_bucket->values.push_back(t);
                }
            }
        }

        std::optional<uint64_t> HashTable::get(uint64_t key) {
            size_t global_bit_mask = backing_table.size() - 1;
            Bucket* to_search = backing_table[key & global_bit_mask];
            for (TableValue t: to_search->values) {
                if (t.key == key) {
                    return t.value;
                }
            }
            return {};
        }

        void HashTable::remove(uint64_t key) {
            size_t global_bit_mask = backing_table.size() - 1;
            Bucket* to_search = backing_table[key & global_bit_mask];
            for (size_t index = 0; index < to_search->values.size(); index++) {
                if (to_search->values[index].key == key) {
                    to_search->values.erase(to_search->values.begin() + index);
                    num_values--;
                    return;
                }
            }
        }

        size_t HashTable::size() {
            return this->num_values;
        }

        HashTable::~HashTable() {
            std::vector<Bucket*> to_delete;
            std::vector<size_t> already_deleted;
            for (Bucket* bucket: backing_table) {
                if (std::count(already_deleted.begin(), already_deleted.end(), bucket->id) == 0) {
                    already_deleted.push_back(bucket->id);
                    to_delete.push_back(bucket);
                }
            }
            for (Bucket* b: to_delete) {
                delete b;
            }
            backing_table.clear();
        }
    }
}
