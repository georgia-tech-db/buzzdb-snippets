#include <cassert>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>

#include "buffer_manager.h"

namespace buzzdb{

typedef std::pair<size_t, uint64_t> frame;

// A demo for Q1 in exercise sheet 3:

// Consider a buffer manager that is managing a database with 8 pages using a buffer
// pool with 3 in-memory frames. Pages are numbered 1 through 8 and buffer pool
// frames are numbered 1 through 3

// transfer each test case into the a vector of page ids
std::vector<uint64_t> parse_input(std::string page_id_string){
    std::vector<uint64_t> page_ids;
    std::istringstream in(page_id_string);
    std::string page_id;
    while(in >> page_id){
        page_ids.push_back(std::stoi(page_id));
    }
    return page_ids;
}

void print_queue(std::vector<frame> queue){
    for(size_t i = 0; i < queue.size(); i++){
        std::cout << "{" << queue[i].first << ", " << queue[i].second << "}";
    }
}

std::vector<frame> fifo(std::vector<uint64_t> page_ids){
    std::vector<frame> fifo_queue;
    uint64_t page_fault = 0;

    for(size_t i = 0; i < page_ids.size(); i++){
        auto page_id = page_ids[i];
        std::cout << "After Insert " << page_id << ": ";
        bool found_page = false;

        for(size_t j = 0; j < fifo_queue.size(); j++){
            if(fifo_queue[j].second == page_id) {
                found_page = true;
                break;
            }
        }
        // if not found, use fifo policy
        if(!found_page){
            page_fault += 1;
            size_t frame_id;

            if(fifo_queue.size() < 3){
                frame_id = fifo_queue.size() + 1;
            }else{
                frame_id = fifo_queue[0].first;
                fifo_queue.erase(fifo_queue.begin());
            }
            fifo_queue.push_back(std::make_pair(frame_id, page_id));
        }
        // print out fifo_queue
        print_queue(fifo_queue);
        std::cout << std::endl;
    }
    std::cout << "Page fault: " << page_fault << std::endl;
    return fifo_queue;
}

std::vector<frame> lru(std::vector<uint64_t> page_ids){
    std::vector<frame> lru_queue;
    uint64_t page_fault = 0;

    for(size_t i = 0; i < page_ids.size(); i++){
        auto page_id = page_ids[i];
        std::cout << "After Insert " << page_id << ": ";
        bool found_page = false;

        for(size_t j = 0; j < lru_queue.size(); j++){
            if(lru_queue[j].second == page_id) {
                found_page = true;
                size_t frame_id = lru_queue[j].first;
                lru_queue.erase(lru_queue.begin() + j);
                lru_queue.push_back(std::make_pair(frame_id, page_id));
                break;
            }
        }
        // if not found, use lru policy
        if(!found_page){
            page_fault += 1;
            size_t frame_id;

            if(lru_queue.size() < 3){
                frame_id = lru_queue.size() + 1;
            }else{
                frame_id = lru_queue[0].first;
                lru_queue.erase(lru_queue.begin());
            }
            lru_queue.push_back(std::make_pair(frame_id, page_id));
        }
        // print out lru_queue
        print_queue(lru_queue);
        std::cout << std::endl;
    }
    std::cout << "Page fault: " << page_fault << std::endl;
    return lru_queue;
}

std::pair<std::vector<frame>, std::vector<frame>> two_queue(std::vector<uint64_t> page_ids){
    std::vector<frame> fifo_queue;
    std::vector<frame> lru_queue;
    uint64_t page_fault = 0;

    for(size_t i = 0; i < page_ids.size(); i++){
        auto page_id = page_ids[i];
        std::cout << "After Insert " << page_id << ": ";
        bool found_page = false;

        // first check lru_queue
        for (size_t j = 0; j < lru_queue.size(); j++) {
            if(lru_queue[j].second == page_id) {
                found_page = true;
                size_t frame_id = lru_queue[j].first;
                lru_queue.erase(lru_queue.begin() + j);
                lru_queue.push_back(std::make_pair(frame_id, page_id));
                break;
            }
        }
        // then check fifo_queue
        if(!found_page) {
            for (size_t j = 0; j < fifo_queue.size(); j++) {
                if(fifo_queue[j].second == page_id) {
                    found_page = true;
                    size_t frame_id = fifo_queue[j].first;
                    // take out from fifo_queue
                    fifo_queue.erase(fifo_queue.begin() + j);
                    lru_queue.push_back(std::make_pair(frame_id, page_id));
                    break;
                }
            }
        }
        // if not already exist in queue, put into fifo_queue
        if(!found_page){
            page_fault += 1;
            size_t frame_id;

            if(fifo_queue.size() + lru_queue.size() < 3){
                frame_id = fifo_queue.size() + 1;
            }else if(fifo_queue.size() > 0){
                frame_id = fifo_queue[0].first;
                fifo_queue.erase(fifo_queue.begin());
            }else{
                frame_id = lru_queue[0].first;
                lru_queue.erase(fifo_queue.begin());
            }
            fifo_queue.push_back(std::make_pair(frame_id, page_id));
        }

        // print out fifo_queue and lru_queue
        std::cout << "FIFO: ";
        print_queue(fifo_queue);
        std::cout << "; ";
        std::cout << "LRU: ";
        print_queue(lru_queue);
        std::cout << std::endl;
    }
    std::cout << "Page fault: " << page_fault << std::endl;
    return std::make_pair(fifo_queue, lru_queue);
}

}  // namespace buzzdb