#pragma once

#include <cstdint>
#include <memory>

namespace buzzdb{
namespace utils{

void externalSort( char* inputFile, char* outputFile, int diffways, int mem);

}  // namespace utils
}  // namespace buzzdb
