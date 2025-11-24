#!/usr/bin/env python3
"""
benchmark_runner.py - Script de Benchmark para Algoritmos de Caminho Mais Curto

Este script executa todos os 5 algoritmos em todos os 10 casos de teste,
mede o tempo de execução, calcula diferenças e gera relatórios e gráficos.
"""

import subprocess
import csv
import os
import sys
import matplotlib.pyplot as plt
import pandas as pd
import time

def get_time_unit_and_divisor(max_time_microseconds):
    """
    Determina a unidade de tempo mais apropriada para visualização
    """
    if max_time_microseconds < 1000:  # < 1ms
        return "μs", 1, "Microssegundos"
    elif max_time_microseconds < 1000000:  # < 1s
        return "ms", 1000, "Milissegundos"
    else:
        return "s", 1000000, "Segundos"

def read_expected_result(test_file):
    """
    Lê o resultado esperado do arquivo de saída
    """
    try:
        with open(test_file, 'r') as f:
            return float(f.read().strip())
    except Exception as e:
        print(f"Erro ao ler {test_file}: {e}")
        return None

def run_algorithm(executable, input_file, is_python=False, venv_path=None):
    """
    Executa um algoritmo e captura resultado e tempo
    """
    try:
        if is_python:
            if venv_path:
                cmd = [f"{venv_path}\\Scripts\\python.exe", executable]
            else:
                cmd = ["python", executable]
        else:
            cmd = [executable]
        
        with open(input_file, 'r') as f:
            result = subprocess.run(
                cmd,
                stdin=f,
                capture_output=True,
                text=True,
                timeout=300  # timeout de 5 minutos (300s)
            )
        
        if result.returncode != 0:
            print(f"Erro executando {executable}: {result.stderr}")
            return None, None
            
        # Output vai para stdout, tempo vai para stderr
        output = result.stdout.strip()
        time_stderr = result.stderr.strip()
        
        # Para Gurobi/Python, pode haver mensagens de licença no stdout também
        # Vamos filtrar essas mensagens do resultado
        output_lines = output.split('\n')
        clean_output_lines = []
        for line in output_lines:
            line = line.strip()
            if ('Restricted license' in line or 
                'expires' in line or 
                'for non-production use only' in line or
                line == ''):
                continue
            clean_output_lines.append(line)
        
        # O resultado deve estar na última linha limpa
        if clean_output_lines:
            output = clean_output_lines[-1]
        else:
            print(f"    Erro: nenhum resultado válido encontrado")
            return None, None
        
        # Processa o tempo 
        # Para Python/Gurobi, filtra mensagens de licença e pega apenas números
        time_lines = []
        for line in time_stderr.split('\n'):
            line = line.strip()
            # Ignora mensagens conhecidas do Gurobi
            if ('Restricted license' in line or 
                'expires' in line or 
                'for non-production use only' in line or
                line == ''):
                continue
            # Se a linha contém apenas dígitos, é o tempo
            if line.isdigit():
                time_lines.append(line)
        
        if time_lines:
            execution_time = int(time_lines[-1])  # em microssegundos
        else:
            print(f"    Aviso: tempo não encontrado para {executable}")
            print(f"    stderr original: '{time_stderr}'")
            print(f"    stderr linhas: {time_stderr.split('\\n')}")
            execution_time = 0
            
        # Processa o resultado
        if output == "INF":
            return float('inf'), execution_time
        else:
            return float(output), execution_time
            
    except subprocess.TimeoutExpired:
        print(f"    Timeout executando {executable} (>5min)")
        return "TIMEOUT", 300000000  # 5 minutos em microssegundos
    except Exception as e:
        print(f"Erro executando {executable}: {e}")
        return None, None

def calculate_differences(expected, obtained):
    """
    Calcula diferença absoluta e percentual entre valores
    """
    if expected is None or obtained is None:
        return None, None
        
    if expected == float('inf') and obtained == float('inf'):
        return 0.0, 0.0
        
    if expected == float('inf') or obtained == float('inf'):
        return float('inf'), float('inf')
        
    abs_diff = abs(expected - obtained)
    
    if expected == 0:
        percent_diff = 0.0 if abs_diff == 0 else float('inf')
    else:
        percent_diff = (abs_diff / abs(expected)) * 100
        
    return abs_diff, percent_diff

