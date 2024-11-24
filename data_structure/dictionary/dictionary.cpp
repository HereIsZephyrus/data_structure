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
}
