//
//  quadtree_benchmark.cpp
//  data_structure
//
//  Created by ChanningTong on 10/24/24.
//

#include <random>
#include <sstream>
#include "dict_benchmark.hpp"
#include "../applications/component.hpp"
#include "../OpenGL/window.hpp"
#include "../OpenGL/graphing.hpp"
#include "../ADT/tree.hpp"
using namespace dict;

std::mt19937 rng(12345);
void readDictfileSpecificSeed(string filePath,std::vector<DictItem>& items,std::mt19937 gen){
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
        getline(ss, word, '"'); getline(ss, word, '"') ;
        getline(ss, meaning, '"');  getline(ss, meaning, '"');
        DictItem item{word,meaning};
        items.push_back(item);
        recorder.wordList.push_back(word);
    }
    file.close();
    std::shuffle(items.begin(), items.end(), gen);
}

double generate_random() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}
std::string  generateWord(std::mt19937 rng,const std::vector<DictItem>& dictionary){
    size_t size = dictionary.size();
    std::uniform_int_distribution<size_t> distribution(0, size-1);
    size_t index = distribution(rng);
    return dictionary.at(index).word;
}

static void BM_BSTdict(benchmark::State& state) {
    BSTTestdict bstdict;
    std::vector<DictItem> rawItems;
    readDictfileSpecificSeed("/Users/channingtong/Program/data_structure/data_structure/dictionary/EnWords.csv", rawItems,rng);
    bstdict.dm.readDictList(rawItems);
    int count = 0;
    for (auto _ : state) {
        std::string word = generateWord(rng,rawItems);
        bstdict.TestQuery(word);
        benchmark::ClobberMemory();
        ++count;
    }
    state.SetComplexityN(count);
}
static void BM_AVLdict(benchmark::State& state) {
    AVLTestdict avldict;
    std::vector<DictItem> rawItems;
    readDictfileSpecificSeed("/Users/channingtong/Program/data_structure/data_structure/dictionary/EnWords.csv", rawItems,rng);
    avldict.dm.readDictList(rawItems);
    int count = 0;
    for (auto _ : state) {
        std::string word = generateWord(rng,rawItems);
        avldict.TestQuery(word);
        benchmark::ClobberMemory();
        ++count;
    }
    state.SetComplexityN(count);
}

//BENCHMARK(BM_BSTdict)->Name("二叉搜索树实现的字典")->Arg(100)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(20000)->Complexity(benchmark::oN);
//BENCHMARK(BM_AVLdict)->Name("二叉平衡树实现的字典")->Arg(100)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(20000)->Complexity(benchmark::oN);

//BENCHMARK_MAIN();
