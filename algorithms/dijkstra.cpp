/**
 * dijkstra.cpp - Algoritmo de Dijkstra Clássico
 * 
 * DESCRIÇÃO:
 * Implementa o algoritmo de Dijkstra para encontrar o caminho mais curto entre
 * dois vértices em um grafo direcionado com pesos não-negativos.
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
 * - Temporal: O((V + E) log V) com priority_queue
 * - Espacial: O(V + E)
 * 
 * COMPILAÇÃO:
 * g++ -std=c++17 -O2 dijkstra.cpp -o dijkstra
 * 
 * EXECUÇÃO:
 * cat input/grafo1.txt | ./dijkstra
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <sstream>
#include <string>
#include <iomanip>

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
 * Implementação do algoritmo de Dijkstra
 * @param graph: grafo representado como lista de adjacências
 * @param N: número de vértices
 * @param S: vértice origem
 * @param D: vértice destino
 * @return distância mínima de S a D, ou INF_DOUBLE se não houver caminho
 */
double dijkstra(const Graph& graph, int N, int S, int D) {
    // Vetor de distâncias inicializado com infinito
    vector<double> dist(N, INF_DOUBLE);
    dist[S] = 0.0;
    
    // Priority queue: pair<distância, vértice>
    // Usamos greater para termos um min-heap
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0.0, S});
    
    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        // Se a distância extraída é maior que a registrada, ignoramos
        // (pode haver duplicatas na fila)
        if (d > dist[u]) {
            continue;
        }
        
        // Otimização: se chegamos ao destino, podemos parar
        if (u == D) {
            break;
        }
        
        // Relaxamento das arestas adjacentes
        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            double new_dist = dist[u] + edge.weight;
            
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                pq.push({new_dist, v});
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
    
    // Construção do grafo
    Graph graph(N);
    
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
        
        // Adiciona aresta bidirecional (grafo não direcionado)
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }
    
    // Execução do algoritmo de Dijkstra
    double result = dijkstra(graph, N, S, D);
    
    // Saída do resultado
    if (result == INF_DOUBLE) {
        cout << "INF" << endl;
    } else {
        // Precisão de 3 casas decimais
        cout << fixed << setprecision(3) << result << endl;
    }
    
    return 0;
}
