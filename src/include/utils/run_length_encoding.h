#pragma once

struct encoding {
    std::string data;
    std::pair<int, int> i;
};

std::vector<std::pair<int, encoding>> run_length_encoder(std::vector<std::pair<int, std::string>> v);