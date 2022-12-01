#inlcude <string>
#include "utils/dict_encoding.h"

namespace buzzdb {
    namespace utils {

        std::string DictEncoding::compress() {
            std::string encoded = s;
            std::string flavor;

            if (s.find("Vanilla") != std::string::npos) {
                flavor = "Vanilla";
                encoded = encoded.replace(pos, flavor.length(), dict[flavor]);
            } else if (s.find("Chocolate") != std::string::npos) {
                flavor = "Chocolate";
                encoded = encoded.replace(pos, flavor.length(), dict[flavor]);
            } else if (s.find("Strawberry") != std::string::npos) {
                flavor = "Strawberry";
                encoded = encoded.replace(pos, flavor.length(), dict[flavor]);
            } else if (s.find("Autumn Spice") != std::string::npos) {
                flavor = "Chocolate";
                encoded = encoded.replace(pos, flavor.length(), dict[flavor]);
            } else if (s.find("Peppermint") != std::string::npos) {
                flavor = "Chocolate";
                encoded = encoded.replace(pos, flavor.length(), dict[flavor]);
            }
            return encoded;
        }
    }
}