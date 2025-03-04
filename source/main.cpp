#include <iostream>
#include <iterator>
#include <vector>
#include "../include/tree.hpp"

int main() {

    try {
        std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        NdimensionalTree::Tree<int, 3> tree(values.begin(), values.end());

        auto root_tree1_it = tree.Find(1);

        NdimensionalTree::TreePrinter<int, 3> printer;

        printer.PrintTree(*root_tree1_it);
        
    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }

    return 0;
}