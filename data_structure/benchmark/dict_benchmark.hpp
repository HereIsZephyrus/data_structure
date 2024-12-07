//
//  quadtree_benchmark.hpp
//  data_structure
//
//  Created by ChanningTong on 10/24/24.
//

#ifndef dicttree_benchmark_hpp
#define dicttree_benchmark_hpp
#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../dictionary/dictionary.hpp"
using namespace dict;
static void BM_BSTdict(benchmark::State& state);
static void BM_AVLdict(benchmark::State& state);
std::string  generateWord(std::mt19937 rng,const std::vector<DictItem>& dictionary);
class BSTTestdict : public BSTDictionary{
public:
    BSTTestdict() = default;
    void TestQuery(std::string word){query(word);}
};
class AVLTestdict : public AVLDictionary{
public:
    AVLTestdict() = default;
    void TestQuery(std::string word){query(word);}
};
#endif /* dicttree_benchmark_hpp */
