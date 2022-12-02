#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <cstddef>
#include <string>

namespace buzzdb{
namespace externalsort{
class File;
/// External sort function
/// Sorts 64 bit unsigned integers using external sort.
/// @param[in] input      String to path to the file that contains 64 bit unsigned integers which are
///                       stored as 8-byte little-endian values. This file may
///                       be in `READ` mode and should not be written to.
/// @param[in] num_values The number of integers that should be sorted from the
///                       input.
/// @param[in] output     String to path to file that should contain the sorted values in the
///                       end. This file must be in `WRITE` mode.
/// @param[in] mem_size   The maximum amount of main-memory in bytes that
///                       should be used for internal sorting.
void external_sort(char input[], size_t num_values, char output[], size_t mem_size);
// void external_sort(std::vector<File *> file_vector);

}  // namespace externalsort
}  // namespace buzzdb
