#include"jpeg/jpeg.h"
#include <algorithm>
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

//template<typename T>
//class X;
void JpegEncoder::Encode(DataUnit&data_unit){
	std::array<block_element_type,du_size>arr;
	for(std::size_t i=0;i<du_hor_size;++i){
	    for(std::size_t j=0;j<du_vert_size;++j){
	        arr[ZZ[i][j]] = data_unit(i,j);
		std::cout <<"data("<<i<<","<<j<<")="<<static_cast<int>(data_unit(i,j))<<std::endl;
	    }
	}
	for(auto it: arr){
		std::cout << it <<" ";
	}
	std::cout<<std::endl;
	//DC
	auto xhufsitbl_dc_lum = huffman_tables_[0].dc_.getXhufsi();
	auto xhufcotbl_dc_lum = huffman_tables_[0].dc_.getXhufco();
	auto xhufco_dc_lum_ = xhufcotbl_dc_lum[arr[0]];
	auto xhufsi_dc_lum_ = xhufsitbl_dc_lum[arr[0]];

//	X<decltype(xhufsi_dc_lum_)>a;
//	X<decltype(xhufsi_dc_lum_)>a;
	std::cout <<"APPENDING DC BITS. HUFCO= "<<static_cast<int>(xhufco_dc_lum_)<<" xhufsi= " <<static_cast<int>(xhufsi_dc_lum_)<<" arr[0]="<< static_cast<int>(arr[0]) <<std::endl;

	//appendBits(0,2);
	appendBits(xhufco_dc_lum_,xhufsi_dc_lum_);
	//AC
	auto ehufsi = huffman_tables_[0].ac_.getEhufsi();
	auto ehufco = huffman_tables_[0].ac_.getEhufco();
	std::size_t K{0};
	std::size_t R{0}; // run length of zeros.
			  //
	auto enc_R_ZZ_K = [this,&ehufsi,&ehufco,&R](auto ZZ_K){
            std::size_t SSSS = getAcCoeffMagnCat(ZZ_K);
            std::size_t RS =  16*R+SSSS;
	    std::cout <<" appending RRRR="<<R<<" SSSS="<<SSSS<<std::endl;
            appendBits(ehufco[RS],ehufsi[RS]);
            if(ZZ_K<0){
                --ZZ_K;
            }
            appendBits(ZZ_K,SSSS);
	};

	static int mycounter=0;
	do{
	    ++K;
	    if(arr[K]==0){
	        if(K==63){
		    appendBits(ehufco[0],ehufsi[0]);
		    std::cout <<"EOB= "<<static_cast<int>(ehufco[0])<<" NUMBITS="	<<static_cast<int>(ehufsi[0])<<std::endl;
		    break;
		}else{
		    ++R;
		    continue;
		}	
	    }else{
	        while(R>15){
			std::cout <<"appending 0xF0 at K = " <<K<<std::endl;
		    appendBits(ehufco[0xF0],ehufsi[0xF0]);
		    R-=16;
		}
		enc_R_ZZ_K(arr[K]);
		R=0;
	    }
	}while(K!=63);


	
}

void JpegEncoder::Encode(std::size_t R,std::int32_t ZZ_K,EHUFSI&ehufsi,EHUFCO&ehufco){
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
        jpeg_raw_.emplace_back(0);
    }
    for(std::size_t k=1;k<=size;++k){
        std::size_t bit_offs=num_bits_written_%8;
        std::size_t index=(num_bits_written_-bit_offs)/8;
        if((1<<(size-k))&bits){
	    jpeg_raw_[index] |= (0b10000000>>bit_offs);
	}else{
	}
	++num_bits_written_;

	// Byte stuffing
        if(num_bits_written_>=8 && (num_bits_written_%8==0)){
            if(jpeg_raw_[index] ==0xFF){
	        jpeg_raw_.emplace_back(0x00);
		num_bits_written_+=8;
	    }
        }
    }

}

void JpegEncoder::writeData(GroupedImage&gimage){
    writeStartOfImage();
    writeHuffmanTables();
    writeQuantizationTables();
    writeFrameHeader();
    writeScans(gimage);
    writeEndOfImage();

}

