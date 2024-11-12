#include "clitools/huffman_cli.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include"utils/logger.h"

void usage(){
}

/**
 * @brief This function reads a file and stores its values
 * in the BITS array. 
 * @param[in] f String indicating the file to be read.
 * @return BITS array.
 */
BITS createBits(std::string f){
    BITS bits;
    auto log_exit = [&f](std::string err){
		std::cerr<<"Error parsing "<<f<<": "<<err<<std::endl; 
		exit(EXIT_FAILURE);
	    };
	    std::ifstream ifs{f,std::ios::binary};
	    if(!ifs.is_open()){
		log_exit("Failed to open file");
	    }
	    auto it = bits.begin();
	    while(!ifs.eof()){
		if(it==bits.end()){
		    log_exit("File contains more than 16 bytes.");
		}
		std::string s;
		std::getline(ifs,s,',');
		std::stringstream ss;
		ss << s; 
		std::size_t buff{0};
		ss >>std::hex>>buff;
		*it++ = buff;
	    }

	    return bits;
}

/**
 * @brief This function reads a file and stores its values
 * in the HUFFVAL vector. 
 * @param[in] f String indicating the file to be read.
 * @return HUFFVAL vector.
 */
HUFFVAL createHuffval(std::string f){
    HUFFVAL huffval;
    auto log_exit = [&f](std::string err){
	std::cerr<<"Error parsing "<<f<<": "<<err<<std::endl; 
	exit(EXIT_FAILURE);
    };
    std::ifstream ifs{f,std::ios::binary};
    if(!ifs.is_open()){
	log_exit("Failed to open file");
    }
    auto it = huffval.begin();
    while(!ifs.eof()){
	std::string line;
	std::getline(ifs,line);
	std::stringstream lss;
	lss<<line;
	while(!lss.eof()&&!line.empty()){
	    std::string s;
	    std::getline(lss,s,',');
	    std::stringstream ss;
	    ss<<s;
	    std::size_t buff{0};
	    ss >> std::hex >> buff;
	    huffval.emplace_back(buff);	
	}
    }

    return huffval;
}



int main(int argc,char*argv[]){
    //BITS bits; 
    //HUFFVAL huffval(0);
    //bool bits_file_provided;
    //bool huffval_file_provided;
    int c{0};
    while((c=getopt(argc,argv,"hb:i:"))!=-1){
	switch(c){
	    case 'b':
		//bits = createBits(optarg);
		break;
	    case 'i':
		//huffval = createHuffval(optarg);
		break;
	    case 'h':
		usage();
		return EXIT_SUCCESS;
	}
    }

    RawImage raw;
    JpegEncoder jpeg_enc;
    auto jpeg_image = jpeg_enc.Encode(raw);
    Logger logger;
    logger.logRawVector(jpeg_image);


	//logger.logHuffmanTable(jpeg_enc.getHuffmanAcLum().getEhufsi(),jpeg_enc.getHuffmanAcLum().getEhufco(),jpeg_enc.getHuffmanAcLum().getHuffval());

//    logger.logHuffmanTable(jpeg_enc.getHuffmanAcLum().getEhufsi(),jpeg_enc.getHuffmanAcLum().getEhufco(),jpeg_enc.getHuffmanAcLum().getHuffval());

    //HuffmanDc hdc;
    //BITS bits1{0x0,0x1,0x5,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    //HUFFVAL huffval1{0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB};
    //hdc.setBits(bits1);
    //hdc.setHuffval(huffval1);
    //hdc.computeHuffsize();
    //hdc.computeHuffcode();
    //hdc.reorder();
    //hdc.extendTable();
    //auto hv=hdc.getHuffval();
    //logger.logHuffmanTable(hdc.getEhufsi(),hdc.getEhufco(),hdc.getHuffval());


    //logger.logHuffmanTable(jpeg_enc.getHuffmanDcLum().getEhufsi(),jpeg_enc.getHuffmanDcLum().getEhufco(),jpeg_enc.getHuffmanDcLum().getHuffval());

    std::ofstream os{"my.jpeg"};
    os.write((char*)jpeg_image.data(),jpeg_image.size());
}
