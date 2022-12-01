#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "directory.h"
namespace buzzdb{
namespace utils{

class Directory;
class Bucket{
public:
    Bucket(size_t bucket_size, size_t local_depth, size_t id, Directory* directory);
    ~Bucket();
    void insert(uint64_t val);
    std::unique_ptr<Bucket> split();
    size_t id;
    uint64_t mask; // binary mask according to the local_depth. e.g. 1, 11, 111
    size_t bucket_size;
    size_t local_depth;
    Directory* directory;
    std::vector<uint64_t> vals;
private:



};

}
}