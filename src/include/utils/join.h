#pragma once

#include <vector>

namespace buzzdb {
namespace utils {
    // Table consists of a list of tuples (ID, Value, Join Condition)
    class Table {
    public:
        std::vector<std::tuple<int, int, bool> > tuples;
    Table(std::vector<std::tuple<int, int, bool> > tuples);
    };

    bool joinCondition(Table s1, Table s2);
    std::vector<std::tuple<int, int, bool> > joinTables(Table s1, Table s2);
}
}