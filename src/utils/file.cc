#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))

string compression(const string & str){
    int i = str.size();
    string letters;

    for (int j = 0; j < i; ++j){
        int count = 1;
        while (str[j] == str[j+1]){
            count++;
            j++;
        }
        letters += std::to_string(count);
        letters.push_back(str[j]);
    }
    return letters;
}

}  // namespace utils
}  // namespace buzzdb
