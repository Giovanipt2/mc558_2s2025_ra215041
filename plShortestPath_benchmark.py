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
        # Leitura e construção do modelo (NÃO incluso no benchmark)
        first_line = sys.stdin.readline().strip()
        if not first_line:
            print("Erro: não foi possível ler a primeira linha", file=sys.stderr)
            return 1
            
        parts = first_line.split()
        if len(parts) != 4:
            print("Erro: formato inválido na primeira linha", file=sys.stderr)
            return 1
            
        N, M, S, D = map(int, parts)
        
        if N <= 0 or M < 0 or S < 0 or S >= N or D < 0 or D >= N:
            print("Erro: parâmetros inválidos", file=sys.stderr)
            return 1
            
        edges = []
        
        for i in range(M):
            line = sys.stdin.readline().strip()
            if not line:
                print(f"Erro: número insuficiente de arestas na linha {i+1}", file=sys.stderr)
                return 1
                
            parts = line.split()
            if len(parts) != 3:
                print(f"Erro: formato inválido na aresta {i+1}", file=sys.stderr)
                return 1
                
            try:
                u, v = int(parts[0]), int(parts[1])
                w = float(parts[2])
            except ValueError:
                print(f"Erro: valores inválidos na aresta {i+1}", file=sys.stderr)
                return 1
                
            if u < 0 or u >= N or v < 0 or v >= N or w < 0.0:
                print(f"Erro: valores inválidos na aresta {i+1}", file=sys.stderr)
                return 1
                
            # Como o grafo é não direcionado, adicionamos ambas as direções
            edges.append((u, v, w))
            if u != v:  # Evita arestas duplicadas para self-loops
                edges.append((v, u, w))
        
        if S == D:
            print("0")
            print("0", file=sys.stderr)  # Tempo zero
            return 0
            
        # Cria o modelo de otimização
        model = gp.Model("shortest_path")
        model.setParam('OutputFlag', 0)
        model.setParam('LogToConsole', 0)
        
        # Configurações agressivas para instâncias grandes
        model.setParam('TimeLimit', 120)      # Limite de 2 minutos  
        model.setParam('Threads', 6)          # Usar mais threads
        model.setParam('MIPFocus', 1)         # Foco em encontrar soluções factíveis
        model.setParam('Heuristics', 0.9)     # Mais heurísticas
        model.setParam('Cuts', 3)             # Cortes mais agressivos
        model.setParam('Presolve', 2)         # Presolve mais agressivo
        
        # Cria variáveis de fluxo para cada aresta
        flow_vars = {}
        for i, (u, v, w) in enumerate(edges):
            var_name = f"x_{u}_{v}_{i}"
            flow_vars[(u, v, i)] = model.addVar(
                lb=0.0,
                ub=1.0,
                obj=w,
                vtype=GRB.CONTINUOUS,
                name=var_name
            )
        
        # Restrições de conservação de fluxo
        for vertex in range(N):
            flow_balance = gp.LinExpr()
            
            for i, (u, v, w) in enumerate(edges):
                if u == vertex:
                    flow_balance += flow_vars[(u, v, i)]
            
            for i, (u, v, w) in enumerate(edges):
                if v == vertex:
                    flow_balance -= flow_vars[(u, v, i)]
            
            if vertex == S:
                model.addConstr(flow_balance == 1, f"flow_balance_{vertex}")
            elif vertex == D:
                model.addConstr(flow_balance == -1, f"flow_balance_{vertex}")
            else:
                model.addConstr(flow_balance == 0, f"flow_balance_{vertex}")
        
        model.setObjective(
            gp.quicksum(w * flow_vars[(u, v, i)] for i, (u, v, w) in enumerate(edges)),
            GRB.MINIMIZE
        )
        
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