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

// Data unit vertical and horizontal size
constexpr std::uint8_t du_vert_size{8};
constexpr std::uint8_t du_hor_size{8};
constexpr std::uint8_t du_size{du_vert_size*du_hor_size};

// Type aliases
using code_type = std::uint16_t;
using code_size_type = std::uint8_t;
using xcode_type = std::uint32_t;
using huffval_type = std::uint8_t; // Huffvals are composed of of R/S
using BITS = std::array<code_size_type,16>;
using HUFFVAL = std::vector<huffval_type>;
using HUFFSIZE = std::vector<code_size_type>;
using HUFFCODE = std::vector<code_type>;
using EHUFCO = std::map<huffval_type,code_type>;
using EHUFSI = std::map<huffval_type,code_size_type>;
using XHUFCO = std::map<huffval_type,xcode_type>;
using XHUFSI = std::map<huffval_type,code_size_type>;



// Type for DC DIFF and AC COEFFS
using block_element_type = std::int16_t;
using block_type = block_element_type[du_vert_size][du_hor_size];

// Types for storing quantization tables
using quantization_value_type = std::uint8_t;
using quantization_table_type = quantization_value_type[du_vert_size][du_hor_size];
using JpegRaw = std::vector<std::uint8_t>;

// Data unit type
struct DataUnit{
    auto&operator()(std::size_t v,std::size_t h);
    private:
    block_type data_;
};
// Concepts
template<typename T>
concept Numeric = (std::is_arithmetic_v<T>);


#endif
