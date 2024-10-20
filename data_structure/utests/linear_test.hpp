//
//  linear_test.hpp
//  data_structure
//
//  Created by ChanningTong on 10/20/24.
//

#ifndef linear_test_hpp
#define linear_test_hpp

#include <gtest/gtest.h>
#include <cstring>
#include <string>
#include "../ADT/linear.hpp"
using namespace tcb;
using std::string;
template<class Object>
class ArrayTest : public ::testing::Test {
protected:
    int counter;
    Array<Object> list;
    ArrayTest(){
        list = Array<Object>(10);
        counter = 0;
    }
    void SetUp() override {}
    void TearDown() override {}
};
using IntArrayTest = ArrayTest<int>;
using DoubleArrayTest = ArrayTest<double>;
using CharArrayTest = ArrayTest<char>;
using StringArrayTest = ArrayTest<string>;

template<class Object>
class VectorTest : public ::testing::Test {
protected:
    Vector<Object> vec;
    VectorTest(){}
    void SetUp() override {}
    void TearDown() override {}
};
using IntVectorTest = VectorTest<int>;
#endif /* linear_test_hpp */
