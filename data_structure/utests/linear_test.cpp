//
//  linear_test.cpp
//  data_structure
//
//  Created by ChanningTong on 10/20/24.
//

#include "linear_test.hpp"
TEST(ArrayTest, GetOutOfBounds) {
    Array<int> list(5);
    EXPECT_THROW(list.at(static_cast<size_t>(6)), std::out_of_range);
    EXPECT_THROW(list.at(static_cast<size_t>(-1)), std::out_of_range);
    EXPECT_THROW(list.set(static_cast<size_t>(6),10), std::out_of_range);
    EXPECT_THROW(list.set(static_cast<size_t>(-1),10), std::out_of_range);
}
TEST(ArrayTest, Equals) {
    Array<int> raw(5),changable(5,3),tomove(5,9),unchangable(6);
    EXPECT_EQ(raw[0], 0);
    raw = changable;
    EXPECT_EQ(raw[0], 3);
    Array<int> tocopy(raw);
    EXPECT_EQ(tocopy.getSize(), 5);
    EXPECT_EQ(tocopy[0], 3);
    raw = std::move(tomove);
    EXPECT_EQ(raw[0], 9);
    EXPECT_THROW(raw = unchangable, std::length_error);
}
TEST_F(IntArrayTest, SetElement) {
    constexpr int defaultVal = 5;
    constexpr int changedVal = 8;
    list.fill(defaultVal);
    for (Array<int>::iterator it = list.begin(); it != list.end(); it++){
        EXPECT_EQ(*it, defaultVal);
        counter++;
    }
    EXPECT_EQ(counter, 10);
    list.set(5, changedVal);
    EXPECT_EQ(list.at(5), changedVal);
    EXPECT_EQ(list[5], changedVal);
}
TEST_F(DoubleArrayTest, SetElement) {
    constexpr double defaultVal = 4.7689869;
    constexpr double changedVal = 8976.8976;
    list.fill(defaultVal);
    for (Array<double>::iterator it = list.begin(); it != list.end(); it++){
        EXPECT_DOUBLE_EQ(*it, defaultVal);
        counter++;
    }
    EXPECT_EQ(counter, 10);
    list.set(5, changedVal);
    EXPECT_EQ(list.at(5), changedVal);
    EXPECT_EQ(list[5], changedVal);
}
TEST_F(CharArrayTest, SetElement) {
    constexpr char defaultVal = 'a';
    constexpr char changedVal = 'B';
    list.fill(defaultVal);
    for (Array<char>::iterator it = list.begin(); it != list.end(); it++){
        EXPECT_EQ(*it, defaultVal);
        counter++;
    }
    EXPECT_EQ(counter, 10);
    list.set(5, changedVal);
    EXPECT_EQ(list.at(5), changedVal);
    EXPECT_EQ(list[5], changedVal);
}
TEST_F(StringArrayTest, SetElement) {
    constexpr string defaultVal = "default_value";
    constexpr string changedVal = "changed_value";
    list.fill(defaultVal);
    for (Array<string>::iterator it = list.begin(); it != list.end(); it++){
        EXPECT_STREQ((*it).c_str(), defaultVal.c_str());
        counter++;
    }
    list.set(5, changedVal);
    EXPECT_STREQ((list.at(5)).c_str(), changedVal.c_str());
    EXPECT_EQ(list[5], changedVal);
}
