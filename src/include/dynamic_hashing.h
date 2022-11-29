#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

/**
 * This is a sample implementation for a hash table that uses
 * extendible hashing to dynamically grow. For more information refer,
 * https://faculty.cc.gatech.edu/~jarulraj/courses/4420-f22/slides/13-hash-tables.pdf
 */
namespace buzzdb {

/**
 * Container for storing all (key,value) pairs that hash to a given key
 */
struct bucket {
  size_t local_depth;
  size_t size;

  bucket(size_t depth, size_t size) : local_depth(depth), size(size) {}

  /**
     * Container for storing a (key,value) pair
   */
  struct KeyValue {
    uint64_t key;
    std::string value;

    bool operator==(const struct KeyValue &kv) const {
      return key == kv.key && value == kv.value;
    }
  };

  std::vector<KeyValue> keyValues;
};

/**
 * Implementation of a dynamic hash table
 */
class directory {
private:
  size_t global_depth_;
  std::unordered_map<uint64_t, std::shared_ptr<bucket>> table_;

public:
  explicit directory(uint64_t bucket_size);

  /**
     * Adds a key-value pair to the dynamic hash table
     * @param key
     * @param value
   */
  void insert(uint64_t key, std::string value);

  /**
     * Returns the value for the specified value
     * @param key
     * @return
   */
  std::optional<std::string> lookup(uint64_t key);

  /**
     * Returns the hash for the key by returning a
     * subset of the most significant bits of the key
     * @param key
     * @return
   */
  uint64_t hash(uint64_t key) const {
    return key & ((1L << global_depth_) - 1);
  }

  /**
     * Returns the global depth for the directory
     * @return
   */
  size_t get_global_depth() const { return global_depth_; }
};
} // namespace buzzdb
