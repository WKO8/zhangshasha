# Calculadora de Distância de Edição de Árvores (Zhang-Shasha)

Este projeto oferece uma implementação em C++ do algoritmo de Zhang e Shasha para calcular a Distância de Edição de Árvores (Tree Edit Distance - TED) entre duas árvores ordenadas e rotuladas.

A Distância de Edição é o número mínimo de operações de **inserção**, **exclusão** e **substituição** de nós necessários para transformar uma árvore na outra.

## Algoritmos Implementados

O projeto inclui e compara duas abordagens de programação dinâmica:

1.  **Algoritmo de Zhang-Shasha (Otimizado):** A implementação fiel do algoritmo rápido proposto por Kaizhong Zhang e Dennis Shasha em seu artigo de 1989. A principal otimização vem do uso de "keyroots" para reduzir drasticamente o número de subproblemas de distância de subárvores que precisam ser calculados.

2.  **Algoritmo Naive (Baseline):** Uma versão do mesmo algoritmo de programação dinâmica, mas sem a otimização dos keyroots. Ele calcula a distância para todos os pares de subflorestas possíveis, servindo como uma linha de base para demonstrar o ganho de eficiência do algoritmo de Zhang-Shasha.

## Estrutura dos Arquivos

* `main.cpp`: Contém o framework de testes. Ele define vários pares de árvores, executa os dois algoritmos para cada par, mede o tempo e o espaço, e salva os resultados em `results.csv`.
* `Tree.h` / `Tree.cpp`: A classe `Tree`, que encapsula a estrutura da árvore e implementa os métodos de pré-processamento (cálculo de índices pós-ordem, descendentes mais à esquerda e keyroots) e de travessia.
* `Node.h`: Define as estruturas de dados básicas para os nós (`Node`) e arestas (`Edge`) da árvore.

## Como Compilar e Executar

O projeto pode ser compilado com um compilador C++ moderno (suporte a C++11 ou superior é recomendado).

**Compilação:**

```bash
g++ main.cpp Tree.cpp -o ted_calculator -std=c++11 -O2
```

*A flag `-O2` é recomendada para otimização e benchmarks de tempo mais precisos.*

**Execução:**

```bash
./ted_calculator
```

## Formato de Entrada da Árvore

As árvores são definidas no `main.cpp` usando uma notação parentizada, onde um nó é seguido por seus filhos entre parênteses, separados por vírgulas.

**Exemplo:** A string `"f(d(a,c(b)),e)"` representa a seguinte árvore:

```
      f
     / \
    d   e
   / \
  a   c
      |
      b
```

## Saída do Programa

Ao ser executado, o programa irá:

1.  Imprimir os resultados da comparação de cada par de árvores no console, incluindo a distância calculada e o tempo médio de execução para cada algoritmo.
2.  Gerar um arquivo chamado `results.csv` com os seguintes campos, permitindo uma análise mais aprofundada:
    * `Tree1_Size`: Número de nós na primeira árvore.
    * `Tree2_Size`: Número de nós na segunda árvore.
    * `ZS_Distance`: Distância calculada pelo algoritmo de Zhang-Shasha.
    * `ZS_Time_ms_avg`: Tempo médio de execução (em milissegundos) de Zhang-Shasha.
    * `ZS_Space_bytes`: Espaço em memória (em bytes) usado pela matriz principal de Zhang-Shasha.
    * `Naive_Distance`: Distância calculada pelo algoritmo ingênuo.
    * `Naive_Time_ms_avg`: Tempo médio de execução (em milissegundos) do algoritmo ingênuo.
    * `Naive_Space_bytes`: Espaço em memória (em bytes) usado pela matriz principal do algoritmo ingênuo.
