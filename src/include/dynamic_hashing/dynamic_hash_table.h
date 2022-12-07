#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <optional>

namespace buzzdb{
    namespace dynamic_hashing {

        struct TableValue {
            uint64_t key;
            uint64_t value;
        };
        struct Bucket {
            size_t id;
            uint64_t bit_mask;
            std::vector<TableValue> values;
        };

        class HashTable {
            public:
                ~HashTable();
                HashTable(size_t bucket_size, size_t initial_bit_depth);
                void put(uint64_t key, uint64_t value);
                std::optional<uint64_t> get(uint64_t key);
                void remove(uint64_t key);
                size_t size();
                void print_backing_table();
            private:
                void insert_with_expansion(Bucket* split_bucket, TableValue new_value);
                size_t bucket_size;
                size_t num_values;
                size_t global_bucket_id;
                std::vector<Bucket*> backing_table;
        };

    } 
}
