#ifndef UTILS_H
#define UTILS_H
#include<array>
#include <cstdint>
#include <type_traits>
#include<vector>
#include<array>
#include <numeric>
#include<cstring>
#include<map>


// Type aliases
using BITS = std::array<std::uint16_t,16>;
using HUFFVAL = std::vector<std::uint16_t>;
using HUFFSIZE = std::vector<std::uint16_t>;
using HUFFCODE = std::vector<std::uint16_t>;
using EHUFCO = std::map<std::uint16_t,std::uint16_t>;
using EHUFSI = std::map<std::uint16_t,std::uint16_t>;

// Concepts
template<typename T>
concept Numeric = (std::is_arithmetic_v<T>);

char setBit(char val,char offs){
    return val|(1<<offs);
}
char clearBit(char val,char offs){
    return val&~(1<<offs);
}

void writeBits(void*dst,const std::size_t dst_offs,const void*src,const std::size_t num_bits){
    std::size_t num_bits_mod8=num_bits%8;
    std::size_t num_bytes = num_bits/8;
    std::memcpy(dst,src,num_bytes);

    char old = *(static_cast<char*>(dst) + num_bytes);
    char newv = *(static_cast<const char*>(src) + num_bytes);
    for(std::size_t i=0;i<num_bits_mod8;++i){
	    if((1<<num_bits_mod8)&newv){
	       setBit(old,i);
	    }
    }
    std::memcpy(static_cast<char*>(dst)+num_bytes,&old,sizeof(old));
}

#endif
