#include <string>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "utils/lru_cache.h"

namespace buzzdb{
namespace utils {

#define UNUSED(p) ((void)(p))

void LruCache::addEntry(string str) {
    if (cache[str] == unordered_map::end) {
        if (count == 3) {
            cache[str] = count + 1;
            for (auto entry : cache) {
                if (entry.second == 1) {
                    cache.erase(entry.first);
                }
                cache[entry.first] = entry.second - 1;
            }
        } else {
            count = count + 1;
            cache[str] = count;
        }
    } else {
        for (auto entry : cache) {
            if (entry.first == str) {
                cache[entry.first] = 0;
            }
            cache[entry.first] = entry.second + 1;
        }
    }
}

int LruCache::getOrderOfEntry(string str) {
    return cache[str] == unordered_map::end ? -1 : cache[str];
}

}
}