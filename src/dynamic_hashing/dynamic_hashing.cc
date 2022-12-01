#include "dynamic_hashing/bucket.h"
#include "dynamic_hashing/directory.h"
namespace buzzdb{
namespace utils {

Directory::Directory(size_t bucket_size): bucket_size(bucket_size) {
    // init global depth as 1
    global_depth = 1;
    mask = 1 << 0;
    // init with two buckets
    auto ptr_bucket1 = std::make_unique<Bucket>(bucket_size, 1, 0, this);
    auto ptr_bucket2 = std::make_unique<Bucket>(bucket_size, 1, 1, this);
    buckets[ptr_bucket1->id] = std::move(ptr_bucket1);
    buckets[ptr_bucket2->id] = std::move(ptr_bucket2);
    hash_relation[std::make_pair(0, 1)] = 0;
    hash_relation[std::make_pair(1, 1)] = 1;

}
Directory::~Directory() { };
void Directory::insert(uint64_t val) {
    auto bucket_id = get_bucketid_by_val(val);
    buckets[bucket_id]->insert(val);
}
size_t Directory::get_bucketid_by_val(uint64_t val) {
    auto suffix = val & mask;
    return hash_relation[std::make_pair(suffix, global_depth)];
}

void Directory::update_hash_relation(size_t local_depth, std::unique_ptr<Bucket> new_bucket) {
    if (global_depth >= local_depth) {
        // other buckets with finer granularity already exists
        // no need to modify them
    } else {
        global_depth++;
        mask <<= 1;
        mask += 1;
        std::map<std::pair<uint64_t, size_t>, size_t> tmp;
        tmp.insert(hash_relation.begin(), hash_relation.end());
        // update all 
        for (auto& entry : tmp) {
            if (entry.first.second < global_depth) {
                // update the key of orginal buckets
                auto new_key = std::make_pair(entry.first.first, global_depth);
                hash_relation[new_key] = hash_relation[entry.first];
                hash_relation.erase(entry.first);
                // make entry for new key
                auto cur_id = entry.second;
                auto new_bucket_id = entry.first.first + (1 << (global_depth - 1));
                // still point to the old bucket, because there is no actual split
                hash_relation[std::make_pair(new_bucket_id, global_depth)] = cur_id;
            }
        }
    }
    // only update the infected bucket
    hash_relation[std::make_pair(new_bucket->id, global_depth)] = new_bucket->id;
    buckets[new_bucket->id] = std::move(new_bucket);
}


Bucket::Bucket(size_t bucket_size, size_t local_depth, size_t id, Directory* directory): 
id(id), bucket_size(bucket_size), local_depth(local_depth) {
    // pass 
    this->directory = directory;
    mask = 1 << local_depth;
    mask -= 1;
}
Bucket::~Bucket(){};
void Bucket::insert(uint64_t val) {
    if (vals.size() < bucket_size) {
        vals.push_back(val);
    } else {
        // need to split
        // also update the directory
        vals.push_back(val);
        directory->update_hash_relation(local_depth, std::move(split()));

    }
}

std::unique_ptr<Bucket> Bucket::split(){
    local_depth++;
    mask <<= 1;
    mask += 1;
    auto new_id = id + (1 << (local_depth - 1));
    auto new_bucket = std::make_unique<Bucket>(bucket_size, local_depth, new_id, directory);
    // split the values to correct bucket
    std::vector<uint64_t> tmp(vals);
    for (int64_t i = tmp.size() - 1; i >= 0; i--) {
        auto val = tmp.at(i);
        if ((val & mask) == new_id) {
            // should go to new buckets
            vals.erase(vals.begin() + i);
            new_bucket->vals.push_back(val);
        }
    }
    return new_bucket;
}
}}