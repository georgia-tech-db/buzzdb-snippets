#pragma once

#include <iostream>
#include <memory>

namespace buzzdb{
namespace utils{

class LruCache {       
  public:      
    unordered_map<string, int> cache;
    int count = 0;

    void addEntry(string str);
    int getOrderOfEntry(string str)  
};


}  
} 
