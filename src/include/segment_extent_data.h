#pragma once



#include <iostream>
#include <memory>
#include <bits/stdc++.h>

namespace buzzdb{
namespace utils{



class Datablocks {
    public:
    std::string data; 
    Datablocks();
    Datablocks(std::string data){
        this->data = data;
    }

};


class Extent{
public:
    std::vector<Datablocks> blocks;
    size_t max_extent_size;
   
    bool is_overflowing(){
        return blocks.size() > max_extent_size;
    }

    Extent* split_the_extent(){
        
        auto ret = new Extent(blocks, max_extent_size);  
        blocks.resize((int)max_extent_size);
        return ret;
    }   

    Extent(std::vector<Datablocks> blocks, size_t max_extent_size){
        std::vector<Datablocks> temp(blocks.begin()+ (int)max_extent_size, blocks.end());
        this->max_extent_size = max_extent_size;
        this->blocks = temp;

    }

    Extent(size_t max_extent_size){
        this->max_extent_size = max_extent_size;
    }

    ~Extent();
    
};


// A class in C++ is a user-defined type that has data and functions 
// (also called member variables and member functions)
// The member variables are typically of built-in data types like integers
class Segment{
public:
    
    std::vector<Extent> vv;
    size_t max_segment_size;
    // Constructor is used to create a "Course" object 
    // Default constructor
    Segment();

    // There can be multiple constructors
    Segment(size_t max_segment_size);

    // Copy constructor
	Segment(const Segment& segment, size_t max_segment_size);

    
};






// Inheritance: Deriving new classes (sub classes) from existing ones such as super class or base class and then forming them into a hierarchy of classes


}  
} 
