/**
 * biDialDijkstra_benchmark.cpp - Dijkstra Bidirecional com Dial com Medição de Tempo
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

long long biDialDijkstra(const Graph& graph_fwd, const Graph& graph_bwd, int N, int S, int D, int C) {
    vector<long long> dist_f(N, INF_LL);
    vector<long long> dist_b(N, INF_LL);
    
    dist_f[S] = 0;
    dist_b[D] = 0;
    
    long long max_dist = (long long)N * C;
    vector<vector<int>> buckets_f(max_dist + 1);
    vector<vector<int>> buckets_b(max_dist + 1);
    
    buckets_f[0].push_back(S);
    buckets_b[0].push_back(D);
    
    vector<bool> visited_f(N, false);
    vector<bool> visited_b(N, false);
    
    long long best_dist = INF_LL;
    
    long long current_f = 0;
    long long current_b = 0;
    
    bool process_forward = true;
    
    while (current_f <= max_dist || current_b <= max_dist) {
        bool has_forward = false;
        bool has_backward = false;
        
        while (current_f <= max_dist && buckets_f[current_f].empty()) {
            current_f++;
        }
        if (current_f <= max_dist) {
            has_forward = true;
        }
        
        while (current_b <= max_dist && buckets_b[current_b].empty()) {
            current_b++;
        }
        if (current_b <= max_dist) {
            has_backward = true;
        }
        
        if (!has_forward && !has_backward) {
            break;
        }
        
        if (best_dist < INF_LL) {
            if ((!has_forward || current_f >= best_dist) && 
                (!has_backward || current_b >= best_dist)) {
                break;
            }
        }
        
        if (has_forward && (process_forward || !has_backward)) {
            int u = buckets_f[current_f].back();
            buckets_f[current_f].pop_back();
            
            if (visited_f[u]) {
                continue;
            }
            visited_f[u] = true;
            
            if (dist_b[u] < INF_LL) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            for (const Edge& edge : graph_fwd[u]) {
                int v = edge.to;
                long long new_dist = dist_f[u] + edge.weight;
                
                if (new_dist < dist_f[v]) {
                    dist_f[v] = new_dist;
                    
                    if (new_dist <= max_dist) {
                        buckets_f[new_dist].push_back(v);
                    }
                    
                    if (dist_b[v] < INF_LL) {
                        best_dist = min(best_dist, dist_f[v] + dist_b[v]);
                    }
                }
            }
            
            process_forward = false;
        }
        else if (has_backward) {
            int u = buckets_b[current_b].back();
            buckets_b[current_b].pop_back();
            
            if (visited_b[u]) {
                continue;
            }
            visited_b[u] = true;
            
            if (dist_f[u] < INF_LL) {
                best_dist = min(best_dist, dist_f[u] + dist_b[u]);
            }
            
            for (const Edge& edge : graph_bwd[u]) {
                int v = edge.to;
                long long new_dist = dist_b[u] + edge.weight;
                
                if (new_dist < dist_b[v]) {
                    dist_b[v] = new_dist;
                    
                    if (new_dist <= max_dist) {
                        buckets_b[new_dist].push_back(v);
                    }
                    
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
        
        graph_fwd[u].emplace_back(v, w);
        graph_fwd[v].emplace_back(u, w);
        
        graph_bwd[v].emplace_back(u, w);
        graph_bwd[u].emplace_back(v, w);
        
        C = max(C, w);
    }
    
    if (C > 10000000) {
        cerr << "Aviso: maior peso C=" << C << " é muito grande" << endl;
    }
    
    auto start_time = high_resolution_clock::now();
    
    long long result = biDialDijkstra(graph_fwd, graph_bwd, N, S, D, C);
    
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