/*
 * main.cpp
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: feelosoff
 */

#include "apriori.h"
#include <chrono>


int main(int argc, char* argv[]){

	string defaultMessage = "./apriori <file name> <support in percents> <lines count> <sort type none:asc:desc>";
	if(argc != 5){
		cout << defaultMessage << endl;
		exit(1);
	}

	size_t sz = 0;
	string filename = argv[1];

	double support = stod(argv[2],&sz);
    if(strlen(argv[2]) != sz){
    	cout << "error in support" << endl;
    	exit(1);
    }
    support /= 100;
    sz = 0;

    uint32_t linesCount = stoi(argv[3],&sz);;
    if(strlen(argv[3]) != sz){
    	cout << "error in lines count" << endl;
    	exit(1);
    }

    Apriori::SortOrder sortType;
    ///< toDo create factory method
	if(string(argv[4]) == "none")
		sortType = Apriori::SortOrder::none;
	else if(string(argv[4]) == "asc")
		sortType = Apriori::SortOrder::asc;
	else if(string(argv[4]) == "desc")
		sortType = Apriori::SortOrder::desc;
	else{
		cout << "error sort" << endl;
		exit(1);
	}

	if(!filename.length() || !support || !linesCount){
	   	cout << defaultMessage << endl;
	   	exit(1);
	}

	auto started = std::chrono::high_resolution_clock::now();
	Apriori alg( support, filename);
	auto prepared = std::chrono::high_resolution_clock::now();

	alg.BuildRules();
	auto done = std::chrono::high_resolution_clock::now();

	auto res = alg.CreateRulesSupLists(linesCount, sortType);
	auto searched = std::chrono::high_resolution_clock::now();

	for( auto item: *res)
	  	cout << item << endl;

	#ifdef PERFORMANCE_MODE
	  	std::cout << "preparing: " << std::chrono::duration_cast<std::chrono::milliseconds>(prepared-started).count()<< std::endl;
	   	std::cout << "build: " << std::chrono::duration_cast<std::chrono::milliseconds>(done-prepared).count()<< std::endl;
	   	std::cout << "search: " << std::chrono::duration_cast<std::chrono::milliseconds>(searched-done).count()<< std::endl;
	   	std::cout << "full: " << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count()<< std::endl;
	#endif

	return 0;

}
