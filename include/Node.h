#ifndef MYNODE_H
#define MYNODE_H

#include <vector>
#include <string>

// Estrutura para representar uma aresta, apontando para o índice de um nó filho.
struct Edge {
    int childnodenum;
    Edge(int c) : childnodenum(c) {}
};

// Estrutura para representar um nó da árvore.
struct Node {
    std::vector<Edge> edgelist;
    std::string nodename;
    int index = -1;
    int leftmost = -1;

    Node() = default;
    Node(const std::vector<Edge>& edges, std::string name) : edgelist(edges), nodename(name) {}
};

#endif