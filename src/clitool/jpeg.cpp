#include "clitool/jpeg.h"

std::string fname;

void usage(){
    std::cout <<"Usage: "<<fname<<" [OPTIONS]" <<"FILE"           <<std::endl;
    std::cout <<"Information: This program implements the single" <<std::endl;
    std::cout <<"processing steps of jpeg compression. The input" <<std::endl;
    std::cout <<"shall be an RGB Image. The processing steps are "<<std::endl;
    std::cout <<"as follows: " 				          <<std::endl;
    std::cout <<"1) Conversion from RGB to YUV "                  <<std::endl;
}

int main(int argc,char *argv[]){
    fname = basename(argv[0]);
    int c{0};
    while((c=getopt(argc,argv,"hc"))!=-1){
        switch(c){
	    case 'c':
		break;
	    case 'h':
	        usage();
		return EXIT_SUCCESS;
	    default:
		std::cout <<"jumping into default, exiting with failure, c= "<<(char)c<<std::endl;
		return EXIT_FAILURE;
	}
    }
    for(int i=0;i<argc;++i){
        std::cout << argv[i]<<std::endl;
    }
}