def create_comprehensive_comparison_plot(all_results, output_dir):
    """
    Cria gráfico único com todos os algoritmos e todas as instâncias
    Cada algoritmo terá pontos conectados por linhas mostrando seu desempenho em cada instância
    """
    plt.figure(figsize=(16, 10))
    
    # Define cores e marcadores para cada algoritmo
    algorithm_styles = {
        'Dijkstra Clássico': {'color': '#1f77b4', 'marker': 'o', 'label': 'Dijkstra Clássico'},
        'Dijkstra Bidirecional': {'color': '#ff7f0e', 'marker': 's', 'label': 'Dijkstra Bidirecional'},
        'Dial Dijkstra': {'color': '#2ca02c', 'marker': '^', 'label': 'Dial Dijkstra'},
        'BiDial Dijkstra': {'color': '#d62728', 'marker': 'D', 'label': 'BiDial Dijkstra'},
        'Programação Linear': {'color': '#9467bd', 'marker': 'v', 'label': 'Programação Linear'}
    }
    
    # Organiza dados por algoritmo, mantendo ordem dos test cases
    algorithm_data = {}
    # Garante ordem correta dos test cases (arq01 a arq10)
    ordered_test_cases = [f'arq{i:02d}' for i in range(1, 11)]
    
    for test_case in ordered_test_cases:
        if test_case in all_results:
            results = all_results[test_case]
            for alg_name, time_us in results.items():
                if alg_name not in algorithm_data:
                    algorithm_data[alg_name] = {'test_cases': [], 'times': []}
                algorithm_data[alg_name]['test_cases'].append(test_case)
                algorithm_data[alg_name]['times'].append(time_us)
    
    # Converte todos os tempos para milissegundos
    divisor = 1000  # microsegundos -> milissegundos
    
    # Plota cada algoritmo
    for alg_name, data in algorithm_data.items():
        if alg_name in algorithm_styles:
            style = algorithm_styles[alg_name]
            # Converte tempos para milissegundos
            # Mantém zero como zero (não filtra)
            # Filtra apenas timeouts (> 60 segundos = 60000000 us)
            times_ms = []
            test_cases_nums = []
            
            for i, (tc, time_us) in enumerate(zip(data['test_cases'], data['times'])):
                # Pula apenas timeouts (> 60000ms = 60s)
                if time_us < 60000000:  # não é timeout
                    times_ms.append(time_us / divisor)  # converte para ms, mantém 0 como 0
                    test_cases_nums.append(int(tc.replace('arq', '')))
            
            if times_ms:
                plt.plot(test_cases_nums, times_ms, 
                        color=style['color'], 
                        marker=style['marker'], 
                        markersize=10,
                        linewidth=2.5,
                        label=style['label'],
                        alpha=0.8)
    
    plt.title('Comparação de Desempenho: Todos os Algoritmos em Todas as Instâncias', 
             fontsize=18, fontweight='bold', pad=20)
    plt.xlabel('Instância de Teste', fontsize=14, fontweight='bold')
    plt.ylabel('Tempo de Execução (ms)', fontsize=14, fontweight='bold')
    plt.legend(fontsize=12, loc='best', framealpha=0.9)
    plt.grid(True, alpha=0.3, linestyle='--')
    
    # Configura eixo X para mostrar 1 a 10 em ordem
    plt.xticks(range(1, 11), fontsize=11)
    plt.xlim(0.5, 10.5)
    
    # Configura eixo Y para começar em 0
    plt.ylim(bottom=0)
    plt.yticks(fontsize=11)
    
    plt.tight_layout()
    
    plot_path = os.path.join(output_dir, 'comparison_all_algorithms.png')
    plt.savefig(plot_path, dpi=300, bbox_inches='tight')
    plt.close()
    
    print(f"\nGráfico geral salvo em: {plot_path}")

