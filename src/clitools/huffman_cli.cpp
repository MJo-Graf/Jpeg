#include "clitools/huffman_cli.h"
#include <cctype>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>

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
    std::ifstream ifs{f,std::ios::binary|std::ios::ate};
    if(!ifs.is_open()){
        log_exit("Failed to open file");
    }
    std::size_t file_size = ifs.tellg();
    ifs.seekg(0);
    if(file_size<bits.size()*3){
        log_exit("File length too short");
    }
    auto it=bits.begin();
    bool prev_delimiter=true;
    for(std::size_t k=0;k<file_size;++k){
        char c1 = ifs.get();
	if(std::isxdigit(c1)&&prev_delimiter){
           prev_delimiter=false;
           char c2 = ifs.get();
	    if(std::isxdigit(c2)){
		if(it==bits.end()){
                    log_exit("File contains more numbers than expected");
		}
   	        std::size_t upper{0},lower{0};
		std::stringstream ss;
		ss << c1;
		ss >> std::hex >> upper;
		ss.clear();
		ss << c2;
		ss >> std::hex >> lower;
		upper=((upper<<4)&0xF0);
		lower=lower&0x0F;
		*it++ = upper|lower;
		prev_delimiter=false;
	    }else{
                log_exit("Hex numbers are expected as two coherent digits.");
	    }
	}else if(c1==',' && !prev_delimiter ){
	    prev_delimiter=true;
	}else if(c1==',' && prev_delimiter){
            log_exit("Two consecutive commas.");
	}else if(std::isxdigit(c1)&&!prev_delimiter){
            log_exit("Delimiter missing.");
        }
    }
    return bits;
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
		for(auto it:bits){std::cout << it <<std::endl;};
	        break;
	    case 'i':
	        break;
	    case 'h':
		usage();
		return EXIT_SUCCESS;
        }
    }

}
