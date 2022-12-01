#include <string>
#include <unordered_map>

namespace {
    namespace {
        class DictEncoding {
            private:

            std::string s;

            public:

            // In this demonstration, we will mimic an ice cream shop that has 5 flavors
            // We will utilize a dictionary encoding compression scheme to shorten the order ticket
            // For example, instead of a ticket saying "Order: Vanilla, Strawberry, Autumn Spice", 
            // it will be compressed using the dict created below to "Order: 1, 3, 4"

            static unordered_map<string, string> dict({
                {"Vanilla", "1"},
                {"Chocolate", "2"},
                {"Strawberry", "3"}, 
                {"Autumn Spice", "4"},
                {"Peppermint", "5"}
            });

            DictEncoding(std::string str) {
                s = str;
            }

            std::string compress();

        }
    } // namespace utils
} // namespace buzzdb