#ifndef LOGGER_H
#define LOGGER_H
#include"utils/utils.h"
#include <cstdint>
#include<iostream>
#include<iomanip>
#include <type_traits>
#include<concepts>



template<Numeric T>
void printBits(T bits,std::size_t num_bits){
    for(std::int16_t k=num_bits-1;k>=0;--k){
        if((1<<k)&bits){
	    std::cout<<"1";
	}else{
	    std::cout<<"0";
	}		
    }
}

class Logger{
    public:
    template<typename...Args>
    void logInfo(Args&&...args){
	std::cout<<std::left;
	std::cout<<"[ INFO  ] ";
        log(args...);
    }
    template<typename...Args>
    void logError(Args&&...args){
	std::cout<<std::left;
	std::cout<<"[ ERROR ] ";
        log(args...);
    }
    template<typename...Args>
    void logDebug(Args&&...args){
	std::cout<<std::left;
	std::cout<<"[ DEBUG ] ";
        log(args...);
    }

    void logHuffmanTable(EHUFSI&&ehufsi,EHUFCO&&ehufco,HUFFVAL&&huffval){
    //for(auto it : huffval){
    //    std::cout <<"huffval="<< static_cast<int>(it)<<std::endl;
    //}
	auto adapt_stringsize=[](std::string&s){
	    if((s.size()%2)!=0){
	        s.append(" ");
	    }
	};
	std::array<std::string,3>h{"Symbol (hex)","Code length","Code word"};
	for(auto&it:h){
	    adapt_stringsize(it);
	}

        constexpr std::size_t cw{20};
	auto setstringcol =[&cw](auto&input){
		const auto padding = (cw-input.size())/2;
		std::cout<<std::setw(padding)<<"";
		std::cout<<input;
		std::cout<<std::setw(padding)<<"";
		std::cout<<std::right<<"|";
	};
	auto setleftstringcol =[&setstringcol](auto&&input){
		std::cout<<"|";
		setstringcol(input);
	};
	auto setrightstringcol =[&setstringcol](auto&&input){
		setstringcol(input);
		std::cout<<std::endl;
	};
	auto setunderline=[&cw](auto&&num_cols){
	    std::cout<<std::setfill('-')<<std::setw(cw*3+num_cols+1)<<""<<std::endl;
	    std::cout<<std::setfill(' ');
	};

	auto setnumcol =[&cw](auto&&input){
		const auto padding = (cw-2)/2;
		std::cout<<std::setw(padding)<<"";
		std::cout<<std::setw(2)<<static_cast<int>(input);
		std::cout<<std::setw(padding)<<"";
		std::cout<<std::right<<"|";
	};
	auto setleftnumcol =[](auto&&input){
		std::cout<<"|";
		const auto padding = (cw-2)/2;
		std::cout<<std::setw(padding)<<"";
		std::cout<<std::hex<<std::setfill('0');
		std::cout<<std::setw(2)<<static_cast<int>(input);
		std::cout<<std::dec<<std::setfill(' ');
		std::cout<<std::setw(padding)<<"";
		std::cout<<std::right<<"|";
	};
	auto setrightnumcol =[&setnumcol](uint16_t input,std::uint8_t num_bits){
		printBits(input,num_bits);
		std::cout<<std::endl;
	};
	setleftstringcol(h.front());
	for(auto it=std::next(h.begin());it!=std::prev(h.end());++it){
	    setstringcol(*it);
	}
	setrightstringcol(h.back());
	setunderline(h.size());
	for(std::size_t k=0;k<ehufsi.size();++k){
	    setleftnumcol(huffval[k]);
	    setnumcol(ehufsi[huffval[k]]);
	    setrightnumcol(ehufco[huffval[k]],ehufsi[huffval[k]]);
        }

    }
    void logRawVector(const std::vector<std::uint8_t> vec){
        std::cout<<std::setfill('0');
         for(const auto it:vec){
                 std::cout<<std::hex;
		 std::cout<<std::setw(2)<<static_cast<std::uint16_t>(it);
		 std::cout<<" ";
         }
         std::cout<<std::setfill(' ');
         std::cout<<std::endl;
    }
    
    private:
    template<typename T,typename...Args>
    void log(T&&arg,Args&&...args){
        std::cout << arg;
	log(args...);
    }
    template<typename T>
    void log(T&&arg){
        std::cout<<arg<<std::endl;
    }
};
#endif
