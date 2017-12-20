/*
 * apriori.cpp
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: feelosoff
 */
#include "apriori.h"



vector<uint8_t> Apriori::ParseLine(string line){
	vector<uint8_t> result(line.size());

	for(size_t i = 0; i < line.size(); ++i)
		result[i] = line[i] - '0';

	return result;
}


ostream& operator <<(std::ostream &os, const RulesSupList &r){
    for (auto item: r.items)
    	os << "{" << item << "} ";

    os << "-> " << r.support;
    return os;
}


Apriori::Apriori(double support, string filename):
		_titleLists(new vector <RulesSupList>),
		_seqPostingLists(new vector <PostingList>){

	_shift = 0;
	_support = support;

	ifstream fin(filename.c_str(), ios::ate);
	if(!fin){
		cout << "Error while open: " << filename << endl;
		exit(1);
	}
	///< get file len
	size_t fileLen = fin.tellg();
	fin.seekg(ios::beg);

	if(!fileLen){
		cout << "File: " << filename << " empty" << endl;
		exit(1);
	}
	///< read file
	for(string line; getline(fin, line);){
		auto transaction = ParseLine(line);
		if(!_rawTable.size()){
			_rawTable.reserve((fileLen + 1) / (line.size() + 1));
		}
		_rawTable.push_back(transaction);
	}

	_transactionCount = _rawTable.size();
	_itemsCount = _rawTable[0].size();

	_seqPostingLists->resize(_itemsCount);

	///< create reverse index item -> transactions
	for(size_t t = 0; t < _transactionCount; ++t){
		for(size_t i = 0; i < _itemsCount; ++i){
			if(_rawTable[t][i]){
				size_t listSize =  (*_seqPostingLists)[i].size();
				ExponentResize<size_t>((*_seqPostingLists)[i], listSize + 1);
				(*_seqPostingLists)[i][listSize] = t;
			}
		}
	}

	_titleLists->resize(_itemsCount);

	for(size_t i = 0; i < _itemsCount; ++i)
		(*_titleLists)[i] = RulesSupList({i}, (*_seqPostingLists)[i].size());

	TitleSort();

	for(size_t i = 0; i < _itemsCount; ++i){
		support = (double)(*_titleLists)[i].support / _transactionCount;
		if(support < _support){
			_itemsCount = i;
			break;
		}
	}

	_titleLists->resize(_itemsCount);
	_elementList.resize(_itemsCount);

	sort(_titleLists->begin(),
		 _titleLists->end(),
		 [](const RulesSupList &a, const RulesSupList &b) -> bool
	            									   {	return a.items[0] < b.items[0];	});

	for(size_t i = 0; i < _itemsCount; ++i){
		_elementList[i] = (*_titleLists)[i].items[0];
	}
}


void Apriori::TitleSort(){
	sort(_titleLists->begin(),
		 _titleLists->end(),
		 [](const RulesSupList &a, const RulesSupList &b) -> bool
	            									   {	return a.support > b.support;	});
}


RulesList Apriori::SearchingInLists(size_t elementId, size_t seqId){
	RulesList result;
	for(auto listId: (*_seqPostingLists)[seqId]){
		if(_rawTable[listId][elementId]){
			size_t pos = result.size();
			ExponentResize<size_t>(result,result.size() + 1);
			result[pos] = listId;
		}
	}
	return result;
}


void Apriori::BuildRules(){

	unique_ptr< vector <RulesSupList> > bufLists(new vector <RulesSupList>);		///< tmp seq list
	unique_ptr< vector <PostingList> > bufPostingLists(new vector <PostingList>);	///< tmp posting list

	bool flag = true;

	while(flag){
		flag = false;

		bufLists->resize(0);
		bufPostingLists->resize(0);

		for(size_t seqId = 0; seqId < _titleLists->size(); ++seqId){
			auto &titleList = (*_titleLists)[seqId].items;
			///< binsearch start pos
			auto startPos = upper_bound(_elementList.begin(), _elementList.end(), titleList[titleList.size() - 1],
					[](const size_t lhs, const size_t &rhs)
														{ return lhs < rhs; });

			for(auto iter = startPos; iter != _elementList.end(); iter++){
				if(*iter <= titleList[titleList.size() - 1])
					continue;

				///< generate and check target
				RulesList targetList = SearchingInLists(*iter, seqId);
				double support = (double)targetList.size() / _transactionCount;

				if(support < _support)
					continue;

				size_t pos = bufLists->size();
				ExponentResize<RulesSupList>(*bufLists, bufLists->size() + 1);

				///< save seq
				(*bufLists)[pos] = (*_titleLists)[seqId];
				(*bufLists)[pos].items.push_back(*iter);
				(*bufLists)[pos].support = targetList.size();

				ExponentResize<PostingList>(*bufPostingLists, bufPostingLists->size() + 1);

				(*bufPostingLists)[pos] = targetList;
				size_t resPos = _rulesStorage.size();

				ExponentResize<RulesSupList>(_rulesStorage, _rulesStorage.size() + 1);
				_rulesStorage[resPos].items = (*bufLists)[pos].items;
				_rulesStorage[resPos].support = targetList.size();

				flag = true;
			}
		}
		_titleLists.swap(bufLists);
		_seqPostingLists.swap(bufPostingLists);
	}
}


shared_ptr<vector<RulesSupList> > Apriori::CreateRulesSupLists(size_t len, Apriori::SortOrder sorting){

	std::function<bool(const RulesSupList&, const RulesSupList&)> lambda;
	shared_ptr<vector<RulesSupList> > result(new vector<RulesSupList>(2 * len));

	switch(sorting){
		case SortOrder::desc :
			lambda = [](const RulesSupList &a, const RulesSupList &b) -> bool
							   	   	   	   	   {	return a.support > b.support;	};
			break;

		case SortOrder::asc  :
			lambda = [](const RulesSupList &a, const RulesSupList &b) -> bool
							   	   	   	   	   {	return a.support < b.support;	};
			break;

		case SortOrder::none :
			copy(_rulesStorage.begin(), _rulesStorage.begin() + min(len, _rulesStorage.size()), result->begin());
			result->resize(min(len, _rulesStorage.size()));

			for (auto &&item : *result)
				item.support /= _transactionCount;

			return result;

	}

	///< fast search k min/max

	if(_rulesStorage.size() <= 2 * len){
		copy(_rulesStorage.begin(), _rulesStorage.end(), result->begin());
		sort(result->begin(), result->begin() + _rulesStorage.size(), lambda);
		result->resize(min(len, _rulesStorage.size()));

		for (auto &&item : *result)
			item.support /= _transactionCount;

		return result;
	}

	copy(_rulesStorage.begin(), _rulesStorage.begin() + 2 * len, result->begin());
	sort(result->begin(), result->begin() + 2 * len, lambda);

	for(size_t iter = 2 * len; iter < _rulesStorage.size(); iter += len){
		size_t endPos = min(iter + len, _rulesStorage.size());

		copy(_rulesStorage.begin() + iter, _rulesStorage.begin() + endPos, result->begin() + len);
		sort(result->begin(), result->begin() + 2 * len, lambda);
	}

	result->resize(len);

	for (auto &&item : *result)
		item.support /= _transactionCount;

	return result;
}

