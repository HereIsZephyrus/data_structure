//
//  dictionary.cpp
//  data_structure
//
//  Created by ChanningTong on 11/23/24.
//
#include "dictionary.hpp"
#include <sstream>
#include <memory>
using std::string;
using std::getline;
namespace dict{
void DictionaryManager:: readDictfile(string filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }
    string line;
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
void DictionaryManager::makeItem(string word, string containment, DictItem &item){
    item.word = word;
    //item.meanning = icu::UnicodeString::fromUTF8("containment");
    item.meanning = containment;
}
}
