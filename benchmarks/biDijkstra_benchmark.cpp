/**
 * biDijkstra_benchmark.cpp - Dijkstra Bidirecional com Medição de Tempo
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
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

double biDijkstra(const Graph& graph_fwd, const Graph& graph_bwd, int N, int S, int D) {
    vector<double> dist_f(N, INF_DOUBLE);
    vector<double> dist_b(N, INF_DOUBLE);
    
    dist_f[S] = 0.0;
    dist_b[D] = 0.0;
    
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq_f;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq_b;
    
    pq_f.push({0.0, S});
    pq_b.push({0.0, D});
    
    vector<bool> processed_f(N, false);
    vector<bool> processed_b(N, false);
    
    double best_dist = INF_DOUBLE;
    
    while (!pq_f.empty() || !pq_b.empty()) {
        double min_f = pq_f.empty() ? INF_DOUBLE : pq_f.top().first;
        double min_b = pq_b.empty() ? INF_DOUBLE : pq_b.top().first;
        
        if (min_f + min_b >= best_dist) {
            break;
        }
        
        if (pq_b.empty() || (!pq_f.empty() && min_f <= min_b)) {
            double d = pq_f.top().first;
            int u = pq_f.top().second;
            pq_f.pop();
            
            if (d > dist_f[u]) {
                continue;
            }
            
            processed_f[u] = true;
            
            if (dist_b[u] < INF_DOUBLE) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            for (const Edge& edge : graph_fwd[u]) {
                int v = edge.to;
                double new_dist = dist_f[u] + edge.weight;
                
                if (new_dist < dist_f[v]) {
                    dist_f[v] = new_dist;
                    pq_f.push({new_dist, v});
                    
                    if (dist_b[v] < INF_DOUBLE) {
                        best_dist = min(best_dist, dist_f[v] + dist_b[v]);
                    }
                }
            }
        } else {
            double d = pq_b.top().first;
            int u = pq_b.top().second;
            pq_b.pop();
            
            if (d > dist_b[u]) {
                continue;
            }
            
            processed_b[u] = true;
            
            if (dist_f[u] < INF_DOUBLE) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            for (const Edge& edge : graph_bwd[u]) {
                int v = edge.to;
                double new_dist = dist_b[u] + edge.weight;
                
                if (new_dist < dist_b[v]) {
                    dist_b[v] = new_dist;
                    pq_b.push({new_dist, v});
                    
                    if (dist_f[v] < INF_DOUBLE) {
                        best_dist = min(best_dist, dist_f[v] + dist_b[v]);
                    }
                }
            }
        }
    }
    
    return best_dist;
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
        
        if (u < 0 || u >= N || v < 0 || v >= N || w < 0.0) {
            cerr << "Erro: valores inválidos na aresta " << (i + 1) << endl;
            return 1;
        }
        
        graph_fwd[u].emplace_back(v, w);
        graph_fwd[v].emplace_back(u, w);
        
        graph_bwd[v].emplace_back(u, w);
        graph_bwd[u].emplace_back(v, w);
    }
    
    auto start_time = high_resolution_clock::now();
    
    double result = biDijkstra(graph_fwd, graph_bwd, N, S, D);
    
    auto end_time = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end_time - start_time);
    
    if (result == INF_DOUBLE) {
        cout << "INF" << endl;
    } else {
        cout << fixed << setprecision(3) << result << endl;
    }
    
    cerr << duration.count() << endl;
    
    return 0;
}