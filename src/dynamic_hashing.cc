#include "dynamic_hashing.h"
#include <cmath>

namespace buzzdb {

directory::directory(uint64_t bucket_size) {
  table_.insert({0, std::make_shared<bucket>(1, bucket_size)});
  table_.insert({1, std::make_shared<bucket>(1, bucket_size)});
  global_depth_ = 1;
}

void directory::insert(uint64_t key, std::string value) {
  auto orig_key = hash(key);
  auto orig_bucket = table_[orig_key];

  // bucket has space, add entry
  if (orig_bucket->keyValues.size() < orig_bucket->size) {
    orig_bucket->keyValues.push_back({key, value});
  }
  // bucket is full, need to create a new bucket
  else {
    // can do without doubling directory, just need new buckets
    if (orig_bucket->local_depth < global_depth_) {
      orig_bucket->local_depth++;

      // move values around for the two keys sharing the same bucket
      bool other_bucket_reset = false;
      for (size_t i = 0; i < orig_bucket->keyValues.size(); i++) {
        // hash the keys again to see if some of them will
        // move to another bucket
        auto other_key = hash(orig_bucket->keyValues[i].key);
        if (other_key != orig_key) {
          if (!other_bucket_reset) {
            other_bucket_reset = true;
            table_[other_key] =
                std::make_shared<bucket>(orig_bucket->local_depth, orig_bucket->size);
          }
          table_[other_key]->keyValues.push_back(orig_bucket->keyValues[i]);
          orig_bucket->keyValues.erase(orig_bucket->keyValues.begin() + i);
        }
      }

    }
    // directory needs to be doubled
    else {
      // for all but the original key, duplicate the key values
      for (auto &entry: table_) {
        if (entry.first != orig_key) {
          auto new_key = entry.first + (uint64_t) pow(2, global_depth_);
          table_.insert({new_key, entry.second});
        }
      }

      // for original key, redistribute values
      auto new_key = orig_key + (uint64_t) pow(2, global_depth_);
      orig_bucket->local_depth++;
      global_depth_++;
      table_.insert(
          {new_key, std::make_shared<bucket>(orig_bucket->local_depth, orig_bucket->size)});

      for (size_t i = 0; i < orig_bucket->keyValues.size(); i++) {
        if (hash(orig_bucket->keyValues[i].key) != orig_key) {
          table_[new_key]->keyValues.push_back(orig_bucket->keyValues[i]);
          orig_bucket->keyValues.erase(orig_bucket->keyValues.begin() + i);
        }
      }
    }

    // finally insert the key, which caused the split of buckets in both the
    // cases
    insert(key, value);
  }
}

std::optional<std::string> directory::lookup(uint64_t key) {
  auto bucket = table_[hash(key)];
  for (auto &kv: bucket->keyValues) {
    if (kv.key == key) {
      return kv.value;
    }
  }
  return std::nullopt;
}
} // namespace buzzdb