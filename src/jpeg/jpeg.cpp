#include"jpeg/jpeg.h"
#include <algorithm>
#include <bit>
#include <cstdint>
#include <iostream>

JpegEncoder::JpegEncoder():
       	num_bits_written_(0){
}

auto&DataUnit::operator()(std::size_t v,std::size_t h){
    return data_[v][h];
}

JpegRaw JpegEncoder::Encode(RawImage&raw){
    num_bits_written_ = 0;
    huffman_tables_.clear();


    // Create grouped image and fill meta params
    auto grouped_image = createGroupedImageFromRaw(raw);

    //TODO: fill GroupedImage from raw and implement DCT and quantization

    writeData(grouped_image);
    return jpeg_raw_;
}

void JpegEncoder::Encode(DataUnit&data_unit){
	std::array<std::size_t,du_size>arr;
	for(std::size_t i=0;i<du_hor_size;++i){
	    for(std::size_t j=0;j<du_vert_size;++j){
	        arr[ZZ[i][j]] = data_unit(i,j);
	    }
	}
	auto xhufsitbl_dc_lum = huffman_tables_[0].dc_.getXhufsi();
	auto xhufcotbl_dc_lum = huffman_tables_[0].dc_.getXhufco();
	//DC
	auto xhufco_dc_lum_ = xhufcotbl_dc_lum[arr[0]];
	auto xhufsi_dc_lum_ = xhufsitbl_dc_lum[arr[0]];
	appendBits(xhufco_dc_lum_,xhufsi_dc_lum_);
	//AC
	auto xhufsitbl_ac_lum = huffman_tables_[1].dc_.getXhufsi();
	auto xhufcotbl_ac_lum = huffman_tables_[1].dc_.getXhufco();
	char RRRR{0};
	for(auto it=std::next(arr.begin());it!=arr.end();++it){
	    auto tmp=std::find_if(it,arr.end(),[](auto n){
			    return n!=0;});
	    if(tmp==arr.end()){
		char RRRRSSSS= 0b00000000;
	        auto xhufco_ac_lum_ = xhufcotbl_dc_lum[RRRRSSSS];
	        auto xhufsi_ac_lum_ = xhufsitbl_dc_lum[RRRRSSSS];
	        appendBits(xhufco_ac_lum_,xhufsi_ac_lum_);
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
	    appendBits(xhufco_ac_lum_,xhufsi_ac_lum_);
	    RRRR=0;
	}
}

//TODO: make template a concept to allow only integers
template<typename T>
void JpegEncoder::appendBytes(T bytes){
    if constexpr(std::endian::native==std::endian::big){
        //TODO: perhaps weak_ptr
        auto  p = reinterpret_cast<unsigned char*>(&bytes);
        for(std::size_t k=0;k<sizeof(T);++k){
            jpeg_raw_.emplace_back(*(p+k));
        }
    }else{
        //TODO: perhaps weak_ptr
        auto  p = reinterpret_cast<unsigned char*>(&bytes);
        for(std::size_t k=0;k<sizeof(T);++k){
            jpeg_raw_.emplace_back(*(p+sizeof(T)-1-k));
        }
    }
    num_bits_written_+=sizeof(T)*8;
}

template<typename T>
void JpegEncoder::appendBits(const T bits,std::size_t size){
    while(jpeg_raw_.size()*8-num_bits_written_<size){
	    std::cout <<"k"<<std::endl;
        jpeg_raw_.emplace_back(0);
    }
    for(std::size_t k=1;k<=size;++k,++num_bits_written_){
        std::size_t bit_offs=num_bits_written_%8;
        std::size_t index=(num_bits_written_-bit_offs)/8;
        if((1<<(size-k))&bits){
	    jpeg_raw_[index] |= (0b10000000>>bit_offs);
	}
    }
}

void JpegEncoder::writeData(GroupedImage&gimage){
    // Write SOI
    std::uint16_t a=0xFFDC;
    appendBytes(a);
    appendBits(0b1010,5);
    appendBits(0b010,3);
    //writeHuffmanTables();
    //writeQuantizationTables();
    //writeFrameHeader();
    //writeScans();
    // Encode raw jpeg
    //EntropyEncoding(std::move(gimage));
}

void JpegEncoder::writeHuffmanTables(){
}
void JpegEncoder::writeHuffmanTable(){
}
void JpegEncoder::writeQuantizationTables(){
}
void JpegEncoder::writeQuantizationTable(){
}
void JpegEncoder::writeFrameHeader(){

}
void JpegEncoder::writeScans(){

}
void JpegEncoder::writeScanHeader(){

}


void JpegEncoder::EntropyEncoding(GroupedImage&&gimage){
    for(auto du:gimage.data_){
        Encode(du);
    }
}


void JpegEncoder::computeHuffmanTable(RawImage&raw){
	//TODO: implement, meanwhile, take the predefined values from https://www.w3.org/Graphics/JPEG/itu-t81.pdf
	BITS bits{0x0,0x1,0x5,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
        HUFFVAL huffval{0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB};
	huffman_tables_.resize(2);
	huffman_tables_[0].dc_.setBits(bits);
	huffman_tables_[0].dc_.setHuffval(huffval);

	bits=BITS{0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D};
        huffval =HUFFVAL{0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,\
                         0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,\
                         0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,\
                         0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,\
                         0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,\
                         0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,\
                         0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,\
                         0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,\
                         0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,\
                         0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,\
                         0xF9,0xFA};
	huffman_tables_[0].ac_.setBits(bits);
	huffman_tables_[0].ac_.setHuffval(huffval);
}


//void JpegEncoder::fillMetaData(JpegRaw&jpeg_image){
//    //Frame Header
//    auto& meta= jpeg_image.getMeta();
//    meta.fh.SOF = 0xFFC0;
//    meta.fh.Nf = 1; //So far, we only work with luminance
//    meta.fh.Lf = 8 + 3*meta.fh.Nf;
//    meta.fh.P = 8;
//    meta.fh.Y = 16;
//    meta.fh.X = 16;
//    meta.fh.Ci.emplace_back(static_cast<std::uint8_t>(Component::Y));
//    meta.fh.HV.emplace_back(0x44);
//    meta.fh.Tq.emplace_back(static_cast<std::uint8_t>(TableSelect::DCLUM));
//
//    //Scan header
//    meta.sh.resize(1);
//    for(auto&it:meta.sh){
//	it.SOS=0xFFDA;
//	it.Ns = 1; //So far we only work with luminance
//	it.Ls = 6+2*it.Ns;
//	it.Cs.emplace_back(meta.fh.Ci.front());
//	it.Tda.emplace_back(0b00000000);
//	it.Ss = 0; 
//	it.Se = 63;
//	it.AhAl=0b00000000;
//    }
//    // Quantization table
//    std::size_t n = 1;
//    meta.qth.DQT = 0xFFDB;
//    meta.qth.PqTq = 0x00;
//    meta.qth.Lq = 2+n* (65+(4>>(meta.qth.PqTq&0xF0))*64);
//    for(std::size_t k=0;k<du_size;++k){
//        meta.qth.Q.emplace_back(1);
//    }
//
//    // Huffman table
//    meta.hth.DHT = 0xFFC4;
//    meta.hth.TcTh = 0x00;
//    n=1;
//    meta.hth.Lh = 2 + n*(17+huffman_dc_lum_.getNumHuffvals());
//    for(auto it: huffman_dc_lum_.getBits()){
//        meta.hth.L.emplace_back(it);
//    }
//    for(auto it: huffman_dc_lum_.getHuffval()){
//        meta.hth.V.emplace_back(it);
//    }
//}
GroupedImage JpegEncoder::createGroupedImageFromRaw(RawImage&raw){
    GroupedImage gimage(12,12);

    //Frame Header
    jpeg_meta_data_.fh_.SOF = 0xFFC0;
    jpeg_meta_data_.fh_.Nf = 1;
    jpeg_meta_data_.fh_.Lf = 8 + 3 * jpeg_meta_data_.fh_.Nf; 
    jpeg_meta_data_.fh_.P = 8;
    jpeg_meta_data_.fh_.Y = 12*8;
    jpeg_meta_data_.fh_.X = 12*8;
    jpeg_meta_data_.fh_.Ci.emplace_back(static_cast<std::uint8_t>(Component::Y));
    jpeg_meta_data_.fh_.HV.emplace_back(0x44);
    jpeg_meta_data_.fh_.Tq.emplace_back(0);

    //Scan header
    jpeg_meta_data_.sh_.clear();
    ScanHeader sh;
    sh.SOS = 0xFFDA;
    sh.Ns = 1;
    sh.Ls = 6 + 2 * sh.Ns;
    sh.Cs.emplace_back(static_cast<std::uint8_t>(Component::Y));
    sh.Tda.emplace_back(0x01);
    sh.Ss =0;
    sh.Se=63;
    sh.AhAl = 0x00;
    jpeg_meta_data_.sh_.emplace_back(sh);;

    // HUffman tables
    computeHuffmanTable(raw);
    jpeg_meta_data_.hth_.clear();
    for(std::size_t k=0;k<huffman_tables_.size();++k){
        HTable htabledc;
	htabledc.DHT = 0xFFC4;
	htabledc.TcTh = (((k%2)&0x0F)<<4)|((2*k)&0x0F);
	jpeg_meta_data_.hth_.emplace_back(htabledc);

        HTable htableac;
	htableac.DHT = 0xFFC4;
	htableac.TcTh = (((k%2)&0x0F)<<4)|((2*k+1)&0x0F);
	htableac.ptr_huffman_table_ = &huffman_tables_[k];
	jpeg_meta_data_.hth_.emplace_back(htableac);
    }
    // Quantization tables
    jpeg_meta_data_.qth_.clear();
    QTableHeader qtablelum;
    qtablelum.DQT = 0xFFDB;
    qtablelum.PqTq = 0x00;
    qtablelum.Lq = 2 + 65; 
    qtablelum.ptr_quantization_table_ = &QLUM;
    return gimage;
}
