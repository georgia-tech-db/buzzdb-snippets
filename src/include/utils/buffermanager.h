using namespace std;
#include <cstddef>
#include <cstdint>
#include <exception>
#include <vector>
#include <map>
#include <memory>

#include "common/macros.h"

namespace buzzdb {

class BufferFrame {
private:
    friend class BufferManager;

    uint64_t page_number;
    bool dirty_bit;
    int counter;
    vector<uint64_t> data; 
    bool exclusive;

public:
    //Constructor
    BufferFrame(uint64_t page_number, uint64_t frame_size, vector<uint64_t> data);
    
    char* get_data();
};


class buffer_full_error
: public std::exception {
public:
    const char* what() const noexcept override {
        return "buffer is full";
    }
};


class BufferManager {
private:
    
    vector<BufferFrame> fifo_queue;
    map<uint64_t, int> fifo_map;

    size_t page_size;
    size_t page_count;

public:

    BufferManager(size_t page_size, size_t page_count);

    ~BufferManager();


    BufferFrame& fix_page(uint64_t page_id, bool exclusive);

    void unfix_page(BufferFrame& page, bool is_dirty);


    static constexpr uint16_t get_segment_id(uint64_t page_id) {
        return page_id >> 48;
    }


    static constexpr uint64_t get_segment_page_id(uint64_t page_id) {
        return page_id & ((1ull << 48) - 1);
    }

    std::vector<uint64_t> get_fifo_list() const;
};


}  