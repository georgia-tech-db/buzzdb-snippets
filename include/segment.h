#include <cstdint>
#include <vector>

#define MAX_EXTENT_SIZE 16
#define MAX_BLOCK_SIZE 16

namespace buzzdb{
class block
{
private:
    /* data */
    std::vector<int> data;
    uint64_t size;
public:
    block(/* args */){
        this->size = 0;
    };
    ~block();

    std::vector<int> read();

    bool write(int ch);

};


class extent
{
private:
    /* data */
    uint64_t size = 0;
    std::vector<block> blocks;

public:
    extent(/* args */){this->size = 0;};
    ~extent();

    std::vector<int> read(uint64_t block_index);

    bool write(int ch);
};


class segment
{
private:
    /* data */
    std::vector<extent> extents;
    uint64_t extent_num = 0;
    
public:
    segment(/* args */);
    ~segment();

    std::vector<int> read(uint64_t extent_index, uint64_t block_index);

    uint64_t get_extent_num();

    bool write(int ch);
};
}