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
template<class SearchTree>
class DictionaryManager{
    SearchTree *dict;
    void makeItem(string word,string containment,DictItem &item){
        item.word = word;
        //item.meanning = icu::UnicodeString::fromUTF8("containment");
        item.meanning = containment;
    }
public:
    DictionaryManager() = default;
    DictionaryManager(SearchTree *dict): dict(dict){}
    void readDictList(const std::vector<DictItem>& items){
        for (std::vector<DictItem>::const_iterator item = items.begin(); item != items.end(); item++)
            dict->insert(*item);
    }
};
template<class TreeNode>
class QueryDictionary{
public:
    std::string query(std::string searchWord,TreeNode *p){
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
    void querysubtree(std::string searchWord,std::vector<std::string>& items){
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
};
class BSTDictionary : public BinarySearchTree<DictItem>, public QueryDictionary<BinaryTreeNode<DictItem>>{
public:
    DictionaryManager<BinarySearchTree<DictItem>> dm;
    BSTDictionary():dm(this){}
    ~BSTDictionary(){}
    std::string query(std::string searchWord){return QueryDictionary::query(searchWord,root);}
};
class AVLDictionary : public AVLSearchTree<DictItem>, public QueryDictionary<AVLTreeNode<DictItem>>{
public:
    DictionaryManager<AVLSearchTree<DictItem>> dm;
    AVLDictionary():dm(this){}
    ~AVLDictionary(){}
    std::string query(std::string searchWord){return QueryDictionary::query(searchWord,root);}
};
void readDictfile(string filePath,std::vector<DictItem>& items);
}

#endif /* dictionary_h */
