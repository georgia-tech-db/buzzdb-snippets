#pragma once

#include <cstdint>
#include <memory>

namespace buzzdb{

typedef std::pair<size_t, uint64_t> frame;

std::vector<uint64_t> parse_input(std::string page_id_string);

void print_queue(std::vector<frame> queue);

std::vector<frame> fifo(std::vector<uint64_t> page_ids);

std::vector<frame> lru(std::vector<uint64_t> page_ids);

std::pair<std::vector<frame>, std::vector<frame>> two_queue(std::vector<uint64_t> page_ids);


}  // namespace buzzdb
