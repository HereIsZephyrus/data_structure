//
//  dictionary.hpp
//  data_structure
//
//  Created by ChanningTong on 11/23/24.
//

#ifndef dictionary_h
#define dictionary_h
#include <cstring>
#include <string>
//#include <unicode/unistr.h>
//#include <unicode/ustream.h>
#include "../ADT/tree.hpp"
using std::string;
namespace dict{
using namespace tcb;
struct DictItem{
    /*
    struct Content{
        string partOfSpeech;
        icu::UnicodeString meanning;
    };
 */
    string word;
    string meanning;
    //icu::UnicodeString meanning;
    //vector<Content> meanning;
    bool operator < (const DictItem& rhs) const{return word < rhs.word;}
    bool operator == (const DictItem&rhs) const{return word == rhs.word;}
    bool operator > (const DictItem&rhs) const{return word > rhs.word;}
};
using baseTree = BinarySearchTree<DictItem>;
class BSTDictionary;
class DictionaryManager{
    baseTree *dict;
    void makeItem(string word,string containment,DictItem &item);
public:
    DictionaryManager() = default;
    DictionaryManager(baseTree *dict): dict(dict){}
    void readDictfile(string filePath);
};
class BSTDictionary : public BinarySearchTree<DictItem>{
public:
    DictionaryManager dm;
    BSTDictionary():dm(this){}
    ~BSTDictionary(){}
};
}

#endif /* dictionary_h */
