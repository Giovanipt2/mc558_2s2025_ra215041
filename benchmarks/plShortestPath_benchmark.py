#!/usr/bin/env python3
"""
plShortestPath_benchmark.py - Solução do Caminho Mais Curto via Programação Linear com Benchmark

Versão modificada do plShortestPath.py que inclui medição precisa do tempo
de execução do algoritmo, excluindo o tempo de leitura da entrada.
"""

import sys
import gurobipy as gp
from gurobipy import GRB
import time
import os


def solve_shortest_path_lp():
    """
    Resolve o problema do caminho mais curto usando programação linear
    """
    
    try:
        # LEITURA OTIMIZADA: Lê tudo de uma vez em memória (NÃO incluso no benchmark)
        input_data = sys.stdin.read().split()
        if not input_data:
            print("Erro: entrada vazia", file=sys.stderr)
            return 1
            
        iterator = iter(input_data)
        
        try:
            N = int(next(iterator))
            M = int(next(iterator))
            S = int(next(iterator))
            D = int(next(iterator))
        except (StopIteration, ValueError):
            print("Erro: formato inválido no cabeçalho", file=sys.stderr)
            return 1
        
        if N <= 0 or M < 0 or S < 0 or S >= N or D < 0 or D >= N:
            print("Erro: parâmetros inválidos", file=sys.stderr)
            return 1
        
        if S == D:
            print("0")
            print("0", file=sys.stderr)  # Tempo zero
            return 0
        
        # OTIMIZAÇÃO: Estrutura de adjacência e pesos separados
        adj = [[] for _ in range(N)]  # adj[u] = lista de vizinhos de u
        weights = {}  # weights[(u,v)] = peso da aresta u->v
        directed_edges = []  # Lista de arestas direcionadas
        
        # Processamento das arestas
        for _ in range(M):
            try:
                u = int(next(iterator))
                v = int(next(iterator))
                w = float(next(iterator))
            except (StopIteration, ValueError):
                print("Erro: formato inválido nas arestas", file=sys.stderr)
                return 1
            
            if u < 0 or u >= N or v < 0 or v >= N or w < 0.0:
                print(f"Erro: valores inválidos na aresta ({u}, {v}, {w})", file=sys.stderr)
                return 1
            
            # Grafo não direcionado
            adj[u].append(v)
            adj[v].append(u)
            weights[(u, v)] = w
            weights[(v, u)] = w
            directed_edges.append((u, v))
            directed_edges.append((v, u))
        
        # Cria o modelo de otimização
        model = gp.Model("shortest_path")
        model.setParam('OutputFlag', 0)
        model.setParam('LogToConsole', 0)
        model.setParam('Method', 0)  # Método Simplex
        model.setParam('TimeLimit', 300)  # 5 minutos
        
        # Criação de variáveis
        x = {}
        for u, v in directed_edges:
            x[u, v] = model.addVar(vtype=GRB.CONTINUOUS, lb=0.0, name=f"x_{u}_{v}")
        
        # Função objetivo
        obj = gp.quicksum(x[u, v] * weights[u, v] for u, v in directed_edges)
        model.setObjective(obj, GRB.MINIMIZE)
        
        # Restrições de fluxo
        for node in range(N):
            outflow = gp.quicksum(x[node, neighbor] for neighbor in adj[node])
            inflow = gp.quicksum(x[neighbor, node] for neighbor in adj[node])
            
            if node == S:
                model.addConstr(outflow - inflow == 1, f"source_{node}")
            elif node == D:
                model.addConstr(outflow - inflow == -1, f"dest_{node}")
            else:
                model.addConstr(outflow - inflow == 0, f"flow_{node}")
        
        # INÍCIO DO BENCHMARK - medição do tempo do algoritmo apenas
        start_time = time.perf_counter()
        
        model.optimize()
        
        end_time = time.perf_counter()
        # FIM DO BENCHMARK
        
        # Calcula tempo em microssegundos
        duration_microseconds = int((end_time - start_time) * 1_000_000)
        
        # Verifica o status da solução
        if model.status == GRB.OPTIMAL:
            optimal_cost = model.objVal
            
            if abs(optimal_cost - round(optimal_cost)) < 1e-9:
                print(f"{int(round(optimal_cost))}")
            else:
                print(f"{optimal_cost:.3f}")
                
            # Saída do tempo (para stderr)
            print(duration_microseconds, file=sys.stderr)
            return 0
            
        elif model.status == GRB.INFEASIBLE:
            print("INF")
            print(duration_microseconds, file=sys.stderr)
            return 0
            
        else:
            print(f"Erro: status de otimização inesperado: {model.status}", file=sys.stderr)
            return 1
            
    except Exception as e:
        print(f"Erro inesperado: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(solve_shortest_path_lp())