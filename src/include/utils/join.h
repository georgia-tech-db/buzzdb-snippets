#pragma once

#include <vector>
#include <string>

namespace buzzdb {
namespace join {

    class Table {
    public:
        Table() {};
        Table(std::vector<std::vector<std::string>> d) : data(d) {};
        ~Table() {};

        std::vector<std::vector<std::string>> data;
    };

    class Join {
    public:
        Join(Table &left, Table &right, size_t left_idx, size_t right_idx);
        ~Join() {};

        Table result_table;
    };

}
}
