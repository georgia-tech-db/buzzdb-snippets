#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))

std::vector<std::pair<int, int>> linearHashing(std::vector<std::pair<int, int>> hashTable, std::pair<int,int> pair) {
    int hashValue = pair.first % hashTable.size();
    int probe = hashValue;
    bool spotFound = false;
    do {
        if (hashTable[probe].first == -1) {
            spotFound = true;
            hashTable[probe] = pair;
        }
        probe = probe + 1;
    }
    while ((spotFound == false) && (hashValue != probe));
}

}  // namespace utils
}  // namespace buzzdb

// std::vector<int> linearHashing(std::vector<int> hashTable, int value) {
//     int hashValue = value % hashTable.size();
//     int probe = hashValue;
//     bool spotFound = false;
//     do {
//         if (hashTable[probe] == -1) {
//             spotFound = true;
//             hashTable[probe] = value;
//         }
//         probe = probe + 1;
//     }
//     while ((spotFound == false) && (hashValue != probe));
// }