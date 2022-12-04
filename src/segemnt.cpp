#include"include/segment.h"

namespace buzzdb{

std::vector<int> block::read(){
    return this->data;
}

bool block::write(int ch){
    if (this->size == MAX_BLOCK_SIZE){
        return false;
    }
    this->data.push_back(ch);
    this->size++;
    return true;
}

std::vector<int> extent::read(uint64_t block_index){
    return this->blocks[block_index].read();
}

bool extent::write(int ch){
    for (uint64_t ii = 0; ii < this->size; ++ii){
        if (this->blocks[ii].write(ch))return true;
    }
    if (this->size >= MAX_EXTENT_SIZE)return false;


    block new_block;
    this->blocks.push_back(new_block);
    this->size++;
    return this->blocks[this->size].write(ch);
}

std::vector<int> segment::read(uint64_t extent_index, uint64_t block_index){
        return this->extents[extent_index].read(block_index);
    }

bool segment::write(int ch){
    for (uint64_t ii = 0; ii < this->extent_num; ++ii){
        if (this->extents[ii].write(ch))return true;
    }


    extent new_extent;
    this->extents.push_back(new_extent);
    this->extent_num++;
    return this->extents[this->extent_num].write(ch);
}

uint64_t segment::get_extent_num(){
    return this->extent_num;
};
} // namespace buzzdb