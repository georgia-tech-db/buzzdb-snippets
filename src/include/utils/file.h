#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <optional>

using namespace std;
namespace buzzdb{
namespace utils{
class RHT{
    public:
        RHT(int table_size);
        int table_size;
        int keys_inserted;
        struct data {
            int key;
            // optional <int> val;
            int val;
        };
        vector<data> data_items;
        vector<int> probe_seq_len;
        int key_hasher(int key);
        bool insert(int key, int val);
        pair<int, bool> search(int key);
        bool erase(int key);
};




}  // namespace utils
}  // namespace buzzdb
