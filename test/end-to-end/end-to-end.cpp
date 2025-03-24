#include <iostream>
#include <iterator>
#include <vector>
#include <numeric>
#include "tree.hpp"

int main() {

    try {
        const size_t size = 5;
        const int root_value_tree2 = 100;
        const int remove_value = 3;

        std::vector<int> vector(size * size);
        std::iota(vector.begin(), vector.end(), 1); 

        NdimensionalTree::Tree<int, size> tree1(vector.begin(), vector.end());

        NdimensionalTree::Tree<int, size> tree2(root_value_tree2);
        auto root_it = tree2.begin();

        auto child1_it = tree2.Insert(root_it, 1);
        auto child2_it = tree2.Insert(root_it, 2);  
        auto child3_it = tree2.Insert(root_it, 3);
        auto child4_it = tree2.Insert(root_it, 4);  
        auto child5_it = tree2.Insert(root_it, 5);

        for (auto it = tree2.begin(); it != tree2.end(); ++it) {
            std::cout << it->GetData() << " ";
        }
        std::cout << "\n";

        auto find_it = tree1.Find(remove_value);
        auto remove_it = tree1.Erase(remove_value);

        auto remove_it1 = tree1.Erase(4);

        tree1.Insert(tree1.begin(), 100);

        NdimensionalTree::TreePrinter<int, size> printer;

        printer.GenerateDotFile(*(tree1.begin()), "../../../examples/tree.dot");

        printer.PrintTree(*root_it);

    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }


    return 0;
}