#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include "Tree.h"

Tree build_tree_from_string(const std::string& s);
int zhang_shasha_distance(Tree& tree1, Tree& tree2);
void treedist_optimized(Tree& tree1, Tree& tree2, int i, int j, std::vector<std::vector<int>>& TD);
int naive_distance(Tree& tree1, Tree& tree2);
void treedist_naive(Tree& tree1, Tree& tree2, int i, int j, std::vector<std::vector<int>>& TD);

int main() {
    // Lista de pares de árvores para testar
    // Adicionados casos maiores para demonstrar a diferença de performance.
    std::vector<std::pair<std::string, std::string>> test_cases = {
        // Casos Pequenos (para validação)
        {"f(d(a,c(b)),e)", "f(c(d(a,b)),e)"},
        {"a(b(c,d),e(f,g(i)))", "a(b(c,d),e(f,g(h)))"},
        {"d", "g(h)"},
        {"a(b,c,d)", "a(c,d,b)"},
        // Caso Médio (Profundo)
        {"a(b(c(d(e(f(g(h(i(j))))))))))", "a(b(c(d(e(f(g(h(i(k))))))))))"},
        // Caso Médio (Largo)
        {"a(b,c,d,e,f,g,h,i,j,k)", "a(c,b,e,d,g,f,i,h,k,j)"},
        
        // MELHOR CASO para Zhang-Shasha: Árvore "pente", alta e estreita.
        // Possui pouquíssimos keyroots, então a otimização é máxima.
        {"a(b(c(d(e(f(g(h(i(j(k(l(m(n(o(p(q(r(s(t))))))))))))))))))))", 
         "a(b(c(d(e(f(g(h(i(j(k(l(m(n(o(p(q(r(s(z))))))))))))))))))))"},
        
        // PIOR CASO para Zhang-Shasha: Árvore "arbusto", baixa e larga.
        // Quase todos os nós são keyroots, então o desempenho se aproxima do ingênuo.
        {"r(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,s,t,u,v,w,x,y,z)",
         "r(z,y,x,w,v,u,t,s,q,p,o,n,m,l,k,j,i,h,g,f,e,d,c,b,a)"},
         
        // CASO MISTO: Árvore com partes largas e partes profundas.
        {"a(b(c,d),e(f,g(h,i(j,k),l(m,n(o(p,q),r),s),t),u),v(w,x,y,z))",
         "a(b(c,d),e(f,g(h,i(j,k),l(m,n(o(p,A),r),s),t),u),v(w,x,y,z))"}
    };

    std::ofstream results_file("results.csv");
    if (!results_file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo results.csv" << std::endl;
        return 1;
    }
    results_file << "Tree1_Size,Tree2_Size,ZS_Distance,ZS_Time_ms_avg,ZS_Space_bytes,Naive_Distance,Naive_Time_ms_avg,Naive_Space_bytes\n";

    for (const auto& test_pair : test_cases) {
        try {
            Tree tree1 = build_tree_from_string(test_pair.first);
            Tree tree2 = build_tree_from_string(test_pair.second);
            
            size_t size1 = tree1.nodelist.size();
            size_t size2 = tree2.nodelist.size();
            
            // Número de repetições para o benchmark
            int repetitions = 1000;

            std::cout << "-----------------------------------------------------\n";
            std::cout << "Testando T1 de " << size1 << " nos e T2 de " << size2 << " nos (repetindo " << repetitions << " vezes)\n";

            // Benchmark do Algoritmo de Zhang-Shasha
            int dist_zs = 0;
            auto start_zs = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < repetitions; ++i) {
                dist_zs = zhang_shasha_distance(tree1, tree2);
            }
            auto end_zs = std::chrono::high_resolution_clock::now();
            double time_zs_avg = std::chrono::duration<double, std::milli>(end_zs - start_zs).count() / repetitions;
            
            size_t space_zs = (size1 + 1) * (size2 + 1) * sizeof(int);

            // Benchmark do Algoritmo Naive
            int dist_naive = 0;
            auto start_naive = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < repetitions; ++i) {
                dist_naive = naive_distance(tree1, tree2);
            }
            auto end_naive = std::chrono::high_resolution_clock::now();
            double time_naive_avg = std::chrono::duration<double, std::milli>(end_naive - start_naive).count() / repetitions;
            
            size_t space_naive = (size1 + 1) * (size2 + 1) * sizeof(int);
            
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "[Zhang-Shasha] Distancia: " << dist_zs << ", Tempo Medio: " << time_zs_avg << " ms\n";
            std::cout << "[Naive]      Distancia: " << dist_naive << ", Tempo Medio: " << time_naive_avg << " ms\n";

            results_file << size1 << "," << size2 << "," << dist_zs << "," << time_zs_avg << ","
                         << space_zs << "," << dist_naive << "," << time_naive_avg << "," << space_naive << "\n";

        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar o par de arvores: " << e.what() << std::endl;
        }
    }

    results_file.close();
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Experimentos concluidos. Resultados salvos em results.csv\n";
    return 0;
}

// Implementação dos Algoritmos

int zhang_shasha_distance(Tree& tree1, Tree& tree2) {
    tree1.build();
    tree2.build();
    if (tree1.nodelist.empty() || tree2.nodelist.empty()) return tree1.nodelist.size() + tree2.nodelist.size();
    std::vector<std::vector<int>> TD(tree1.nodelist.size() + 1, std::vector<int>(tree2.nodelist.size() + 1, 0));
    for (int i : tree1.keyroots) {
        for (int j : tree2.keyroots) {
            treedist_optimized(tree1, tree2, i, j, TD);
        }
    }
    return TD[tree1.nodelist.size()][tree2.nodelist.size()];
}

