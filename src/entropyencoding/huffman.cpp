#include"entropyencoding/huffman.h"


const signed_type getDcDiffMagnCat(const signed_type diff){
    signed_type result{0};
    if(diff==0){
	result=0;
    }else if(-1<= diff && diff <= 1){
	result=1;
    }else if((-3<=diff&&diff<=-2)||(2<=diff&&diff<=3)){
	result=2;
    }else if((-7<=diff&&diff<=-4)||(4<=diff&&diff<=7)){
	result=3;
    }else if((-15<=diff&&diff<=-8)||(8<=diff&&diff<=15)){
	result=4;
    }else if((-31<=diff&&diff<=-16)||(16<=diff&&diff<=31)){
	result=5;
    }else if((-63<=diff&&diff<=-32)||(32<=diff&&diff<=63)){
	result=6;
    }else if((-127<=diff&&diff<=-64)||(64<=diff&&diff<=127)){
	result=7;
    }else if((-255<=diff&&diff<=-128)||(128<=diff&&diff<=255)){
	result=8;
    }else if((-511<=diff&&diff<=-256)||(256<=diff&&diff<=511)){
	result=9;
    }else if((-1023<=diff&&diff<=-512)||(512<=diff&&diff<=1023)){
	result=10;
    }else if((-2047<=diff&&diff<=-1024)||(1024<=diff&&diff<=2047)){
	result=11;
    }else{
	result=-1;
    }
    return result;
}


const signed_type getAcCoeffMagnCat(const signed_type coeff){
    signed_type result{0};
    if(-1<= coeff && coeff <= 1){
	result=1;
    }else if((-3<=coeff&&coeff<=-2)||(2<=coeff&&coeff<=3)){
	result=2;
    }else if((-7<=coeff&&coeff<=-4)||(4<=coeff&&coeff<=7)){
	result=3;
    }else if((-15<=coeff&&coeff<=-8)||(8<=coeff&&coeff<=15)){
	result=4;
    }else if((-31<=coeff&&coeff<=-16)||(16<=coeff&&coeff<=31)){
	result=5;
    }else if((-63<=coeff&&coeff<=-32)||(32<=coeff&&coeff<=63)){
	result=6;
    }else if((-127<=coeff&&coeff<=-64)||(64<=coeff&&coeff<=127)){
	result=7;
    }else if((-255<=coeff&&coeff<=-128)||(128<=coeff&&coeff<=255)){
	result=8;
    }else if((-511<=coeff&&coeff<=-256)||(256<=coeff&&coeff<=511)){
	result=9;
    }else if((-1023<=coeff&&coeff<=-512)||(512<=coeff&&coeff<=1023)){
	result=10;
    }else{
	result=-1;
    }
    return result;
}

/**
 * brief Verify bits_ and huffval_ are consistently set.
 * @return flag to indicate whether bits_ and huffval_ were set consistenly.
 */
template<typename T>
bool HuffmanBase<T>::validBitsAndHuffval(){
	std::size_t sum=std::accumulate(bits_.begin(),bits_.end(),0);
	return sum == huffval_.size();
}
/**
 * @brief Compute Huffsize. Implementation according to
 * https://www.w3.org/Graphics/JPEG/itu-t81.pdf (Section C.2).
 * @return HUFFSIZE vector.
 */
template<typename T>
void HuffmanBase<T>::computeHuffsize(){
    huffsize_.clear();
    std::size_t i{1},j{1};
    do{
        if(j>bits_[i-1]){
            ++i; j=1;
	}else{
	    huffsize_.emplace_back(i);
	    ++j;
	    continue;
	}
    }while(i<=bits_.size());
    huffsize_.emplace_back(0);
}
template<typename T>
void HuffmanBase<T>::computeHuffcode(){
    huffcode_.clear();    
    std::size_t code{0}, si{huffsize_.front()};

    while(true){
        huffcode_.emplace_back(code++);
	if(huffsize_[huffcode_.size()]==si){
	    continue;
	}
	if(huffsize_[huffcode_.size()]==0){
	    break;
	}
	do{
	    code = code<<1;
	    ++si;
	}
	while(!(huffsize_[huffcode_.size()]==si));
    }

}

template<typename T>
void HuffmanBase<T>::reorder(){
    for(std::size_t k=0;k<huffval_.size();++k){
    std::size_t i=huffval_[k];
        ehufco_.emplace(std::make_pair(i,huffcode_[k]));
        ehufsi_.emplace(std::make_pair(i,huffsize_[k]));
    }
}

template<typename T>
std::size_t HuffmanBase<T>::getNumHuffvals(){
    return huffval_.size();
}

template<typename T>
void HuffmanBase<T>::createCodeTableInterface(){
    static_cast<T*>(this)->createCodeTable();
}


void HuffmanDc::extendTable(){
    for(std::size_t k=0;k<huffval_.size();++k){
        std::size_t diff=huffval_[k];
	const auto SSSS = getDcDiffMagnCat(diff);
        auto xcode = (ehufco_[SSSS]<<SSSS)|(diff>=0?diff:(diff-1));
	auto xsize = ehufsi_[SSSS] + SSSS;
        xhufco_.emplace(std::make_pair(diff,xcode));
        xhufsi_.emplace(std::make_pair(diff,xsize));
    }
}
void HuffmanDc::createCodeTable(){
    computeHuffsize();
    computeHuffcode();
    reorder();
    extendTable();
}
void HuffmanAc::createCodeTable(){
    computeHuffsize();
    computeHuffcode();
    reorder();
}


template class HuffmanBase<HuffmanDc>;
template class HuffmanBase<HuffmanAc>;
