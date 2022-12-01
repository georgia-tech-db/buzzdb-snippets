#include "utils/join.h"
#include <utility>
#include <tuple>


namespace buzzdb {
namespace utils {

    // List of tuples with ID, Value, and Join Condition.
    Table::Table(std::vector<std::tuple<int, int, bool> > tuples) {
        this->tuples = tuples;
    }

    // Join condition based on matching id for tuples.
    bool joinCondition(Table t1, Table t2) {
        for (int i = 0; i < (int)t1.tuples.size(); i++) {
            for (int j = 0; j < (int)t2.tuples.size(); j++) {
                auto t1_tuple = std::make_tuple(t1.tuples[i]);
                auto t2_tuple = std::make_tuple(t2.tuples[j]);
                if (std::get<0>(t1_tuple) == std::get<0>(t2_tuple)) {
                    // Set join condition tuple to be true.
                    for (std::tuple<int, int, bool> &tup : t1.tuples) {
                        std::get<2>(tup) = true;
                    }
                    for (std::tuple<int, int, bool> &tup : t2.tuples) {
                        std::get<2>(tup) = true;
                    }
                }
            }
        }
        return true;
    }

    // Process of joining two tables. For cases where tuple join condition is satisfied, add to new table.
    std::vector<std::tuple<int, int, bool> > joinTables(Table t1, Table t2) {
        std::vector<std::tuple<int, int, bool> > finalTuples;
        if (joinCondition(t1, t2)) {
            for (int i = 0; i < (int)t1.tuples.size(); i++) {
                if (std::get<2>(t1.tuples[i])) {
                    finalTuples.push_back(t1.tuples[i]);
                }
            }
            for (int j = 0; j < (int)t2.tuples.size(); j++) {
                if (std::get<2>(t2.tuples[j])) {
                    finalTuples.push_back(t1.tuples[j]);
                }
            }
        }
        return finalTuples;
    }
}
}