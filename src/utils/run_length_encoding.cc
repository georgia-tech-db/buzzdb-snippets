// 1> Write a toy C++ example for run length encoding compression scheme
// given a vector of pairs (index, string), return a vector with run length encoding of that (index, (string, (s_index, e_index)))
// for most efficient encoding, input vector should be sorted beforehand

#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "utils/run_length_encoding.h"

std::vector<std::pair<int, encoding>> run_length_encoder(std::vector<std::pair<int, std::string>> v) {
    std::vector<std::pair<int, encoding>> encoded;
    int i = 0;
    std::string last_str;
    int start = 0;
    int end = 0;
    int index = 0;
    for (std::pair<int, std::string> p : v) {
        if (i == 0) {
            last_str = p.second;
            i++;
            end++;
            continue;
        }
        if (0 == strcmp(p.second.c_str(), last_str.c_str())) {
            end++;
            continue;
        } else {
            encoding e;
            e.data = last_str;
            e.i = std::pair<int, int>(start, end);
            std::pair<int, encoding> np = std::pair<int, encoding>(index, e);
            index++;
            encoded.push_back(np);
            start = end;
            last_str = p.second;
        }
    }
    return encoded;
}

int main() {
    std::vector<std::pair<int, std::string>> r;
    std::pair<int, std::string> p;
    for (int i = 0; i < 50; i++) {
        if (i % 6 == 0) p = std::pair<int, std::string>(i, "M");
        else p = std::pair<int, std::string>(i, "F");
        r.push_back(p);
    }
    for (auto p : r) {
        std::cout << p.first << ": " << p.second << std::endl;
    }
    std::cout << "-----------" << std::endl;
    auto v = run_length_encoder(r);
    for (auto p : v) {
        std::cout << p.first << ": " << p.second.data << p.second.i.first << ", " << p.second.i.second << std::endl;
    }
    return 0;
}