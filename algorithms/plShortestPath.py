#!/usr/bin/env python3
"""
plShortestPath.py - Solução do Caminho Mais Curto via Programação Linear

DESCRIÇÃO:
Resolve o problema do caminho mais curto usando programação linear.
O problema é modelado como um problema de fluxo de custo mínimo onde:
- Queremos enviar 1 unidade de fluxo da origem para o destino
- Cada aresta tem um custo igual ao seu peso
- Minimizamos o custo total do fluxo

FORMULAÇÃO MATEMÁTICA:
Variáveis:
- x_e ∈ [0,1] para cada aresta e (fluxo na aresta)

Objetivo:
- minimizar Σ(peso_e * x_e) para todas as arestas e

Restrições:
- Para vértice origem S: Σ(x_out) - Σ(x_in) = 1
- Para vértice destino D: Σ(x_out) - Σ(x_in) = -1  
- Para outros vértices v: Σ(x_out) - Σ(x_in) = 0
- x_e >= 0 para todas as arestas

ENTRADA (stdin):
- Primeira linha: N M S D
  N = número de vértices (0..N-1)
  M = número de arestas
  S = vértice origem
  D = vértice destino
- Próximas M linhas: U V W
  U = vértice origem da aresta
  V = vértice destino da aresta  
  W = peso da aresta (double)

SAÍDA (stdout):
- Um único número: distância mínima de S a D
- Ou "INF" se não existir caminho

NOTA:
- O grafo é tratado como NÃO DIRECIONADO (cada aresta U V cria U->V e V->U)
- Usa precisão double mantendo a máxima fidelidade dos pesos

DEPENDÊNCIAS:
- pip install gurobipy

COMPILAÇÃO/EXECUÇÃO:
python plShortestPath.py < input/arquivo.in
"""

import sys
import gurobipy as gp
from gurobipy import GRB


def solve_shortest_path_lp():
    """
    Resolve o problema do caminho mais curto usando programação linear
    """
    
    try:
        # LEITURA OTIMIZADA: Lê tudo de uma vez em memória
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
        
        # Validação dos parâmetros
        if N <= 0 or M < 0 or S < 0 or S >= N or D < 0 or D >= N:
            print("Erro: parâmetros inválidos", file=sys.stderr)
            return 1
        
        # Se origem e destino são iguais
        if S == D:
            print("0")
            return 0
        
        # OTIMIZAÇÃO: Estrutura de adjacência e pesos separados
        # Usa dicionário para vizinhos e pesos (acesso O(1))
        adj = [[] for _ in range(N)]  # adj[u] = lista de vizinhos de u
        weights = {}  # weights[(u,v)] = peso da aresta u->v
        directed_edges = []  # Lista de arestas direcionadas para criar variáveis
        
        # Processamento das arestas
        for _ in range(M):
            try:
                u = int(next(iterator))
                v = int(next(iterator))
                w = float(next(iterator))
            except (StopIteration, ValueError):
                print("Erro: formato inválido nas arestas", file=sys.stderr)
                return 1
            
            # Validação
            if u < 0 or u >= N or v < 0 or v >= N or w < 0.0:
                print(f"Erro: valores inválidos na aresta ({u}, {v}, {w})", file=sys.stderr)
                return 1
            
            # Grafo não direcionado: adiciona ambas as direções
            adj[u].append(v)
            adj[v].append(u)
            weights[(u, v)] = w
            weights[(v, u)] = w
            directed_edges.append((u, v))
            directed_edges.append((v, u))
        
        # Cria o modelo de otimização
        model = gp.Model("shortest_path")
        
        # Configurações do Gurobi otimizadas
        model.setParam('OutputFlag', 0)
        model.setParam('LogToConsole', 0)
        model.setParam('Method', 0)  # Método Simplex (mais rápido para esse tipo de problema)
        model.setParam('TimeLimit', 300)  # Timeout de 5 minutos
        
        # OTIMIZAÇÃO: Criação de variáveis usando dicionário
        x = {}
        for u, v in directed_edges:
            x[u, v] = model.addVar(vtype=GRB.CONTINUOUS, lb=0.0, name=f"x_{u}_{v}")
        
        # Define função objetivo usando quicksum
        obj = gp.quicksum(x[u, v] * weights[u, v] for u, v in directed_edges)
        model.setObjective(obj, GRB.MINIMIZE)
        
        # CONSTRUÇÃO OTIMIZADA DAS RESTRIÇÕES - Usa lista de adjacência
        for node in range(N):
            # Fluxo que sai: soma de x[node, vizinho]
            outflow = gp.quicksum(x[node, neighbor] for neighbor in adj[node])
            # Fluxo que entra: soma de x[vizinho, node]
            inflow = gp.quicksum(x[neighbor, node] for neighbor in adj[node])
            
            if node == S:
                model.addConstr(outflow - inflow == 1, f"source_{node}")
            elif node == D:
                model.addConstr(outflow - inflow == -1, f"dest_{node}")
            else:
                model.addConstr(outflow - inflow == 0, f"flow_{node}")
        
        # Resolve o modelo
        model.optimize()
        
        # Verifica o status da solução
        if model.status == GRB.OPTIMAL:
            # Solução ótima encontrada
            optimal_cost = model.objVal
            
            # Verifica se é muito próximo de um inteiro
            if abs(optimal_cost - round(optimal_cost)) < 1e-9:
                # É um inteiro, imprime sem casas decimais
                print(f"{int(round(optimal_cost))}")
            else:
                # Tem parte decimal, imprime com precisão de 3 casas decimais
                print(f"{optimal_cost:.3f}")
            return 0
            
        elif model.status == GRB.INFEASIBLE:
            # Não existe caminho
            print("INF")
            return 0
            
        elif model.status == GRB.UNBOUNDED:
            # Problema mal formulado (não deveria acontecer neste caso)
            print("Erro: problema mal formulado (ilimitado)", file=sys.stderr)
            return 1
            
        else:
            # Outros estados (erro, interrompido, etc.)
            print(f"Erro: status de otimização inesperado: {model.status}", file=sys.stderr)
            return 1
            
    except Exception as e:
        print(f"Erro inesperado: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(solve_shortest_path_lp())