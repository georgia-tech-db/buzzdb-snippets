
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))

void hello_world(){
  printf("Hello world");
}

/**
 * This function joins two tables based on matching their join keys
 * @param input1 table 1
 * @param input2 table 2
 * @param jk_idx_input1 index of join key of table 1
 * @param jk_idx_input2 index of join key of table 2
*/
std::vector<std::vector<int>> join(std::vector<std::vector<int>>& input1,
  std::vector<std::vector<int>>& input2, size_t jk_idx_input1, size_t jk_idx_input2) {
  // output table
  std::vector<std::vector<int>> output;

  if (input1.size() == 0 && input2.size() == 0) {
    return output;
  } else if (input1.size() == 0) {
    return input2;
  } else if (input2.size() == 0) {
    return input1;
  }

  size_t input1_row_len = input1[0].size();
  size_t input2_row_len = input2[0].size();

  for (size_t i = 0; i < input1.size(); i++) {
    for (size_t j = 0; j < input2.size(); j++) {
      // see if the join keys match
      if (input1[i][jk_idx_input1] == input2[j][jk_idx_input2]) {
        // join the tables
        std::vector<int> row;

        for (size_t ii = 0; ii < input1_row_len; ii++) {
          row.push_back(input1[i][ii]);
        }

        for (size_t jj = 0; jj < input2_row_len; jj++) {
          if (jj != jk_idx_input2) {
            row.push_back(input2[j][jj]);
          }
        }

        output.push_back(row);
        break;
      }
    }
  }

  return output;
}

}  // namespace utils
}  // namespace buzzdb
