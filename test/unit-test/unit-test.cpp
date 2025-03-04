#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <fstream>
#include "tree.hpp"

TEST(TreeTest, BaseTest) {

    NdimensionalTree::Tree<int, 3> tree;

    const int value = 4;

    auto root_it = tree.Insert(tree.end(), value);

    auto find_it = tree.Find(value);

    ASSERT_EQ(root_it->GetData(), find_it->GetData());
}

TEST(TreeTest, IteratorTest) {

    std::vector<int> elems{1, 2, 3,
                           4, 5, 6,
                           7, 8, 9};

    NdimensionalTree::Tree<int, 3> tree(elems.begin(), elems.end());

    std::vector<int> pre_order_tree{1, 2, 5, 
                                    6, 7, 3, 
                                    8, 9, 4};

    std::vector<int> resulting_vector;

    for (auto it = tree.begin(); !(it == tree.end()); ++it) {
        resulting_vector.push_back(it->GetData());
    }

    ASSERT_EQ(pre_order_tree, resulting_vector);
}

TEST(TreeTest, EraseTest) {

    const std::string str = "world";

    NdimensionalTree::Tree<std::string, 2> tree("world");

    auto root_it = tree.begin();

    auto child1_it = tree.Insert(root_it, "get");
    auto child2_it = tree.Insert(root_it, "take");

    auto next_it = tree.Erase("get");

    ASSERT_EQ((*next_it).GetData(), (*root_it).GetData());
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}