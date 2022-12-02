#include <iostream>
#include <string>

namespace buzzdb {

#define NULL_CHAR '\0'

std::string encode_string(std::string str) {
    if (str.size() == 0) {
        return str;
    }

    std::string str_encoded;

    size_t curr_run_size = 0;
    char curr_run_char = NULL_CHAR;

    for (size_t i = 0; i < str.size(); i++) {
        if (curr_run_size == 0 || str[i] != curr_run_char) {

            // If not just starting to encode, add current run size to back of string and end current run
            if (curr_run_size != NULL_CHAR) str_encoded += std::to_string(curr_run_size);

            // Start new run and add the new run char to back of string
            curr_run_char = str[i];
            curr_run_size = 0;
            str_encoded.push_back(curr_run_char);
        }
        
        curr_run_size++;
    }

    // We have reached end of string. Add the curr run size to back and return!
    str_encoded += std::to_string(curr_run_size);

    return str_encoded;
}

} // namespace buzzdb