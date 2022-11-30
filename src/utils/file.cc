#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "utils/file.h"

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))

std::vector<Element> run_length_encoding(std::vector<char> data) {
    std::vector<Element> encoded;
    int n = data.size();
    for (int i = 0; i < n; i++) {
        char c = data[i];
        int count = 1;
        int offset = i;
        while (i < n - 1 && data[i] == data[i + 1]) {
            i++;
            count++;
        }
        Element e = {c, offset, count};
        encoded.push_back(e);
    }
    return encoded;
}

}  // namespace utils
}  // namespace buzzdb
