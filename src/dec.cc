#include "include/dec.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>

#define UNUSED(p) ((void)(p))

namespace buzzdb {
namespace dec {

// template <typename T> 
// bool Entry<T>::operator==(const Entry<T> other){
//     return value == other.value;
// }
bool operator==(const CompressedEntry c1, CompressedEntry c2){
    return c1.value == c2.value;
}

// template <typename E> 
// Dictionary<E>::Dictionary(std::vector<Entry<E>> entries): counter{0}{
//     for(auto e:entries){
//         if(!encode_map.contains(e.get_value())){
//             counter++;
//             encode_map.insert({e.get_value(),counter});
//             decode_map.insert({counter,e.get_value()});
//         }
//     }
// }

// template <typename E> CompressedEntry Dictionary<E>::compress(Entry<E> e){
//     return encode_map.at(e);
// }
// template <typename E> Entry<E> Dictionary<E>::decompress(CompressedEntry ce){
//     return decode_map.at(ce);
// }

}  // namespace dec
}  // namespace buzzdb
