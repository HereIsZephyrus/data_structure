//
//  quadtree_benchmark.hpp
//  data_structure
//
//  Created by ChanningTong on 10/24/24.
//

#ifndef quadtree_benchmark_hpp
#define quadtree_benchmark_hpp

#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using para = std::pair<GLuint, unsigned long long>;
para updateSpatialByQuadTreeIndex(GLfloat gridsize);
para updateSpatialByNoIndex(GLfloat gridsize);
static void BM_QuadTreeIndex(benchmark::State& state);
static void BM_NoIndex(benchmark::State& state);
#endif /* quadtree_benchmark_hpp */
