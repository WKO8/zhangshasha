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
    std::vector<Edge> edgelist; // Lista de arestas (filhos) do nó
    std::string nodename;
    int index = -1;             // Índice pós-ordem (calculado)
    int leftmost = -1;          // Índice do descendente folha mais à esquerda (calculado)

    Node() = default;
    Node(const std::vector<Edge>& edges, std::string name) : edgelist(edges), nodename(name) {}
};

#endif