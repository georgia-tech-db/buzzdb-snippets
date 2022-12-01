#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "bucket.h"


namespace buzzdb{
namespace utils{

// A class in C++ is a user-defined type that has data and functions 
// (also called member variables and member functions)
// The member variables are typically of built-in data types like integers
class Bucket;

class Directory{
public:

    // Constructor is used to create a "Course" object 
    // Default constructor
    Directory(size_t bucket_size);
    ~Directory();
    void insert(uint64_t val);
    size_t get_bucketid_by_val(uint64_t val);
    void update_hash_relation(size_t local_depth, std::unique_ptr<Bucket> new_bucket);

    // Overloading << operator for pretty printing objects of this class
    friend class Bucket;

    size_t global_depth;
    size_t bucket_size;
    size_t mask; // binary mask according to the global_depth. e.g. 1, 11, 111
    std::map<size_t, std::unique_ptr<Bucket>> buckets;
    std::map<std::pair<uint64_t, size_t>, size_t> hash_relation; // (suffix, bucketid)

private:
};
}
}