#ifndef UTILS_H
#define UTILS_H
#include<array>
#include<vector>
#include<cstring>
using BITS = std::array<std::size_t,16>;
using HUFFVAL = std::vector<char>;

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
