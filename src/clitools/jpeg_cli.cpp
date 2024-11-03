#include "clitools/jpeg_cli.h"

std::string exec_name;

void usage(){
    std::cout <<"Usage: "<<exec_name<<" [OPTIONS] FILE"               <<std::endl;
    std::cout <<"Information: This program implements the single" <<std::endl;
    std::cout <<"processing steps of jpeg compression. The input" <<std::endl;
    std::cout <<"shall be an RGB Image. The processing steps are "<<std::endl;
    std::cout <<"as follows: " 				          <<std::endl;
    std::cout <<"1) Conversion from RGB to YUV "                  <<std::endl;
}

int main(int argc,char *argv[]){
    exec_name = basename(argv[0]);
    int c{0};
    int count{0};
    setenv("POSIXLY_CORRECT","1",1);
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
	++count;
    }
    if(count >= (argc-1)){
        usage();
	return EXIT_FAILURE;
    }
    std::string image_fname = argv[argc-1];

}
