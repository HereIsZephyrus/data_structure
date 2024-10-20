//
//  linear_test.cpp
//  data_structure
//
//  Created by ChanningTong on 10/20/24.
//

#include "linear_test.hpp"
TEST(ArrayTest, GetOutOfBounds) {
    Array<int>* list;
    list = new Array<int>(static_cast<size_t>(5));
    EXPECT_THROW(list->at(static_cast<size_t>(6)), std::out_of_range);
    EXPECT_THROW(list->at(static_cast<size_t>(-1)), std::out_of_range);
    EXPECT_THROW(list->set(static_cast<size_t>(6),10), std::out_of_range);
    EXPECT_THROW(list->set(static_cast<size_t>(-1),10), std::out_of_range);
    delete list;
}
TEST_F(IntArrayTest, SetElement) {
    constexpr int defaultVal = 5;
    constexpr int changedVal = 8;
    list->fill(defaultVal);
    for (Array<int>::iterator it = list->begin(); it != list->end(); it++){
        EXPECT_EQ(*it, defaultVal);
        counter++;
    }
    EXPECT_EQ(counter, 10);
    list->set(5, changedVal);
    EXPECT_EQ(list->at(5), changedVal);
}
TEST_F(DoubleArrayTest, SetElement) {
    constexpr double defaultVal = 4.7689869;
    constexpr double changedVal = 8976.8976;
    list->fill(defaultVal);
    for (Array<double>::iterator it = list->begin(); it != list->end(); it++){
        EXPECT_DOUBLE_EQ(*it, defaultVal);
        counter++;
    }
    EXPECT_EQ(counter, 10);
    list->set(5, changedVal);
    EXPECT_EQ(list->at(5), changedVal);
}
TEST_F(CharArrayTest, SetElement) {
    constexpr char defaultVal = 'a';
    constexpr char changedVal = 'B';
    list->fill(defaultVal);
    int counter = 0;
    for (Array<char>::iterator it = list->begin(); it != list->end(); it++){
        EXPECT_EQ(*it, defaultVal);
        counter++;
    }
    EXPECT_EQ(counter, 10);
    list->set(5, changedVal);
    EXPECT_EQ(list->at(5), changedVal);
}
TEST_F(StringArrayTest, SetElement) {
    constexpr string defaultVal = "default_value";
    constexpr string changedVal = "changed_value";
    list->fill(defaultVal);
    EXPECT_STREQ((list->at(0)).c_str(), defaultVal.c_str());
    EXPECT_STREQ((list->at(1)).c_str(), defaultVal.c_str());
    EXPECT_STREQ((list->at(4)).c_str(), defaultVal.c_str());
    EXPECT_STREQ((list->at(9)).c_str(), defaultVal.c_str());
    list->set(5, changedVal);
    EXPECT_STREQ((list->at(5)).c_str(), changedVal.c_str());
}
