#pragma once

#include "includs.hpp"

class Tree {
private:
    class Node {
    public:
        int data;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* father = nullptr;
        Node(int, Node* left, Node* right);
    };
    Node* root = nullptr;

    void print_tree_level(Node* top, int level);

    void remove(Node* root);

    Node* buildTree(int left, int right);

    void FillTree();

public:
    Tree(int n);

    ~Tree();

    void bfs();

    void print();

    void RootToAll(int rank);
};