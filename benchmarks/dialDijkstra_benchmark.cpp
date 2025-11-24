/**
 * dialDijkstra_benchmark.cpp - Dijkstra com Dial com Medição de Tempo
 */

#include <iostream>
#include <vector>
#include <limits>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

const long long INF_LL = numeric_limits<long long>::max();

struct Edge {
    int to;
    int weight;
    
    Edge(int t, int w) : to(t), weight(w) {}
};

using Graph = vector<vector<Edge>>;

long long dialDijkstra(const Graph& graph, int N, int S, int D, int C) {
    vector<long long> dist(N, INF_LL);
    dist[S] = 0;
    
    long long max_dist = (long long)N * C;
    vector<vector<int>> buckets(max_dist + 1);
    buckets[0].push_back(S);
    
    vector<bool> visited(N, false);
    
    for (long long d = 0; d <= max_dist; d++) {
        if (buckets[d].empty()) {
            continue;
        }
        
        while (!buckets[d].empty()) {
            int u = buckets[d].back();
            buckets[d].pop_back();
            
            if (visited[u]) {
                continue;
            }
            
            visited[u] = true;
            
            if (u == D) {
                return dist[D];
            }
            
            for (const Edge& edge : graph[u]) {
                int v = edge.to;
                long long new_dist = dist[u] + edge.weight;
                
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    
                    if (new_dist <= max_dist) {
                        buckets[new_dist].push_back(v);
                    }
                }
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
    int C = 0;
    
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
        
        int w = static_cast<int>(round(w_double));
        
        if (u < 0 || u >= N || v < 0 || v >= N || w < 0) {
            cerr << "Erro: valores inválidos na aresta " << (i + 1) << endl;
            return 1;
        }
        
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
        C = max(C, w);
    }
    
    if (C > 10000000) {
        cerr << "Aviso: maior peso C=" << C << " é muito grande" << endl;
    }
    
    auto start_time = high_resolution_clock::now();
    
    long long result = dialDijkstra(graph, N, S, D, C);
    
    auto end_time = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end_time - start_time);
    
    if (result == INF_LL) {
        cout << "INF" << endl;
    } else {
        cout << result << endl;
    }
    
    cerr << duration.count() << endl;
    
    return 0;
}