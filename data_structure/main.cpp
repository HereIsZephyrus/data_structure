//
//  main.cpp
//  data_structure
//
//  Created by ChanningTong on 10/9/24.
//

#include <iostream>
#include <gtest/gtest.h>
#include "linear.hpp"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "-IntVectorTest*";
    return RUN_ALL_TESTS();
}
