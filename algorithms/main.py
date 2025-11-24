"""
main.py - Orquestrador Interativo para Avaliação dos Algoritmos de Caminho Mínimo

Este script permite que avaliadores testem os algoritmos de forma interativa,
fornecendo entrada via stdin e recebendo apenas a distância mínima como saída.
"""

import subprocess
import sys
import os
from pathlib import Path

def print_header():
    """Imprime cabeçalho do programa"""
    print("\n" + "="*70)
    print("  ALGORITMOS DE CAMINHO MÍNIMO - MC558")
    print("="*70)

def get_input_data():
    """
    Solicita entrada do usuário no formato especificado:
    - Primeira linha: N M S D
    - Próximas M linhas: U V W
    """
    print("\n--- ENTRADA DE DADOS ---")
    print("Digite a entrada conforme especificado:")
    print("  Linha 1: N M S D")
    print("  Próximas M linhas: U V W\n")
    
    # Lê primeira linha
    try:
        first_line = input()
    except EOFError:
        print("\nErro: Nenhuma entrada fornecida.")
        return None
    
    # Valida primeira linha
    try:
        parts = first_line.strip().split()
        if len(parts) != 4:
            print("\nErro: Primeira linha deve conter exatamente 4 valores (N M S D).")
            return None
        N, M, S, D = map(int, parts)
    except ValueError:
        print("\nErro: Formato inválido na primeira linha.")
        return None
    
    # Lê as M linhas de arestas
    lines = [first_line]
    for i in range(M):
        try:
            line = input()
            lines.append(line)
        except EOFError:
            print(f"\nErro: Esperadas {M} arestas, mas recebeu apenas {i}.")
            return None
    
    return "\n".join(lines)

def select_algorithm():
    """
    Permite ao usuário selecionar qual algoritmo executar
    """
    print("\n--- SELEÇÃO DE ALGORITMO ---")
    print("Digite o número do algoritmo que deseja utilizar:")
    print("[1] Dijkstra Clássico")
    print("[2] Dijkstra Bidirecional")
    print("[3] Dial Dijkstra")
    print("[4] BiDial Dijkstra")
    print("[5] Programação Linear (Gurobi)")
    
    algorithms = {
        '1': ('dijkstra.exe', False, 'Dijkstra Clássico'),
        '2': ('biDijkstra.exe', False, 'Dijkstra Bidirecional'),
        '3': ('dialDijkstra.exe', False, 'Dial Dijkstra'),
        '4': ('biDialDijkstra.exe', False, 'BiDial Dijkstra'),
        '5': ('plShortestPath.py', True, 'Programação Linear')
    }
    
    while True:
        choice = input("\nSua escolha: ").strip()
        if choice in algorithms:
            return algorithms[choice]
        else:
            print("Opção inválida. Digite um número entre 1 e 5.")

def run_algorithm(executable, is_python, input_data):
    """
    Executa o algoritmo selecionado com a entrada fornecida
    Retorna apenas o resultado (distância mínima) ou mensagem de timeout
    """
    # base_dir aponta para algorithms/, então precisamos subir um nível para acessar .venv
    algorithms_dir = Path(__file__).parent
    base_dir = algorithms_dir.parent
    
    if is_python:
        venv_python = base_dir / '.venv' / 'Scripts' / 'python.exe'
        if venv_python.exists():
            cmd = [str(venv_python), str(algorithms_dir / executable)]
        else:
            cmd = ['python', str(algorithms_dir / executable)]
    else:
        cmd = [str(algorithms_dir / executable)]
    
    try:
        result = subprocess.run(
            cmd,
            input=input_data,
            capture_output=True,
            text=True,
            timeout=300  # 5 minutos para PL
        )
        
        if result.returncode != 0:
            return f"ERRO: {result.stderr.strip()}"
        
        # Filtra mensagens do Gurobi se for Python
        output_lines = result.stdout.strip().split('\n')
        clean_lines = []
        for line in output_lines:
            line = line.strip()
            if ('Restricted license' not in line and 
                'expires' not in line and 
                'for non-production use only' not in line and
                line != ''):
                clean_lines.append(line)
        
        if clean_lines:
            return clean_lines[-1]  # Última linha com resultado
        else:
            return "ERRO: Nenhum resultado obtido"
            
    except subprocess.TimeoutExpired:
        return "TIMEOUT (>5min)"
    except Exception as e:
        return f"ERRO: {str(e)}"

def ask_continue():
    """
    Pergunta se o usuário deseja continuar testando
    """
    print("\n--- CONTINUAR? ---")
    print("[1] Sim, quero continuar testando")
    print("[2] Não, quero sair")
    
    while True:
        choice = input("\nSua escolha: ").strip()
        if choice == '1':
            return True
        elif choice == '2':
            return False
        else:
            print("Opção inválida. Digite 1 ou 2.")

def main():
    """
    Loop principal do programa
    """
    print_header()
    
    while True:
        # 1. Obter entrada
        input_data = get_input_data()
        if input_data is None:
            if not ask_continue():
                break
            continue
        
        # 2. Selecionar algoritmo
        executable, is_python, alg_name = select_algorithm()
        
        # 3. Executar algoritmo
        print(f"\nExecutando {alg_name}...")
        result = run_algorithm(executable, is_python, input_data)
        
        # 4. Exibir resultado (APENAS a distância)
        print("\n" + "="*70)
        print(f"RESULTADO: {result}")
        print("="*70)
        
        # 5. Perguntar se continua
        if not ask_continue():
            break
    
    print("\nEncerrando programa. Obrigado por usar o sistema!")
    print("="*70 + "\n")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\nPrograma interrompido pelo usuário.")
        sys.exit(0)
