#ifndef UTILS_H
#define UTILS_H
#include<array>
#include<variant>
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include<vector>
#include<array>
#include<bitset>
#include <numeric>
#include<cstring>
#include<map>

using unsigned_type = std::uint8_t;
using signed_type = std::int32_t;
using quantization_type = std::uint8_t;
using code_type = std::uint16_t;
// Type aliases
using BITS = std::array<unsigned_type,16>;
using HUFFVAL = std::vector<unsigned_type>;
using HUFFSIZE = std::vector<unsigned_type>;
using HUFFCODE = std::vector<code_type>;
using EHUFCO = std::map<unsigned_type,code_type>;
using EHUFSI = std::map<unsigned_type,unsigned_type>;
using XHUFCO = std::map<unsigned_type,code_type>;
using XHUFSI = std::map<unsigned_type,unsigned_type>;

// Concepts
template<typename T>
concept Numeric = (std::is_arithmetic_v<T>);


#endif
