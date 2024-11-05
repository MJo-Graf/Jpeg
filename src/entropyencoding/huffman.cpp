#include"entropyencoding/huffman.h"

/**
 * brief Verify bits_ and huffval_ are consistently set.
 * @return flag to indicate whether bits_ and huffval_ were set consistenly.
 */
bool Huffman::validBitsAndHuffval(){
	std::size_t sum=std::accumulate(bits_.begin(),bits_.end(),0);
	return sum == huffval_.size();
}
/**
 * @brief Compute Huffsize. Implementation according to
 * https://www.w3.org/Graphics/JPEG/itu-t81.pdf (Section C.2).
 * @return HUFFSIZE vector.
 */
void Huffman::computeHuffsize(){
    huffsize_.clear();
    std::size_t i{0},j{1};
    do{
        if(j>bits_[i-1]){
            ++i; j=1;
	}else{
	    huffsize_.emplace_back(i);
	    ++j;
	    continue;
	}
    }while(i<bits_.size());
    huffsize_.emplace_back(0);
}
void Huffman::computeHuffcode(){
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

void Huffman::reorder(){
    for(std::size_t k=0;k<huffval_.size();++k){
    std::size_t i=huffval_[k];
        ehufco_.emplace(std::make_pair(i,huffcode_[k]));
        ehufsi_.emplace(std::make_pair(i,huffsize_[k]));
    }
}
