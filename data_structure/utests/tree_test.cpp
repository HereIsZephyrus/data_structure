//
//  tree_test.cpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#include "tree_test.hpp"
TEST_F(BinarySearchTreeTest, containsExistingValues) {
    EXPECT_TRUE(bst.contains(10));
    EXPECT_TRUE(bst.contains(5));
    EXPECT_TRUE(bst.contains(15));
    EXPECT_TRUE(bst.contains(3));
    EXPECT_TRUE(bst.contains(7));
    EXPECT_TRUE(bst.contains(12));
    EXPECT_TRUE(bst.contains(18));
}
TEST_F(BinarySearchTreeTest, SearchNonExistingValues) {
    EXPECT_FALSE(bst.contains(1));
    EXPECT_FALSE(bst.contains(20));
    EXPECT_FALSE(bst.contains(8));
}
TEST_F(BinarySearchTreeTest, InsertAndSearch) {
    bst.insert(11);
    EXPECT_TRUE(bst.contains(11));
    bst.insert(1);
    EXPECT_TRUE(bst.contains(1));
}
TEST_F(BinarySearchTreeTest, RemoveAndFindExtreme) {
    EXPECT_EQ(bst.findMax(),18);
    EXPECT_EQ(bst.findMin(),3);
    bst.remove(18);
    bst.remove(15);
    bst.remove(3);
    EXPECT_EQ(bst.findMax(),12);
    EXPECT_EQ(bst.findMin(),5);
}
