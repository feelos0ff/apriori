/*
 * apriori.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: feelosoff
 */

#ifndef APRIORI_H_
#define APRIORI_H_


#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <unordered_map>
//#define PERFORMANCE_MODE

using namespace std;


typedef vector<size_t> RulesList;
typedef vector<size_t> PostingList;



struct RulesSupList{
	RulesList items;
	double support;

	RulesSupList(){}
	RulesSupList(RulesList rules, size_t s):
					items(rules), support(s){}

};

ostream& operator <<(std::ostream &os, const RulesSupList &r);



template<class T>		///< fast resize with exp algorithm
void ExponentResize(vector<T> &data, size_t newLen){
	if(data.size() <= newLen){
		if(data.capacity()<= newLen)
			data.reserve(newLen << 1);
	}
	data.resize(newLen);
}


class Apriori{

	double _support;
	size_t _shift;

	size_t _itemsCount;
	size_t _transactionCount;

	vector< vector<uint8_t> > _rawTable;	///< RAW data

	vector <size_t> _elementList;			///< element freq first - item, second freq

	unique_ptr< vector <PostingList> > _seqPostingLists;		///< seq reverse index
	unique_ptr< vector <RulesSupList> > _titleLists;		///< k-1 seq

	vector <RulesSupList> _rulesStorage;	///< store all supported rules

	void TitleSort();

	vector<uint8_t> ParseLine(string line);	///< convert from string to vector
	RulesList SearchingInLists(size_t elementId, size_t seqId);

 public:

	Apriori(double support, string filename);
	~Apriori(){}

	enum SortOrder {none, asc, desc};

	shared_ptr< vector<RulesSupList> > CreateRulesSupLists(size_t len, SortOrder sorting);
	void BuildRules();
};


#endif /* APRIORI_H_ */
