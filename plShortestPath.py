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
        # Lê a primeira linha: N M S D
        first_line = sys.stdin.readline().strip()
        if not first_line:
            print("Erro: não foi possível ler a primeira linha", file=sys.stderr)
            return 1
            
        parts = first_line.split()
        if len(parts) != 4:
            print("Erro: formato inválido na primeira linha", file=sys.stderr)
            return 1
            
        N, M, S, D = map(int, parts)
        
        # Validação dos parâmetros
        if N <= 0 or M < 0 or S < 0 or S >= N or D < 0 or D >= N:
            print("Erro: parâmetros inválidos", file=sys.stderr)
            return 1
            
        # Lista para armazenar as arestas (será expandida para grafo não direcionado)
        edges = []
        
        # Lê as arestas
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
                
            # Validação dos vértices
            if u < 0 or u >= N or v < 0 or v >= N:
                print(f"Erro: vértices inválidos na aresta {i+1}", file=sys.stderr)
                return 1
                
            # Validação do peso
            if w < 0.0:
                print(f"Erro: peso negativo na aresta {i+1}", file=sys.stderr)
                return 1
                
            # Como o grafo é não direcionado, adicionamos ambas as direções
            edges.append((u, v, w))
            if u != v:  # Evita arestas duplicadas para self-loops
                edges.append((v, u, w))
        
        # Se origem e destino são iguais
        if S == D:
            print("0")
            return 0
            
        # Cria o modelo de otimização
        model = gp.Model("shortest_path")
        
        # Suprime output do Gurobi
        model.setParam('OutputFlag', 0)
        model.setParam('LogToConsole', 0)
        
        # Cria variáveis de fluxo para cada aresta
        flow_vars = {}
        for i, (u, v, w) in enumerate(edges):
            var_name = f"x_{u}_{v}_{i}"
            flow_vars[(u, v, i)] = model.addVar(
                lb=0.0,           # Fluxo não negativo
                ub=1.0,           # Máximo 1 unidade de fluxo
                obj=w,            # Coeficiente na função objetivo
                vtype=GRB.CONTINUOUS,
                name=var_name
            )
        
        # Restrições de conservação de fluxo
        for vertex in range(N):
            # Fluxo de saída - fluxo de entrada
            flow_balance = gp.LinExpr()
            
            # Adiciona fluxo de saída (aresta sai do vértice)
            for i, (u, v, w) in enumerate(edges):
                if u == vertex:
                    flow_balance += flow_vars[(u, v, i)]
            
            # Subtrai fluxo de entrada (aresta chega no vértice)  
            for i, (u, v, w) in enumerate(edges):
                if v == vertex:
                    flow_balance -= flow_vars[(u, v, i)]
            
            # Restrição de conservação
            if vertex == S:
                # Origem: deve sair 1 unidade líquida
                model.addConstr(flow_balance == 1, f"flow_balance_{vertex}")
            elif vertex == D:
                # Destino: deve chegar 1 unidade líquida
                model.addConstr(flow_balance == -1, f"flow_balance_{vertex}")
            else:
                # Outros vértices: conservação perfeita (entrada = saída)
                model.addConstr(flow_balance == 0, f"flow_balance_{vertex}")
        
        # Define objetivo: minimizar custo total
        model.setObjective(
            gp.quicksum(w * flow_vars[(u, v, i)] for i, (u, v, w) in enumerate(edges)),
            GRB.MINIMIZE
        )
        
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