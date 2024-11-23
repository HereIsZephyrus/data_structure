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
#include <vector>
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
    void readDictList(const std::vector<DictItem>& items);
};
class QueryDictionary{
    using node = BinaryTreeNode<DictItem>;
public:
    std::string query(std::string searchWord,node *p);
};
class BSTDictionary : public BinarySearchTree<DictItem>, public QueryDictionary{
public:
    DictionaryManager dm;
    BSTDictionary():dm(this){}
    ~BSTDictionary(){}
    std::string query(std::string searchWord){return QueryDictionary::query(searchWord,root);}
    void querysubtree(std::string searchWord,std::vector<std::string>& items);
};
void readDictfile(string filePath,std::vector<DictItem>& items);
}

#endif /* dictionary_h */
