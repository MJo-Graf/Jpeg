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
    std::ifstream ifs{f,std::ios::binary};
    if(!ifs.is_open()){
        std::cerr<<"Failed to open file: "<<f<<std::endl;
	exit(EXIT_FAILURE);
    }
    for(std::size_t k=0;k<bits.size();++k){
	std::string s(2,0);
        std::stringstream ss;
        ifs.read(s.data(),s.size());
	const auto symbol = ifs.get();
	if(symbol!=',' && symbol!='\n'){
	    std::cerr<<"Invalid file format"<<std::endl;
	    exit(EXIT_FAILURE);
	}
	ss<<s;
	ss>>std::hex>>bits[k];
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
