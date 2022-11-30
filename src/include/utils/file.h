#pragma once

#include <cstdint>
#include <memory>

namespace buzzdb{
namespace utils{

struct Element {
    char c;
    int offset;
    int count;
    Element(char c, int offset, int count): c(c), offset(offset), count(count){};
    friend bool operator==(const Element& r1, const Element& r2) {
        return r1.c == r2.c && r1.offset == r2.offset && r1.count == r2.count;
    }
};

std::vector<Element> run_length_encoding(std::vector<char> data);

}  // namespace utils
}  // namespace buzzdb
