#ifndef HUFFMAN_H
#define HUFFMAN_H
#include"utils/utils.h"

/**
 * @brief Get Diff magnitude category corresponding to DC difference value.
 * This implementation corresponds to https://www.w3.org/Graphics/JPEG/itu-t81.pdf
 * table F.1.
 * @param[in] diff Difference magnitude value.
 * return difference category.
 */
const signed_type getDcDiffMagnCat(const signed_type diff);
/**
 * @brief Get Coeffients magnitude category corresponding to AC coefficient value.
 * This implementation corresponds to https://www.w3.org/Graphics/JPEG/itu-t81.pdf
 * table F.2.
 * @param[in] coeff Coefficient magnitude value.
 * return Coefficient category.
 */
const signed_type getAcCoeffMagnCat(const signed_type coeff);


template<typename T>
class HuffmanBase {
    public:
    
    void setBits(BITS bits){
            bits_=bits;
    }

    void setHuffval(HUFFVAL huffval){
        huffval_ = huffval;
    }

    BITS getBits()const{
        return bits_;
    }
    HUFFVAL getHuffval()const{
        return huffval_;
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

    EHUFSI getXhufsi() const{
        return xhufsi_;
    }
    EHUFCO getXhufco() const{
        return xhufco_;
    }

    bool validBitsAndHuffval();

    void computeHuffsize();
    void computeHuffcode();
    void reorder();

    std::size_t getNumHuffvals();



    protected:
    void extendTableInterface();

    
    BITS bits_;
    HUFFVAL huffval_;

    HUFFSIZE huffsize_;
    HUFFCODE huffcode_;

    EHUFCO ehufco_;
    EHUFSI ehufsi_;

    XHUFCO xhufco_;
    XHUFSI xhufsi_;
};

class HuffmanDc : public HuffmanBase<HuffmanDc>{
    public:
    void extendTable();
};
class HuffmanAc : public HuffmanBase<HuffmanAc>{
    public:
    void extendTable();
};

struct HuffmanCompTable{
    HuffmanDc dc_;
    HuffmanAc ac_;
};
#endif