void JpegEncoder::writeStartOfImage(){
    jpeg_raw_.clear();
    appendBytes(static_cast<std::uint16_t>(0xFFD8));
}
void JpegEncoder::writeHuffmanTables(){
    appendBytes(jpeg_meta_data_.hth_.DHT);
    appendBytes(jpeg_meta_data_.hth_.Lh);
    for(auto&it:jpeg_meta_data_.hth_.htables_){
        appendBytes(it.dc_TcTh);
	for(auto inner_it:it.ptr_huffman_table_->dc_.getBits()){
	    appendBytes(inner_it);
	}
	for(auto inner_it:it.ptr_huffman_table_->dc_.getHuffval()){
	    appendBytes(inner_it);
	}
        appendBytes(it.ac_TcTh);
	for(auto inner_it:it.ptr_huffman_table_->ac_.getBits()){
	    appendBytes(inner_it);
	}
	for(auto inner_it:it.ptr_huffman_table_->ac_.getHuffval()){
	    appendBytes(inner_it);
	}
    }
}
void JpegEncoder::writeEndOfImage(){
    appendBytes(static_cast<std::uint16_t>(0xFFD9));
}
void JpegEncoder::writeQuantizationTables(){
    appendBytes(jpeg_meta_data_.qth_.DQT);
    appendBytes(jpeg_meta_data_.qth_.Lq);
    for(auto&it:jpeg_meta_data_.qth_.qtables_){
        appendBytes(it.PqTq);
	for(std::size_t i=0;i<du_vert_size;++i){
	    for(std::size_t j=0;j<du_hor_size;++j){
		//    std::cout <<"APPENDING BYTES "<<static_cast<int>(*it.ptr_quantization_table_[i][j])<<std::endl;
	        appendBytes((*it.ptr_quantization_table_)[i][j]);
		//    std::cout <<"APPENDING BYTES "<<static_cast<int>(QLUM[i][j])<<std::endl;
	        //appendBytes(QLUM[i][j]);
//		    std::cout <<"DONE... "<<std::endl;
	    }
	}
    }
		
}
void JpegEncoder::writeQuantizationTable(){
}
void JpegEncoder::writeFrameHeader(){
    appendBytes(jpeg_meta_data_.fh_.SOF);
    appendBytes(jpeg_meta_data_.fh_.Lf);
    appendBytes(jpeg_meta_data_.fh_.P);
    appendBytes(jpeg_meta_data_.fh_.Y);
    appendBytes(jpeg_meta_data_.fh_.X);
    appendBytes(jpeg_meta_data_.fh_.Nf);
    for(std::size_t k=0;k<jpeg_meta_data_.fh_.Nf;++k){
        appendBytes(jpeg_meta_data_.fh_.Ci[k]);
        appendBytes(jpeg_meta_data_.fh_.HV[k]);
        appendBytes(jpeg_meta_data_.fh_.Tq[k]);
    }
}
void JpegEncoder::writeScans(GroupedImage&gimage){
    appendBytes(static_cast<std::uint16_t>(0xFFDA));
    for(auto &it:jpeg_meta_data_.sh_){
        appendBytes(it.Ls);
        appendBytes(it.Ns);
	for(std::size_t k=0;k<it.Ns;++k){
	    appendBytes(it.Cs[k]);
	    appendBytes(it.Tda[k]);
	}
	appendBytes(it.Ss);
	appendBytes(it.Se);
	appendBytes(it.AhAl);
        EntropyEncoding(gimage);
    }
}
void JpegEncoder::writeScanHeader(){

}


void JpegEncoder::EntropyEncoding(GroupedImage&gimage){
    for(auto du:gimage.data_){
        Encode(du);
    }
    //padding of last byte
    std::size_t num_bits_remaining = 8-(num_bits_written_%8);
    appendBits(0b11111111,num_bits_remaining);
    //appendBits(0b00000000,num_bits_remaining);
}


void JpegEncoder::computeHuffmanTable(RawImage&raw){
	//TODO: implement, meanwhile, take the predefined values from https://www.w3.org/Graphics/JPEG/itu-t81.pdf
	BITS bits{0x0,0x1,0x5,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
        HUFFVAL huffval{0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB};
	huffman_tables_.resize(1);
	huffman_tables_[0].dc_.setBits(bits);
	huffman_tables_[0].dc_.setHuffval(huffval);
	huffman_tables_[0].dc_.createCodeTable();
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
	huffman_tables_[0].ac_.createCodeTable();
}


GroupedImage JpegEncoder::createGroupedImageFromRaw(RawImage&raw){
    const std::size_t num_hor_data_units{1};
    const std::size_t num_ver_data_units{4};
    GroupedImage gimage(num_hor_data_units,num_ver_data_units);
    gimage.data_.front()(0,0)=00;
    gimage.data_[0](7,7)=30;

    //Frame Header
    jpeg_meta_data_.fh_.SOF = 0xFFC0;
    jpeg_meta_data_.fh_.Nf = 1;
    jpeg_meta_data_.fh_.Lf = 8 + 3 * jpeg_meta_data_.fh_.Nf; 
    jpeg_meta_data_.fh_.P = 8;
    jpeg_meta_data_.fh_.Y = num_ver_data_units*8;
    jpeg_meta_data_.fh_.X = num_hor_data_units*8;
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
    std::size_t n =huffman_tables_.size()*2;
    std::size_t num_vals=0;
    for(auto it=huffman_tables_.cbegin();it!=huffman_tables_.end();++it){
	num_vals+= it->dc_.getHuffval().size();	
	num_vals+= it->ac_.getHuffval().size();	
    }
    for(std::size_t k=0;k<huffman_tables_.size();++k){
	jpeg_meta_data_.hth_.DHT = 0xFFC4;
	jpeg_meta_data_.hth_.Lh =2+n*17+num_vals;
	HTableHeader::HTable htable;
	htable.dc_TcTh = ((2*k)&0x0F);
	htable.ac_TcTh = (1<<4)|((2*k+1)&0x0F);
	htable.ptr_huffman_table_ = &huffman_tables_[k];
	jpeg_meta_data_.hth_.htables_.emplace_back(htable);
    }
    // Quantization tables
    jpeg_meta_data_.qth_.DQT = 0xFFDB;
    jpeg_meta_data_.qth_.Lq = 2*65+2;
    QTableHeader::QTable qtablelum;
    qtablelum.PqTq = 0x00;
    qtablelum.ptr_quantization_table_ = &QLUM;
    jpeg_meta_data_.qth_.qtables_.emplace_back(qtablelum);

    QTableHeader::QTable qtablechr;
    qtablechr.PqTq = 0x01;
    qtablechr.ptr_quantization_table_ = &QCHR;
    jpeg_meta_data_.qth_.qtables_.emplace_back(qtablechr);
    
    return gimage;
}
