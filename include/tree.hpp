#pragma once

#include <array>
#include <cstddef>
#include <memory>

// TODO:
// 1) реализовать remove, insert, fill (можно просто разные конструкторы)
// 2) реализовать отдельный класс TreePrinter
// 3) добавить итераторы и разобраться с их видами

namespace NdimensionalTree {

    template<typename T, std::size_t n = 2>
    class Tree final {

    private:
        struct Node final {

            T data;
            std::array<std::unique_ptr<Node>, n> childs;

            explicit Node(const T& value) : data(value) {

                childs.fill(nullptr);
            }
        };

        std::unique_ptr<Node> root;

        Node* FindNode(Node* node, const T& value) const {

            if (!node) {
                return nullptr;
            } 
            if (node->data == value) {
                return node;
            }
    
            for (const auto& child : node->childs) {
                if (child) {
                    Node* result = findNode(child.get(), value);         
                    if (result) {
                        return result;
                    }
                }
            }
    
            return nullptr;
        }

    public:
        Tree() noexcept : root(nullptr) {}

        const Node* GetRoot() const noexcept {

            return root.get();
        }

        const std::array<std::unique_ptr<Node>, n>& GetChilds(const Node* node) const { // READ: (Gettеры)

            return node->childs;
        }

        bool Find(const T& value) const { // TODO: изменить возвращаемый тип на Iterator, как только добавлю поддержку

            return FindNode(root.get(), value) != nullptr;
        }
    };
}