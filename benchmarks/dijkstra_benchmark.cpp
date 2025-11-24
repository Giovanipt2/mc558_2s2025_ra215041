/**
 * dijkstra_benchmark.cpp - Dijkstra Clássico com Medição de Tempo
 * 
 * Versão modificada do dijkstra.cpp que inclui medição precisa do tempo
 * de execução do algoritmo, excluindo o tempo de leitura da entrada.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

const double INF_DOUBLE = numeric_limits<double>::infinity();

struct Edge {
    int to;
    double weight;
    
    Edge(int t, double w) : to(t), weight(w) {}
};

using Graph = vector<vector<Edge>>;

double dijkstra(const Graph& graph, int N, int S, int D) {
    vector<double> dist(N, INF_DOUBLE);
    dist[S] = 0.0;
    
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0.0, S});
    
    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        if (d > dist[u]) {
            continue;
        }
        
        if (u == D) {
            break;
        }
        
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

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

int main() {
    // Leitura e construção do grafo (NÃO incluso no benchmark)
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
    
    if (N <= 0 || M < 0 || S < 0 || S >= N || D < 0 || D >= N) {
        cerr << "Erro: parâmetros inválidos" << endl;
        return 1;
    }
    
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
        
        if (u < 0 || u >= N || v < 0 || v >= N || w < 0.0) {
            cerr << "Erro: valores inválidos na aresta " << (i + 1) << endl;
            return 1;
        }
        
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }
    
    // INÍCIO DO BENCHMARK - medição do tempo do algoritmo apenas
    auto start_time = high_resolution_clock::now();
    
    double result = dijkstra(graph, N, S, D);
    
    auto end_time = high_resolution_clock::now();
    // FIM DO BENCHMARK
    
    // Calcula tempo em microssegundos
    auto duration = duration_cast<microseconds>(end_time - start_time);
    
    // Saída do resultado
    if (result == INF_DOUBLE) {
        cout << "INF" << endl;
    } else {
        cout << fixed << setprecision(3) << result << endl;
    }
    
    // Saída do tempo (para stderr para não interferir com resultado)
    cerr << duration.count() << endl;
    
    return 0;
}