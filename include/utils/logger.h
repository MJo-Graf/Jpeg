#ifndef LOGGER_H
#define LOGGER_H
#include"utils/utils.h"
#include<iostream>
#include<iomanip>

constexpr std::size_t INTROWIDTH{10};

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

    void logHuffmanTable(EHUFSI&&ehufsi,EHUFCO&&ehufco){
	auto adapt_stringsize=[](std::string&s){
	    if((s.size()%2)!=0){
	        s.append(" ");
	    }
	};
	std::array<std::string,3>h{"Category","Code length","Code word"};
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
		std::cout<<std::setw(2)<<input;
		std::cout<<std::setw(padding)<<"";
		std::cout<<std::right<<"|";
	};
	auto setleftnumcol =[&setnumcol](auto&&input){
		std::cout<<"|";
		setnumcol(input);
	};
	auto setrightnumcol =[&setnumcol](uint16_t input,std::uint8_t num_bits){
		for(std::int16_t k=num_bits-1;k>=0;--k){
		    if((1<<k)&input){
		            std::cout<<"1"; 
		    }else{
		            std::cout<<"0"; 
		    }
		}
		std::cout<<std::endl;
	};
	setleftstringcol(h.front());
	for(auto it=std::next(h.begin());it!=std::prev(h.end());++it){
	    setstringcol(*it);
	}
	setrightstringcol(h.back());
	setunderline(h.size());
	for(std::size_t k=0;k<ehufsi.size();++k){
	    setleftnumcol(1);
	    setnumcol(++k);
	    setrightnumcol(1,k);
        }

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