def run_benchmark():
    """
    Executa o benchmark completo
    """
    # Configurações
    base_dir = os.path.dirname(os.path.abspath(__file__))
    venv_path = os.path.join(base_dir, '.venv')
    
    algorithms = [
        ('Dijkstra Clássico', 'dijkstra_benchmark.exe', False),
        ('Dijkstra Bidirecional', 'biDijkstra_benchmark.exe', False),
        ('Dial Dijkstra', 'dialDijkstra_benchmark.exe', False),
        ('BiDial Dijkstra', 'biDialDijkstra_benchmark.exe', False),
        ('Programação Linear', 'plShortestPath_benchmark.py', True)
    ]
    
    test_cases = [f'arq{i:02d}' for i in range(1, 11)]
    
    # Estrutura para armazenar todos os resultados para o gráfico geral
    all_results = {}  # {test_case: {algorithm_name: time_us}}
    
    print("=== INICIANDO BENCHMARK ===")
    print(f"Testando {len(algorithms)} algoritmos em {len(test_cases)} casos de teste")
    print()
    
    # Para cada caso de teste
    for test_case in test_cases:
        all_results[test_case] = {}
        print(f"Processando {test_case}...")
        
        input_file = os.path.join(base_dir, 'input', f'{test_case}.in')
        output_file = os.path.join(base_dir, 'output', f'{test_case}.out')
        result_dir = os.path.join(base_dir, 'results', test_case)
        
        if not os.path.exists(input_file):
            print(f"  Erro: arquivo de entrada {input_file} não encontrado")
            continue
            
        if not os.path.exists(output_file):
            print(f"  Erro: arquivo de saída {output_file} não encontrado")
            continue
            
        # Lê resultado esperado
        expected_result = read_expected_result(output_file)
        if expected_result is None:
            print(f"  Erro: não foi possível ler resultado esperado")
            continue
            
        # Dados para a tabela
        table_data = []
        
        # Executa cada algoritmo
        for alg_name, alg_executable, is_python in algorithms:
            print(f"  Executando {alg_name}...")
            
            executable_path = os.path.join(base_dir, alg_executable)
            if not os.path.exists(executable_path):
                print(f"    Erro: executável {executable_path} não encontrado")
                table_data.append([
                    alg_name, expected_result, "N/A", "N/A", "N/A", "N/A"
                ])
                continue
                
            # Executa algoritmo
            obtained_result, execution_time = run_algorithm(
                executable_path, input_file, is_python, 
                venv_path if is_python else None
            )
            
            if obtained_result is None or obtained_result == "TIMEOUT":
                if obtained_result == "TIMEOUT":
                    table_data.append([
                        alg_name,
                        expected_result,
                        "TIMEOUT (>5min)",
                        "N/A",
                        "N/A",
                        execution_time
                    ])
                    print(f"    Resultado: TIMEOUT (>5min)")
                else:
                    print(f"    Erro: execução falhou")
                    table_data.append([
                        alg_name, expected_result, "N/A", "N/A", "N/A", "N/A"
                    ])
                continue
                
            # Calcula diferenças
            abs_diff, percent_diff = calculate_differences(expected_result, obtained_result)
            
            # Adiciona à tabela
            table_data.append([
                alg_name,
                expected_result,
                obtained_result,
                abs_diff if abs_diff is not None else "N/A",
                f"{percent_diff:.6f}%" if percent_diff is not None and percent_diff != float('inf') else "N/A",
                execution_time
            ])
            
            # Armazena tempo para o gráfico geral
            all_results[test_case][alg_name] = execution_time
            
            print(f"    Resultado: {obtained_result}")
        
        # Salva tabela CSV
        csv_path = os.path.join(result_dir, f'{test_case}_results.csv')
        os.makedirs(result_dir, exist_ok=True)
        
        with open(csv_path, 'w', newline='', encoding='utf-8') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([
                'Algoritmo',
                'Resultado_Esperado', 
                'Resultado_Obtido',
                'Diferenca_Absoluta',
                'Diferenca_Percentual',
                'Tempo_Execucao_us'
            ])
            writer.writerows(table_data)
        
        print(f"  Tabela salva em: {csv_path}")
        print()
    
    # Cria gráfico geral com todos os resultados
    print("\nGerando gráfico geral de comparação...")
    results_dir = os.path.join(base_dir, 'results')
    create_comprehensive_comparison_plot(all_results, results_dir)
    
    print("\n=== BENCHMARK CONCLUÍDO ===")

if __name__ == "__main__":
    # Verifica dependências
    try:
        import matplotlib.pyplot as plt
        import pandas as pd
    except ImportError as e:
        print(f"Erro: {e}")
        print("Instale as dependências: pip install matplotlib pandas")
        sys.exit(1)
    
    run_benchmark()