void treedist_optimized(Tree& tree1, Tree& tree2, int i, int j, std::vector<std::vector<int>>& TD) {
    std::vector<std::vector<int>> forestdist(tree1.nodelist.size() + 1, std::vector<int>(tree2.nodelist.size() + 1, 0));
    const int D = 1, I = 1, R = 1;
    int l1_i = tree1.left[i - 1];
    int l2_j = tree2.left[j - 1];
    for (int i1 = l1_i; i1 <= i; ++i1) forestdist[i1][l2_j - 1] = forestdist[i1 - 1][l2_j - 1] + D;
    for (int j1 = l2_j; j1 <= j; ++j1) forestdist[l1_i - 1][j1] = forestdist[l1_i - 1][j1 - 1] + I;
    for (int i1 = l1_i; i1 <= i; ++i1) {
        for (int j1 = l2_j; j1 <= j; ++j1) {
            if (tree1.left[i1 - 1] == l1_i && tree2.left[j1 - 1] == l2_j) {
                int cost = (tree1.labels[i1 - 1] == tree2.labels[j1 - 1]) ? 0 : R;
                forestdist[i1][j1] = std::min({ forestdist[i1 - 1][j1] + D, forestdist[i1][j1 - 1] + I, forestdist[i1 - 1][j1 - 1] + cost });
                TD[i1][j1] = forestdist[i1][j1];
            } else {
                forestdist[i1][j1] = std::min({ forestdist[i1 - 1][j1] + D, forestdist[i1][j1 - 1] + I, forestdist[tree1.left[i1 - 1] - 1][tree2.left[j1 - 1] - 1] + TD[i1][j1] });
            }
        }
    }
    TD[i][j] = forestdist[i][j];
}

int naive_distance(Tree& tree1, Tree& tree2) {
    tree1.build();
    tree2.build();
    if (tree1.nodelist.empty() || tree2.nodelist.empty()) return tree1.nodelist.size() + tree2.nodelist.size();
    std::vector<std::vector<int>> TD(tree1.nodelist.size() + 1, std::vector<int>(tree2.nodelist.size() + 1, 0));
    for (size_t i = 1; i <= tree1.nodelist.size(); ++i) {
        for (size_t j = 1; j <= tree2.nodelist.size(); ++j) {
            treedist_naive(tree1, tree2, i, j, TD);
        }
    }
    return TD[tree1.nodelist.size()][tree2.nodelist.size()];
}

void treedist_naive(Tree& tree1, Tree& tree2, int i, int j, std::vector<std::vector<int>>& TD) {
    std::vector<std::vector<int>> forestdist(tree1.nodelist.size() + 1, std::vector<int>(tree2.nodelist.size() + 1, 0));
    const int D = 1, I = 1, R = 1;
    int l1_i = tree1.left[i - 1];
    int l2_j = tree2.left[j - 1];
    for (int i1 = l1_i; i1 <= i; ++i1) forestdist[i1][l2_j - 1] = forestdist[i1 - 1][l2_j - 1] + D;
    for (int j1 = l2_j; j1 <= j; ++j1) forestdist[l1_i - 1][j1] = forestdist[l1_i - 1][j1 - 1] + I;
    for (int i1 = l1_i; i1 <= i; ++i1) {
        for (int j1 = l2_j; j1 <= j; ++j1) {
            if (tree1.left[i1 - 1] == l1_i && tree2.left[j1 - 1] == l2_j) {
                int cost = (tree1.labels[i1 - 1] == tree2.labels[j1 - 1]) ? 0 : R;
                forestdist[i1][j1] = std::min({ forestdist[i1 - 1][j1] + D, forestdist[i1][j1 - 1] + I, forestdist[i1 - 1][j1 - 1] + cost });
            } else {
                forestdist[i1][j1] = std::min({ forestdist[i1 - 1][j1] + D, forestdist[i1][j1 - 1] + I, forestdist[tree1.left[i1 - 1] - 1][tree2.left[j1 - 1] - 1] + TD[i1][j1] });
            }
        }
    }
    TD[i][j] = forestdist[i][j];
}

// --- Implementação do Parser de Árvores ---

void parse_recursive(const std::string& s, int& pos, std::vector<Node>& nodes, int parent_idx) {
    while (pos < s.length()) {
        if (s[pos] == ' ' || s[pos] == ',') { pos++; continue; }
        if (s[pos] == ')' || s[pos] == ']') { pos++; return; }
        size_t start_label = pos;
        while (pos < s.length() && isalnum(s[pos])) pos++;
        std::string label = s.substr(start_label, pos - start_label);
        if (label.empty()) throw std::runtime_error("Erro de parsing: rotulo de no vazio");

        Node new_node;
        new_node.nodename = label;
        nodes.push_back(new_node);
        int current_node_idx = nodes.size() - 1;
        if (parent_idx != -1) nodes[parent_idx].edgelist.push_back(Edge(current_node_idx));
        if (pos < s.length() && (s[pos] == '(' || s[pos] == '[')) {
            pos++;
            parse_recursive(s, pos, nodes, current_node_idx);
        }
    }
}

Tree build_tree_from_string(const std::string& s) {
    if (s.empty()) return Tree({}, -1);
    std::vector<Node> nodes;
    int pos = 0;
    parse_recursive(s, pos, nodes, -1);
    if (nodes.empty()) throw std::runtime_error("Erro de parsing: arvore vazia");
    return Tree(nodes, 0);
}
