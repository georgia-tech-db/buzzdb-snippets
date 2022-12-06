#include "utils/join.h"

namespace buzzdb {
namespace join {

    Join::Join(Table &left, Table &right, size_t left_idx, size_t right_idx) {
        std::vector<std::vector<std::string>> result;
        for (auto &left_row : left.data) {
            for (auto &right_row : right.data) {
                if (left_row[left_idx] == right_row[right_idx]) {
                    // join the two rows but only include the join column once
                    std::vector<std::string> row;
                    row.insert(row.end(), left_row.begin(), left_row.end());
                    for (size_t i = 0; i < right_row.size(); i++) {
                        if (i != right_idx) {
                            row.push_back(right_row[i]);
                        }
                    }
                    result.push_back(row);
                }
            }
        }
        result_table = Table(result);
    }
}
}
