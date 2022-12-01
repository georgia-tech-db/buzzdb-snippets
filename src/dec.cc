#include "include/dec.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>

#define UNUSED(p) ((void)(p))

namespace buzzdb
{
    namespace dec
    {

        CompressedEntry::CompressedEntry(uint32_t val) : value{val}
        {
        }

        uint32_t CompressedEntry::get_value()
        {
            return value;
        }

        uint64_t CompressedEntry::get_hash() const
        {
            return std::hash<uint32_t>{}(value);
        }

        bool CompressedEntry::operator==(const CompressedEntry other) const
        {
            return value == other.value;
        }

    } // namespace dec
} // namespace buzzdb
