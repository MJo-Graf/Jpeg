#include "clitools/huffman_cli.h"

void usage(){
}



int main(int argc,char*argv[]){
    BITS bits; 
    HUFFVAL huffval(0);
    int c{0};
    while((c=getopt(argc,argv,"h"))!=-1){
        switch(c){
	    case 'h':
		usage();
		return EXIT_SUCCESS;
        }
    }
}
