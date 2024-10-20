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
#include "../ADT/myarray.hpp"
using namespace tcb;
using std::string;
template<class Object>
class ArrayTest : public ::testing::Test {
protected:
    Array<Object>* list;
    int counter;
    void SetUp() override {
        list = new Array<Object>(static_cast<size_t>(10));
        counter = 0;
    }
    void TearDown() override {delete list;}
public:
    
};
using IntArrayTest = ArrayTest<int>;
using DoubleArrayTest = ArrayTest<double>;
using CharArrayTest = ArrayTest<char>;
using StringArrayTest = ArrayTest<string>;
#endif /* linear_test_hpp */
