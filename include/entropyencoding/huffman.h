#ifndef HUFFMAN_H
#define HUFFMAN_H
#include"utils/utils.h"

class Huffman {
    public:
    
    void setBits(BITS bits){
            bits_=bits;
    }

    void setHuffval(HUFFVAL huffval){
        huffval_ = huffval;
    }

    HUFFSIZE getHuffsize() const{
        return huffsize_;
    }
    HUFFCODE getHuffcode() const{
        return huffcode_;
    }
    EHUFSI getEhufsi() const{
        return ehufsi_;
    }
    EHUFCO getEhufco() const{
        return ehufco_;
    }

    bool validBitsAndHuffval();

    void computeHuffsize();
    void computeHuffcode();

    void reorder();

    private:

    
    BITS bits_;
    HUFFVAL huffval_;

    HUFFSIZE huffsize_;
    HUFFCODE huffcode_;

    EHUFCO ehufco_;
    EHUFSI ehufsi_;
};
#endif
