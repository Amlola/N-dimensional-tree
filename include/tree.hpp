#pragma once

#include <bits/types/FILE.h>
#include <type_traits>
#include <vector>
#include <cstddef>
#include <memory>
#include <stack>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <queue>

namespace NdimensionalTree {

    template<typename T, std::size_t N = 2>
    class Tree final {

    public:
        class Node final {

            T data;
            std::vector<std::unique_ptr<Node>> children;
            Node* parent;

        public:
            explicit Node(const T& value, Node* parent = nullptr) : data(value), parent(parent) {}

            explicit Node(T&& value, Node* parent = nullptr) : data(std::move(value)), parent(parent) {}

            std::vector<std::unique_ptr<Node>>& GetChildren() noexcept {

                return children;
            }

            const std::vector<std::unique_ptr<Node>>& GetChildren() const noexcept {

                return children;
            }

            const T& GetData() const noexcept {

                return data;
            } 

            T& GetData() noexcept {

                return data;
            }

            std::size_t GetChildrenSize() const noexcept {

                return children.size();
            }

            Node* GetParent() const noexcept {

                return parent;
            }
        };

    private:
        std::unique_ptr<Node> root;

        template <typename It>
        void FillTree(std::queue<Node*>& queue, It& current, It& end) {
        
            while (!queue.empty() && current != end) {
                Node* current_node = queue.front();
                queue.pop();
    
                auto& children = current_node->GetChildren();
                std::size_t children_size = current_node->GetChildrenSize();

                while (children_size++ < N && current != end) {
                    auto new_node = std::make_unique<Node>(*current, current_node);
                    children.push_back(std::move(new_node));
                    queue.push(children.back().get());
                    ++current;
                }
            }
        }

    public:
        Tree() noexcept = default;

        explicit Tree(const T& value) : root(std::make_unique<Node>(value)) {}

        template <typename It>
        Tree(It begin, It end) {

            if (begin == end) {
                return;
            }

            root = std::make_unique<Node>(*begin, nullptr);
            ++begin;

            std::queue<Node*> queue;
            queue.push(root.get());

            FillTree(queue, begin, end);
        }

        class Iterator final {
        
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

        private:
            std::stack<Node*> stack;

        public:
            explicit Iterator(Node* node = nullptr) {

                if (node) {
                    stack.push(node);
                }
            }

            Node& operator*() const {

                return *stack.top();
            }

            Node* operator->() const {

                return stack.top();
            }

            Iterator& operator++() {

                if (stack.empty()) {
                    return *this;
                }

                Node* current = stack.top();
                stack.pop();

                const auto& children = current->GetChildren();

                for (auto it = children.rbegin(); it != children.rend(); ++it) {
                    stack.push(it->get());
                }

                return *this;
            }

            bool operator==(const Iterator& other) const {

                if (stack.empty() && other.stack.empty()) {
                    return true;
                }
                if (stack.empty() || other.stack.empty()) {
                    return false;
                }

                return stack.top() == other.stack.top();
            }
        };

    private:     
        Iterator FindNode(Node& node, const T& value) const {

            if (node.GetData() == value) {
                return Iterator(&node);
            }

            const auto& children = node.GetChildren();            
            for (const auto& child : children) {
                auto result = FindNode(*child, value);         
                if (result != end()) {
                    return result;
                }
            }

            return end();
        }

    public:
        Iterator begin() const noexcept {

            return Iterator(root.get());
        }
    
        Iterator end() const noexcept {

            return Iterator(nullptr);
        }

        Iterator Find(const T& value) const {

            return FindNode(*root, value);
        }

        Iterator Insert(Iterator parent_it, const T& value) {
        
            if (!root) {
                root = std::make_unique<Node>(value, nullptr);
                return Iterator(root.get());
            }

            if (parent_it == end()) {
                throw std::invalid_argument("Invalid parent iterator");
            }

            Node* parent_node = parent_it.operator->();

            if (parent_node->GetChildrenSize() >= N) {
                throw std::invalid_argument("Maximum capacity of one node reached");
            }

            auto& children = parent_node->GetChildren();

            children.push_back(std::make_unique<Node>(value, parent_node));

            return Iterator(children.back().get());
        }

        Iterator Erase(const T& value) {

            auto value_it = Find(value);
            if (value_it == end()) {
                return value_it;
            }
        
            Node* remove_node = value_it.operator->();
            Node* parent_node = remove_node->GetParent();
        
            if (!parent_node) {
                root.reset();
                return end();
            }
        
            auto& children = parent_node->GetChildren();
            auto child_it = std::find_if(children.begin(), children.end(),
                [remove_node](const std::unique_ptr<Node>& child) {
                    return child.get() == remove_node;
                });
        
            if (child_it != children.end()) {
                auto next_child_it = std::next(child_it);
                children.erase(child_it);
        
                if (next_child_it != children.end()) {
                    return Iterator(next_child_it->get());
                }
        
                return Iterator(parent_node);
            }
        
            return end();
        }
    };

    template <typename T, std::size_t N = 2>
    class TreePrinter {

    public:    
        void PrintTree(const typename Tree<T, N>::Node& node) const {

            std::ostringstream oss;
            GetPreOrder(node, oss);

            std::cout << oss.str() << "\n";
        }

        void GenerateDotFile(const typename Tree<T, N>::Node& node, const std::string& filename) const {

            std::ofstream dot_file(filename);
            if (dot_file.is_open()) {
                dot_file << "digraph Tree {\n";
        
                std::unordered_map<const typename Tree<T, N>::Node*, std::size_t> node_ids;
                std::size_t counter{};
        
                GenerateDotFile(node, dot_file, node_ids, counter);
        
                dot_file << "}\n";
            } else {
                std::ostringstream error_message;
                error_message << "Can't open file: " << filename;
                throw std::invalid_argument(error_message.str());
            }
        }
        
    
    private:
        void GetPreOrder(const typename Tree<T, N>::Node& node, std::ostringstream& oss) const {

            oss << node.GetData() << " ";

            const auto& children = node.GetChildren();
    
            for (const auto& child : children) {
                GetPreOrder(*child, oss);
            }
        }

        void GenerateDotFile(const typename Tree<T, N>::Node& node, std::ofstream& dot_file, std::unordered_map<const typename Tree<T, N>::Node*, std::size_t>& node_ids, std::size_t& counter) const {

            if (node_ids.find(&node) == node_ids.end()) {
                node_ids[&node] = counter++;
            }
        
            const std::size_t& node_id = node_ids[&node];
            dot_file << "  " << node_id << " [label=\"" << node.GetData() << "\"];\n";

            const auto& children = node.GetChildren();
        
            for (const auto& child : children) {
                auto child_node = child.get();
                if (node_ids.find(child_node) == node_ids.end()) {
                    node_ids[child_node] = counter++;
                }
        
            const std::size_t& child_id = node_ids[child.get()];
            dot_file << "  " << node_id << " -> " << child_id << ";\n";
    
            GenerateDotFile(*child, dot_file, node_ids, counter);
            }
        }
    };
}