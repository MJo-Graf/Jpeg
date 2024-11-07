#ifndef JPEG_H
#define JPEG_H
#include"utils/utils.h"
#include"entropyencoding/huffman.h"


// Data unit vertical and horizontal size
constexpr unsigned_type du_vert_size{8};
constexpr unsigned_type du_hor_size{8};
constexpr unsigned_type du_size{du_vert_size*du_hor_size};

constexpr std::size_t ZZ[du_vert_size][du_hor_size]={
	{ 0, 1, 5, 6,14,15,27,28},\
	{ 2, 4, 7,13,16,26,29,42},\
	{ 3, 8,12,17,25,30,41,43},\
	{ 9,11,18,24,31,40,44,53},\
	{10,19,23,32,39,45,52,54},\
	{20,22,33,38,46,51,55,60},\
	{21,34,37,47,50,56,59,61},\
	{35,36,48,49,57,58,62,63}
};



struct DataUnit{
    std::array<signed_type,du_vert_size*du_hor_size>data;
    signed_type operator()(signed_type v,signed_type h){
        return data[v*du_hor_size+h];
    }
    signed_type getDc(){
        return data[0];
    }
};

struct RawImage{
    
    std::size_t num_channels{3};
    std::size_t vert_size{1024};
    std::size_t hor_size{1024};
    std::size_t channel_size{vert_size*hor_size};
    std::vector<std::uint16_t>payload;
};

struct GroupedImage{
    public:
    GroupedImage(const std::size_t num_v_blocks,
		 const std::size_t num_h_blocks):
	    num_v_blocks{0}
	   ,num_h_blocks{0}
	   ,data_(num_h_blocks*num_v_blocks)
    {}

    DataUnit& operator()(std::size_t v,std::size_t h){
        return data_[v*num_h_blocks+h];
    }
    std::size_t num_v_blocks{0};
    std::size_t num_h_blocks{0};
    std::vector<DataUnit>data_;
};

struct JpegImage{
    public:
    std::vector<char>data_;
    std::size_t num_bits;
};

class JpegEncoder{
    public:
    JpegImage Encode(RawImage&raw){
	computeDcLumBits(raw);
	computeDcLumHuffvals(raw);
	GroupedImage gimage(12,12);
	auto jpeg_image = EntropyEncoding(std::move(gimage));
	return jpeg_image;
    }

    void Encode(DataUnit&data_unit,JpegImage& jpeg_image){
	std::array<std::size_t,du_size>arr;
	for(std::size_t i=0;i<du_hor_size;++i){
	    for(std::size_t j=0;j<du_vert_size;++j){
	        arr[ZZ[i][j]] = data_unit(i,j);
	    }
	}
	auto xhufsitbl_dc_lum = huffman_dc_lum_.getXhufsi();
	auto xhufcotbl_dc_lum = huffman_dc_lum_.getXhufco();
	//DC
	auto xhufco_dc_lum_ = xhufcotbl_dc_lum[arr[0]];
	auto xhufsi_dc_lum_ = xhufsitbl_dc_lum[arr[0]];
	appendBits(jpeg_image,xhufco_dc_lum_,xhufsi_dc_lum_);
	//AC
	auto xhufsitbl_ac_lum = huffman_ac_lum_.getXhufsi();
	auto xhufcotbl_ac_lum = huffman_ac_lum_.getXhufco();
	char RRRR{0};
	for(auto it=std::next(arr.begin());it!=arr.end();++it){
	    auto tmp=std::find_if(it,arr.end(),[](auto n){
			    return n!=0;});
	    if(tmp==arr.end()){
		char RRRRSSSS= 0b00000000;
	        auto xhufco_ac_lum_ = xhufcotbl_dc_lum[RRRRSSSS];
	        auto xhufsi_ac_lum_ = xhufsitbl_dc_lum[RRRRSSSS];
	        appendBits(jpeg_image,xhufco_ac_lum_,xhufsi_ac_lum_);
	    }
	    if((*it)==0){
		++RRRR;
	        if((RRRR)!=0x0F){
		    continue;
		}
	    }
	    const auto SSSS= getAcCoeffMagnCat(*it);
	    char RRRRSSSS = (RRRR<<4)|SSSS;
	    auto xhufco_ac_lum_ = xhufcotbl_dc_lum[RRRRSSSS];
	    auto xhufsi_ac_lum_ = xhufsitbl_dc_lum[RRRRSSSS];
	    appendBits(jpeg_image,xhufco_ac_lum_,xhufsi_ac_lum_);
	    RRRR=0;
	}
    }

    void appendBits(JpegImage&jpeg_image,HUFFCODE::value_type hufco,HUFFSIZE::value_type hufsi){
        while(jpeg_image.data_.size()*8-jpeg_image.num_bits<hufsi){
	    jpeg_image.data_.emplace_back(0);
	}
	for(std::size_t k=0;k<hufsi;++k,++jpeg_image.num_bits){
            std::size_t bit_offs=jpeg_image.num_bits%8;
	    std::size_t index=(jpeg_image.num_bits-bit_offs)/8;
	    std::bitset<8>b{static_cast<unsigned long long>(jpeg_image.data_[index])};
	    if((1<<(hufsi-k))&hufco){
	        b.set(bit_offs);
	    }
	}
    }

    JpegImage EntropyEncoding(GroupedImage&&gimage){
	JpegImage jpeg_image;
	for(auto du:gimage.data_){
	    Encode(du,jpeg_image);
        }
	return jpeg_image;
    }
    void computeDcLumBits(RawImage&raw){
	BITS bits{00,01,05,01,01,01,01,01,01,00,00,00,00,00,00,00};
        huffman_dc_lum_.setBits(bits);
    }
    void computeDcLumHuffvals(RawImage&raw){
        HUFFVAL huffval{00,01,02,03,04,05,06,07,0x8,0x9,0xA,0xB};
	huffman_dc_lum_.setHuffval(huffval);
    }
    HuffmanDc&getHuffmanDcLum(){
	    return huffman_dc_lum_;
    }
    HuffmanDc&getHuffmanDcChr(){
	    return huffman_dc_chr_;
    }
    HuffmanAc&getHuffmanAcLum(){
	    return huffman_ac_lum_;
    }
    HuffmanAc&getHuffmanAcChr(){
	    return huffman_ac_chr_;
    }
    private:
    HuffmanDc huffman_dc_lum_;
    HuffmanDc huffman_dc_chr_;
    HuffmanAc huffman_ac_lum_;
    HuffmanAc huffman_ac_chr_;
};

#endif
