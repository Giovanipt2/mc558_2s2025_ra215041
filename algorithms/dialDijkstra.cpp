/**
 * dialDijkstra.cpp - Algoritmo de Dijkstra com Otimização de Dial
 * 
 * DESCRIÇÃO:
 * Implementa o algoritmo de Dial (otimização do Dijkstra para grafos com
 * pesos inteiros pequenos). Usa buckets circulares ao invés de priority queue,
 * resultando em complexidade O(V + E + C*V) onde C é o maior peso de aresta.
 * 
 * OBSERVAÇÃO IMPORTANTE:
 * Os pesos das arestas são ARREDONDADOS para o inteiro mais próximo.
 * Isso pode resultar em distâncias diferentes do Dijkstra clássico.
 * 
 * ENTRADA (stdin):
 * - Primeira linha: N M S D
 *   N = número de vértices (0..N-1)
 *   M = número de arestas
 *   S = vértice origem
 *   D = vértice destino
 * - Próximas M linhas: U V W
 *   U = vértice origem da aresta
 *   V = vértice destino da aresta
 *   W = peso da aresta (será arredondado para inteiro)
 * 
 * SAÍDA (stdout):
 * - Um único número: distância mínima de S a D (em inteiros)
 * - Ou "INF" se não existir caminho
 * 
 * COMPLEXIDADE:
 * - Temporal: O(V + E + C*V) onde C é o maior peso
 * - Espacial: O(V + E + C) para os buckets
 * 
 * LIMITAÇÕES:
 * - Se C for muito grande (> 10^6), a memória pode ser excessiva
 * - Pesos são arredondados, resultando em aproximação
 * 
 * COMPILAÇÃO:
 * g++ -std=c++17 -O2 dialDijkstra.cpp -o dialDijkstra
 * 
 * EXECUÇÃO:
 * cat input/grafo1.txt | ./dialDijkstra
 */

#include <iostream>
#include <vector>
#include <limits>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

const long long INF_LL = numeric_limits<long long>::max();

// Estrutura para representar uma aresta (vértice destino, peso inteiro)
struct Edge {
    int to;
    int weight;
    
    Edge(int t, int w) : to(t), weight(w) {}
};

// Tipo para o grafo: lista de adjacências
using Graph = vector<vector<Edge>>;

/**
 * Implementação do algoritmo de Dial
 * @param graph: grafo representado como lista de adjacências
 * @param N: número de vértices
 * @param S: vértice origem
 * @param D: vértice destino
 * @param C: maior peso de aresta (para dimensionar os buckets)
 * @return distância mínima de S a D, ou INF_LL se não houver caminho
 */
long long dialDijkstra(const Graph& graph, int N, int S, int D, int C) {
    // Vetor de distâncias inicializado com infinito
    vector<long long> dist(N, INF_LL);
    dist[S] = 0;
    
    // Buckets: cada bucket[i] contém vértices com distância i
    // Usamos o máximo possível de distância: N * C
    long long max_dist = (long long)N * C;
    vector<vector<int>> buckets(max_dist + 1);
    buckets[0].push_back(S);
    
    // Vetor para marcar vértices visitados
    vector<bool> visited(N, false);
    
    // Processa buckets em ordem crescente de distância
    for (long long d = 0; d <= max_dist; d++) {
        // Se o bucket está vazio, continua
        if (buckets[d].empty()) {
            continue;
        }
        
        // Processa todos os vértices no bucket atual
        while (!buckets[d].empty()) {
            int u = buckets[d].back();
            buckets[d].pop_back();
            
            // Se já foi visitado, ignora
            if (visited[u]) {
                continue;
            }
            
            visited[u] = true;
            
            // Otimização: se chegamos ao destino, podemos parar
            if (u == D) {
                return dist[D];
            }
            
            // Relaxamento das arestas adjacentes
            for (const Edge& edge : graph[u]) {
                int v = edge.to;
                long long new_dist = dist[u] + edge.weight;
                
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    
                    // Adiciona v ao bucket apropriado se não exceder o limite
                    if (new_dist <= max_dist) {
                        buckets[new_dist].push_back(v);
                    }
                }
            }
        }
    }
    
    return dist[D];
}

/**
 * Função auxiliar para remover espaços em branco no início e fim de uma string
 */
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

int main() {
    // Leitura da primeira linha: N M S D
    string line;
    if (!getline(cin, line)) {
        cerr << "Erro: não foi possível ler a primeira linha" << endl;
        return 1;
    }
    
    line = trim(line);
    istringstream iss(line);
    int N, M, S, D;
    
    if (!(iss >> N >> M >> S >> D)) {
        cerr << "Erro: formato inválido na primeira linha" << endl;
        return 1;
    }
    
    // Validação dos parâmetros
    if (N <= 0 || M < 0 || S < 0 || S >= N || D < 0 || D >= N) {
        cerr << "Erro: parâmetros inválidos" << endl;
        return 1;
    }
    
    // Construção do grafo e determinação do maior peso
    Graph graph(N);
    int C = 0;  // Maior peso inteiro de aresta
    
    for (int i = 0; i < M; i++) {
        if (!getline(cin, line)) {
            cerr << "Erro: número insuficiente de arestas" << endl;
            return 1;
        }
        
        line = trim(line);
        istringstream edge_stream(line);
        int u, v;
        double w_double;
        
        if (!(edge_stream >> u >> v >> w_double)) {
            cerr << "Erro: formato inválido na aresta " << (i + 1) << endl;
            return 1;
        }
        
        // Arredonda o peso para o inteiro mais próximo
        int w = static_cast<int>(round(w_double));
        
        // Validação dos vértices
        if (u < 0 || u >= N || v < 0 || v >= N) {
            cerr << "Erro: vértices inválidos na aresta " << (i + 1) << endl;
            return 1;
        }
        
        // Validação do peso (deve ser não-negativo após arredondamento)
        if (w < 0) {
            cerr << "Erro: peso negativo na aresta " << (i + 1) << endl;
            return 1;
        }
        
        // Adiciona aresta bidirecional (grafo não direcionado)
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
        C = max(C, w);
    }
    
    // Verificação de segurança: C muito grande pode causar problema de memória
    if (C > 10000000) {
        cerr << "Aviso: maior peso C=" << C << " é muito grande, pode causar problema de memória" << endl;
    }
    
    // Execução do algoritmo de Dial
    long long result = dialDijkstra(graph, N, S, D, C);
    
    // Saída do resultado
    if (result == INF_LL) {
        cout << "INF" << endl;
    } else {
        cout << result << endl;
    }
    
    return 0;
}
