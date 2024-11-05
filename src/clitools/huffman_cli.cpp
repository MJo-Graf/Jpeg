#include "clitools/huffman_cli.h"
#include <cctype>
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
        std::string s;
        std::getline(ifs,s,',');
        std::stringstream ss;
        ss << s; 
	std::size_t buff{0};
	ss >> std::hex >> buff;
        huffval.emplace_back(buff);	
    }

    return huffval;
}

template<typename T>
void printCodeTable(T code_sizes,T code_values){
    if((code_sizes.size())!=code_values.size()){
	 std::cerr <<"Error printing code table, size!=values"<<std::endl;
	 std::cerr<<"code_sizes: "<<code_sizes.size()<< " code_values: "<<code_values.size()<<std::endl;
         exit(EXIT_FAILURE);
    }
}


int main(int argc,char*argv[]){
    BITS bits; 
    HUFFVAL huffval(0);
    bool bits_file_provided;
    bool huffval_file_provided;
    int c{0};
    while((c=getopt(argc,argv,"hb:i:"))!=-1){
        switch(c){
	    case 'b':
		bits = createBits(optarg);
		std::cout <<"bits:"<<std::endl;
		for(auto it:bits){std::cout << +it <<std::endl;};
	        break;
	    case 'i':
		std::cout <<"huffval:"<<std::endl;
		huffval = createHuffval(optarg);
		for(auto it:huffval){std::cout << static_cast<int>(it) <<std::endl;};
	        break;
	    case 'h':
		usage();
		return EXIT_SUCCESS;
        }
    }

    Huffman huffman;
    huffman.setBits(bits);
    huffman.setHuffval(huffval);
    if(!huffman.validBitsAndHuffval()){
        std::cerr<<"Bits and Huffval are not set consistenly"<<std::endl;
	return EXIT_FAILURE;
    }
    huffman.computeHuffsize();
    huffman.computeHuffcode();
    huffman.reorder();
    for(auto&it:huffman.getHuffsize()){
	    std::cout << "huffsize: " <<it<<std::endl;
    }
    printCodeTable(huffman.getEhufsi(),huffman.getEhufco());
    Logger logger;
    logger.logInfo("hello","ok");
    logger.logError("hello","ok");
    logger.logDebug("hello","ok");
    logger.logHuffmanTable(huffman.getEhufsi(),huffman.getEhufco());

}
