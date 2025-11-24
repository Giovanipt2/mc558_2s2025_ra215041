/**
 * biDijkstra.cpp - Algoritmo de Dijkstra Bidirecional
 * 
 * DESCRIÇÃO:
 * Implementa o algoritmo de Dijkstra Bidirecional, que executa duas buscas
 * simultâneas: uma partindo da origem (forward) e outra partindo do destino
 * (backward). A busca termina quando as fronteiras se encontram.
 * 
 * VANTAGEM:
 * Para grafos grandes e esparsos, pode explorar aproximadamente metade dos
 * vértices comparado ao Dijkstra unidirecional, resultando em melhor desempenho.
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
 *   W = peso da aresta (double)
 * 
 * SAÍDA (stdout):
 * - Um único número: distância mínima de S a D
 * - Ou "INF" se não existir caminho
 * 
 * COMPLEXIDADE:
 * - Temporal: O((V + E) log V) - na prática mais rápido que Dijkstra unidirecional
 * - Espacial: O(V + E)
 * 
 * CORRETUDE:
 * O algoritmo mantém a menor distância encontrada através de vértices de encontro.
 * Termina quando a soma das menores chaves nas duas filas ≥ melhor caminho encontrado,
 * garantindo que não existe caminho melhor não explorado.
 * 
 * COMPILAÇÃO:
 * g++ -std=c++17 -O2 biDijkstra.cpp -o biDijkstra
 * 
 * EXECUÇÃO:
 * cat input/grafo1.txt | ./biDijkstra
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

const double INF_DOUBLE = numeric_limits<double>::infinity();

// Estrutura para representar uma aresta (vértice destino, peso)
struct Edge {
    int to;
    double weight;
    
    Edge(int t, double w) : to(t), weight(w) {}
};

// Tipo para o grafo: lista de adjacências
using Graph = vector<vector<Edge>>;

/**
 * Implementação do algoritmo de Dijkstra Bidirecional
 * @param graph_fwd: grafo forward (direção original)
 * @param graph_bwd: grafo backward (arestas reversas)
 * @param N: número de vértices
 * @param S: vértice origem
 * @param D: vértice destino
 * @return distância mínima de S a D, ou INF_DOUBLE se não houver caminho
 */
double biDijkstra(const Graph& graph_fwd, const Graph& graph_bwd, int N, int S, int D) {
    // Vetores de distâncias para as duas buscas
    vector<double> dist_f(N, INF_DOUBLE);  // Forward: de S
    vector<double> dist_b(N, INF_DOUBLE);  // Backward: de D
    
    dist_f[S] = 0.0;
    dist_b[D] = 0.0;
    
    // Priority queues para as duas direções
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq_f;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq_b;
    
    pq_f.push({0.0, S});
    pq_b.push({0.0, D});
    
    // Vetores para marcar vértices processados
    vector<bool> processed_f(N, false);
    vector<bool> processed_b(N, false);
    
    // Melhor distância encontrada até agora através de algum vértice de encontro
    double best_dist = INF_DOUBLE;
    
    while (!pq_f.empty() || !pq_b.empty()) {
        // Critério de parada: se as menores distâncias em ambas as filas
        // somadas são maiores ou iguais à melhor distância encontrada
        double min_f = pq_f.empty() ? INF_DOUBLE : pq_f.top().first;
        double min_b = pq_b.empty() ? INF_DOUBLE : pq_b.top().first;
        
        if (min_f + min_b >= best_dist) {
            break;
        }
        
        // Alterna entre processar da fila forward e backward
        // Escolhe a fila com menor distância mínima
        if (pq_b.empty() || (!pq_f.empty() && min_f <= min_b)) {
            // Processa da fila forward
            double d = pq_f.top().first;
            int u = pq_f.top().second;
            pq_f.pop();
            
            if (d > dist_f[u]) {
                continue;
            }
            
            processed_f[u] = true;
            
            // Verifica se há encontro com a busca backward
            if (dist_b[u] < INF_DOUBLE) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            // Relaxamento das arestas forward
            for (const Edge& edge : graph_fwd[u]) {
                int v = edge.to;
                double new_dist = dist_f[u] + edge.weight;
                
                if (new_dist < dist_f[v]) {
                    dist_f[v] = new_dist;
                    pq_f.push({new_dist, v});
                    
                    // Verifica encontro
                    if (dist_b[v] < INF_DOUBLE) {
                        best_dist = min(best_dist, dist_f[v] + dist_b[v]);
                    }
                }
            }
        } else {
            // Processa da fila backward
            double d = pq_b.top().first;
            int u = pq_b.top().second;
            pq_b.pop();
            
            if (d > dist_b[u]) {
                continue;
            }
            
            processed_b[u] = true;
            
            // Verifica se há encontro com a busca forward
            if (dist_f[u] < INF_DOUBLE) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            // Relaxamento das arestas backward
            for (const Edge& edge : graph_bwd[u]) {
                int v = edge.to;
                double new_dist = dist_b[u] + edge.weight;
                
                if (new_dist < dist_b[v]) {
                    dist_b[v] = new_dist;
                    pq_b.push({new_dist, v});
                    
                    // Verifica encontro
                    if (dist_f[v] < INF_DOUBLE) {
                        best_dist = min(best_dist, dist_f[v] + dist_b[v]);
                    }
                }
            }
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
    
    for (int i = 0; i < M; i++) {
        if (!getline(cin, line)) {
            cerr << "Erro: número insuficiente de arestas" << endl;
            return 1;
        }
        
        line = trim(line);
        istringstream edge_stream(line);
        int u, v;
        double w;
        
        if (!(edge_stream >> u >> v >> w)) {
            cerr << "Erro: formato inválido na aresta " << (i + 1) << endl;
            return 1;
        }
        
        // Validação dos vértices
        if (u < 0 || u >= N || v < 0 || v >= N) {
            cerr << "Erro: vértices inválidos na aresta " << (i + 1) << endl;
            return 1;
        }
        
        // Validação do peso (deve ser não-negativo)
        if (w < 0.0) {
            cerr << "Erro: peso negativo na aresta " << (i + 1) << endl;
            return 1;
        }
        
        // Adiciona arestas bidirecionais no grafo forward
        graph_fwd[u].emplace_back(v, w);
        graph_fwd[v].emplace_back(u, w);
        
        // Adiciona arestas bidirecionais no grafo backward
        graph_bwd[v].emplace_back(u, w);
        graph_bwd[u].emplace_back(v, w);
    }
    
    // Execução do algoritmo de Dijkstra Bidirecional
    double result = biDijkstra(graph_fwd, graph_bwd, N, S, D);
    
    // Saída do resultado
    if (result == INF_DOUBLE) {
        cout << "INF" << endl;
    } else {
        // Precisão de 3 casas decimais
        cout << fixed << setprecision(3) << result << endl;
    }
    
    return 0;
}
