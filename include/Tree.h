#ifndef TREE_H
#define TREE_H

#include "Node.h"
#include <vector>
#include <string>

class Tree {
public:
    std::vector<Node> nodelist;
    int root = -1;

    // Vetores para os dados do pré-processamento de Zhang-Shasha
    std::vector<int> left;      // m(i) -> leftmost leaf descendant
    std::vector<int> keyroots;
    std::vector<std::string> labels;

    // Construtores
    Tree();
    Tree(std::vector<Node> t, int r);
    ~Tree();

    // Funções de pré-processamento
    void build();
    void traverse();
    void getindex();
    void getleftarr();
    void getnodeleft();
    void getkeyroots();

private:
    // Funções auxiliares recursivas
    void traverse(int node_idx);
    int getindex(int node_idx, int index);
    void getleftarr(int node_idx);
    void getnodeleft(int node_idx);
};

#endif