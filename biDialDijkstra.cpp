/**
 * biDialDijkstra.cpp - Algoritmo de Dijkstra Bidirecional com Otimização de Dial
 * 
 * DESCRIÇÃO:
 * Combina as ideias do algoritmo bidirecional com a otimização de Dial.
 * Executa duas buscas simultâneas (forward e backward) usando buckets circulares
 * ao invés de priority queues.
 * 
 * OBSERVAÇÃO IMPORTANTE:
 * Os pesos das arestas são ARREDONDADOS para o inteiro mais próximo.
 * Combina as vantagens da busca bidirecional com a eficiência do Dial para
 * grafos com pesos inteiros pequenos.
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
 * - Espacial: O(V + E + 2*C) para os buckets de ambas direções
 * 
 * LIMITAÇÕES:
 * - Se C for muito grande (> 10^6), a memória pode ser excessiva
 * - Pesos são arredondados, resultando em aproximação
 * - Implementação mais complexa que os algoritmos individuais
 * 
 * COMPILAÇÃO:
 * g++ -std=c++17 -O2 biDialDijkstra.cpp -o biDialDijkstra
 * 
 * EXECUÇÃO:
 * cat input/grafo1.txt | ./biDialDijkstra
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
 * Implementação do algoritmo de Dijkstra Bidirecional com Dial
 * @param graph_fwd: grafo forward (direção original)
 * @param graph_bwd: grafo backward (arestas reversas)
 * @param N: número de vértices
 * @param S: vértice origem
 * @param D: vértice destino
 * @param C: maior peso de aresta (para dimensionar os buckets)
 * @return distância mínima de S a D, ou INF_LL se não houver caminho
 */
long long biDialDijkstra(const Graph& graph_fwd, const Graph& graph_bwd, int N, int S, int D, int C) {
    // Vetores de distâncias para as duas buscas
    vector<long long> dist_f(N, INF_LL);  // Forward: de S
    vector<long long> dist_b(N, INF_LL);  // Backward: de D
    
    dist_f[S] = 0;
    dist_b[D] = 0;
    
    // Buckets lineares para ambas as direções
    long long max_dist = (long long)N * C;
    vector<vector<int>> buckets_f(max_dist + 1);
    vector<vector<int>> buckets_b(max_dist + 1);
    
    buckets_f[0].push_back(S);
    buckets_b[0].push_back(D);
    
    // Vetores para marcar vértices visitados
    vector<bool> visited_f(N, false);
    vector<bool> visited_b(N, false);
    
    // Melhor distância encontrada até agora
    long long best_dist = INF_LL;
    
    // Índices atuais de distância para ambas as direções
    long long current_f = 0;
    long long current_b = 0;
    
    // Alterna entre processar forward e backward
    bool process_forward = true;
    
    while (current_f <= max_dist || current_b <= max_dist) {
        // Decide qual direção processar (alterna ou escolhe a com menor distância)
        bool has_forward = false;
        bool has_backward = false;
        
        // Procura próximo bucket não-vazio em forward
        while (current_f <= max_dist && buckets_f[current_f].empty()) {
            current_f++;
        }
        if (current_f <= max_dist) {
            has_forward = true;
        }
        
        // Procura próximo bucket não-vazio em backward
        while (current_b <= max_dist && buckets_b[current_b].empty()) {
            current_b++;
        }
        if (current_b <= max_dist) {
            has_backward = true;
        }
        
        // Se nenhum tem vértices, terminamos
        if (!has_forward && !has_backward) {
            break;
        }
        
        // Critério de parada: se encontramos caminho e as distâncias são suficientes
        if (best_dist < INF_LL) {
            if ((!has_forward || current_f >= best_dist) && 
                (!has_backward || current_b >= best_dist)) {
                break;
            }
        }
        
        // Processa forward se disponível e escolhido
        if (has_forward && (process_forward || !has_backward)) {
            int u = buckets_f[current_f].back();
            buckets_f[current_f].pop_back();
            
            if (visited_f[u]) {
                continue;
            }
            visited_f[u] = true;
            
            // Verifica encontro com backward
            if (dist_b[u] < INF_LL) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            // Relaxamento forward
            for (const Edge& edge : graph_fwd[u]) {
                int v = edge.to;
                long long new_dist = dist_f[u] + edge.weight;
                
                if (new_dist < dist_f[v]) {
                    dist_f[v] = new_dist;
                    
                    if (new_dist <= max_dist) {
                        buckets_f[new_dist].push_back(v);
                    }
                    
                    // Verifica encontro
                    if (dist_b[v] < INF_LL) {
                        best_dist = min(best_dist, dist_f[v] + dist_b[v]);
                    }
                }
            }
            
            process_forward = false;
        }
        // Processa backward
        else if (has_backward) {
            int u = buckets_b[current_b].back();
            buckets_b[current_b].pop_back();
            
            if (visited_b[u]) {
                continue;
            }
            visited_b[u] = true;
            
            // Verifica encontro com forward
            if (dist_f[u] < INF_LL) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            // Relaxamento backward
            for (const Edge& edge : graph_bwd[u]) {
                int v = edge.to;
                long long new_dist = dist_b[u] + edge.weight;
                
                if (new_dist < dist_b[v]) {
                    dist_b[v] = new_dist;
                    
                    if (new_dist <= max_dist) {
                        buckets_b[new_dist].push_back(v);
                    }
                    
                    // Verifica encontro
                    if (dist_f[v] < INF_LL) {
                        best_dist = min(best_dist, dist_f[v] + dist_b[v]);
                    }
                }
            }
            
            process_forward = true;
        }
    }
    
    return best_dist;
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
    
    // Construção dos grafos forward e backward
    Graph graph_fwd(N);
    Graph graph_bwd(N);
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
        
        // Adiciona arestas bidirecionais no grafo forward
        graph_fwd[u].emplace_back(v, w);
        graph_fwd[v].emplace_back(u, w);
        
        // Adiciona arestas bidirecionais no grafo backward
        graph_bwd[v].emplace_back(u, w);
        graph_bwd[u].emplace_back(v, w);
        
        C = max(C, w);
    }
    
    // Verificação de segurança: C muito grande pode causar problema de memória
    if (C > 10000000) {
        cerr << "Aviso: maior peso C=" << C << " é muito grande, pode causar problema de memória" << endl;
    }
    
    // Execução do algoritmo de Dijkstra Bidirecional com Dial
    long long result = biDialDijkstra(graph_fwd, graph_bwd, N, S, D, C);
    
    // Saída do resultado
    if (result == INF_LL) {
        cout << "INF" << endl;
    } else {
        cout << result << endl;
    }
    
    return 0;
}
