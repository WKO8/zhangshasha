/*
============================================================================
 Arquivo: Tree.cpp
 Descrição: Implementação dos métodos da classe Tree.
============================================================================
*/
#include "Tree.h"
#include <iostream>
#include <algorithm>

Tree::Tree() {}

Tree::Tree(std::vector<Node> t, int r) {
    nodelist = t;
    root = r;
}

Tree::~Tree() {}

// Função principal que orquestra todo o pré-processamento
void Tree::build() {
    if (root < 0 || nodelist.empty()) return;
    
    // Limpa dados de execuções anteriores
    left.clear();
    keyroots.clear();
    labels.clear();
    for(auto& node : nodelist) {
        node.index = -1;
        node.leftmost = -1;
    }

    getindex();
    getleftarr();
    getkeyroots();
    traverse();
}


void Tree::traverse() {
    labels.clear();
    if(root >= 0) traverse(root);
}

void Tree::traverse(int node_idx) {
    for (const auto& edge : nodelist[node_idx].edgelist) {
        traverse(edge.childnodenum);
    }
    labels.push_back(nodelist[node_idx].nodename);
}

void Tree::getindex() {
    if(root >= 0) getindex(root, 0);
}

int Tree::getindex(int node_idx, int index) {
    for (const auto& edge : nodelist[node_idx].edgelist) {
        index = getindex(edge.childnodenum, index);
    }
    index++;
    nodelist[node_idx].index = index;
    return index;
}

void Tree::getleftarr() {
    left.clear();
    if (root < 0) return;
    // Primeiro calcula o 'leftmost' para cada nó
    getnodeleft();
    // Depois popula o array 'left' na ordem pós-fixa correta
    left.resize(nodelist.size());
    getleftarr(root);
}

void Tree::getleftarr(int node_idx) {
    for (const auto& edge : nodelist[node_idx].edgelist) {
        getleftarr(edge.childnodenum);
    }
    // O índice pós-fixo - 1 corresponde à posição no array
    left[nodelist[node_idx].index - 1] = nodelist[node_idx].leftmost;
}

void Tree::getnodeleft() {
    if (root >= 0) getnodeleft(root);
}

void Tree::getnodeleft(int node_idx) {
    if (node_idx < 0) return;

    for (const auto& edge : nodelist[node_idx].edgelist) {
        getnodeleft(edge.childnodenum);
    }

    if (nodelist[node_idx].edgelist.empty()) {
        nodelist[node_idx].leftmost = nodelist[node_idx].index;
    } else {
        int first_child_idx = nodelist[node_idx].edgelist[0].childnodenum;
        nodelist[node_idx].leftmost = nodelist[first_child_idx].leftmost;
    }
}

void Tree::getkeyroots() {
    keyroots.clear();
    if (root < 0) return;
    
    std::vector<bool> is_keyroot(nodelist.size() + 1, true);
    for (size_t i = 0; i < left.size(); ++i) {
        for (size_t j = i + 1; j < left.size(); ++j) {
            if (left[j] == left[i]) {
                is_keyroot[i + 1] = false;
                break;
            }
        }
    }
    
    for (size_t i = 1; i <= nodelist.size(); ++i) {
        if (is_keyroot[i]) {
            keyroots.push_back(i);
        }
    }
}