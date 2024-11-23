//
//  dictionary.cpp
//  data_structure
//
//  Created by ChanningTong on 11/23/24.
//
#include <sstream>
#include <memory>
#include <algorithm>
#include <random>
#include "../applications/component.hpp"
#include "dictionary.hpp"
using std::string;
using std::getline;
namespace dict{
void DictionaryManager::readDictList(const std::vector<DictItem>& items){
    for (std::vector<DictItem>::const_iterator item = items.begin(); item != items.end(); item++)
        dict->insert(*item);
}
void DictionaryManager::readDictfile(string filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }
    string line;
    getline(file, line);// tick off header
    while (getline(file, line)) {
        std::stringstream ss(line);
        string word, meaning;
        //get string between "
        getline(ss, word, '"'); getline(ss, word, '"') ;
        getline(ss, meaning, '"');  getline(ss, meaning, '"');
        DictItem item;
        makeItem(word,meaning,item);
        dict->insert(item);
    }
    file.close();
}
void readDictfile(string filePath,std::vector<DictItem>& items){
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }
    string line;
    Recorder& recorder = Recorder::getRecord();
    getline(file, line);// tick off header
    while (getline(file, line)) {
        std::stringstream ss(line);
        string word, meaning;
        //get string between "
        getline(ss, word, '"'); getline(ss, word, '"') ;
        getline(ss, meaning, '"');  getline(ss, meaning, '"');
        DictItem item{word,meaning};
        items.push_back(item);
        recorder.wordList.push_back(word);
    }
    file.close();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(items.begin(), items.end(), gen);
}
void DictionaryManager::makeItem(string word, string containment, DictItem &item){
    item.word = word;
    //item.meanning = icu::UnicodeString::fromUTF8("containment");
    item.meanning = containment;
}
std::string QueryDictionary::query(std::string searchWord,node *p){
    if (p == nullptr)
        return "not find";
    std::string word = p->element.word;
    if (searchWord == word)
        return p->element.meanning;
    if (searchWord < word)
        return query(searchWord, p->left);
    else
        return query(searchWord, p->right);
}
void BSTDictionary::querysubtree(std::string searchWord,std::vector<std::string>& items){
    items.clear();
    using sp = std::vector<std::string>::iterator;
    if (searchWord == "")
        return;
    for (char aux = 'a'; aux <= 'z'; aux++){
        std::string word = searchWord + aux;
    }
    Recorder& recorder = Recorder::getRecord();
    sp startInd = std::lower_bound(recorder.wordList.begin(), recorder.wordList.end(), searchWord);
    std::string biggerword = searchWord;
    for (size_t c = biggerword.size()-1; c >=0; c--)
        if (biggerword[c]!='z' && biggerword[c]!='Z'){
            ++biggerword[c];
            break;
        }
    sp endInd = std::lower_bound(recorder.wordList.begin(), recorder.wordList.end(), biggerword);
    for (sp word = startInd; word != endInd; word++){
        items.push_back(*word);
        if (items.size()>=20)
            break;
    }
}
}
