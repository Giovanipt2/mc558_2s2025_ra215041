# MC558 - Projeto de Implementação: Algoritmos de Caminho Mais Curto

Este repositório contém a implementação e análise comparativa de cinco algoritmos para o problema de caminho mais curto em grafos não direcionados com pesos não-negativos.

**Disciplina:** MC558 - Projeto e Análise de Algoritmos II  
**Semestre:** 2º Semestre de 2025  
**RA:** 215041

## 📋 Índice

- [Visão Geral](#visão-geral)
- [Estrutura do Repositório](#estrutura-do-repositório)
- [Algoritmos Implementados](#algoritmos-implementados)
- [Requisitos](#requisitos)
- [Compilação](#compilação)
- [Execução Interativa](#execução-interativa)
- [Sistema de Benchmark](#sistema-de-benchmark)
- [Formato de Entrada/Saída](#formato-de-entradasaída)

---

## 🎯 Visão Geral

Este projeto implementa e compara cinco abordagens diferentes para resolver o problema de caminho mais curto:

1. **Dijkstra Clássico** - Implementação padrão com priority queue
2. **Dijkstra Bidirecional** - Busca simultânea da origem e do destino
3. **Dial Dijkstra** - Otimização do Dijkstra usando buckets para pesos inteiros
4. **BiDial Dijkstra** - Combinação da busca bidirecional com Dial
5. **Programação Linear** - Formulação como problema de fluxo de custo mínimo (requer Gurobi)

O projeto oferece duas formas de uso:
- **Modo Interativo** (`main.py`) - Interface para avaliadores testarem os algoritmos
- **Modo Benchmark** (`benchmark_runner.py`) - Sistema completo de avaliação de desempenho com geração de relatórios e gráficos

---

## 📁 Estrutura do Repositório

```
.
├── README.md                      # Este arquivo
├── .gitignore                     # Regras para ignorar arquivos
│
├── main.py                        # Interface interativa para avaliadores
├── benchmark_runner.py            # Sistema de benchmark automatizado
│
├── dijkstra.cpp                   # Algoritmo de Dijkstra clássico
├── biDijkstra.cpp                 # Dijkstra bidirecional
├── dialDijkstra.cpp               # Dial Dijkstra (pesos inteiros)
├── biDialDijkstra.cpp             # BiDial bidirecional
├── plShortestPath.py              # Programação Linear (Gurobi)
│
├── dijkstra_benchmark.cpp         # Versão benchmark do Dijkstra clássico
├── biDijkstra_benchmark.cpp       # Versão benchmark do bidirecional
├── dialDijkstra_benchmark.cpp     # Versão benchmark do Dial
├── biDialDijkstra_benchmark.cpp   # Versão benchmark do BiDial
├── plShortestPath_benchmark.py    # Versão benchmark do PL
│
├── input/                         # Arquivos de entrada (fornecer antes de executar)
│   ├── arq01.in
│   ├── arq02.in
│   └── ... arq10.in
│
├── output/                        # Arquivos com resultados esperados
│   ├── arq01.out
│   ├── arq02.out
│   └── ... arq10.out
│
└── results/                       # Resultados do benchmark (gerado automaticamente)
    ├── arq01/
    │   └── arq01_results.csv
    ├── arq02/
    │   └── arq02_results.csv
    ├── ...
    └── comparison_all_algorithms.png
```

---

## 🔬 Algoritmos Implementados

### 1. Dijkstra Clássico (`dijkstra.cpp`)

- **Estrutura de dados:** Priority queue (min-heap)
- **Pesos:** Double (ponto flutuante)
- **Complexidade:** O((V + E) log V)
- **Uso:** Solução padrão para grafos com pesos não-negativos
- **Característica:** Mantém precisão decimal dos pesos

### 2. Dijkstra Bidirecional (`biDijkstra.cpp`)

- **Estrutura de dados:** Duas priority queues (forward e backward)
- **Pesos:** Double
- **Complexidade:** O((V + E) log V) - mais rápido na prática
- **Uso:** Muito eficiente em grafos grandes
- **Característica:** Encontro das buscas tipicamente ocorre antes de explorar todo o grafo

### 3. Dial Dijkstra (`dialDijkstra.cpp`)

- **Estrutura de dados:** Buckets lineares (array de listas)
- **Pesos:** Inteiros (arredonda doubles para o inteiro mais próximo)
- **Complexidade:** O(V + E + N×C) onde C é o maior peso
- **Uso:** Eficiente quando C é pequeno (C << N)
- **Observação:** Arredondamento pode causar pequenas diferenças nos resultados

### 4. BiDial Dijkstra (`biDialDijkstra.cpp`)

- **Estrutura de dados:** Buckets lineares para ambas direções
- **Pesos:** Inteiros
- **Complexidade:** O(V + E + N×C)
- **Uso:** Combina vantagens de busca bidirecional com buckets
- **Observação:** Pode ser mais lento que algoritmos básicos quando C é grande

### 5. Programação Linear (`plShortestPath.py`)

- **Formulação:** Problema de fluxo de custo mínimo
- **Solver:** Gurobi Optimizer (Python API - gurobipy)
- **Variáveis:** Fluxo em cada aresta x_e ∈ [0, ∞)
- **Restrições:** Conservação de fluxo (entrada - saída = demanda)
- **Objetivo:** Minimizar Σ w_e × x_e
- **Uso:** Validação teórica e comparação de desempenho
- **Observação:** Muito mais lento que algoritmos especializados; timeout de 5 minutos em instâncias grandes

---

## 💻 Requisitos

### Para Algoritmos C++ (Obrigatório)

- **Compilador:** GCC/G++ com suporte a C++17
- **Sistema:** Linux, macOS ou Windows (com MSYS2/MinGW ou WSL)

### Para Programação Linear e Benchmarks (Obrigatório)

- **Python 3.8+** 
- **Ambiente Virtual Python** (recomendado)
- **Bibliotecas Python:**
  - `gurobipy` (Gurobi Python API)
  - `matplotlib` (para gráficos do benchmark)
  - `pandas` (para processamento de dados do benchmark)

### Instalação do Gurobi

O algoritmo de Programação Linear requer o Gurobi Optimizer:

1. **Baixar Gurobi:** https://www.gurobi.com/downloads/
2. **Licença Acadêmica:** https://www.gurobi.com/academia/academic-program-and-licenses/
3. **Instalar gurobipy:**
   ```bash
   # No ambiente virtual do projeto
   python -m pip install gurobipy
   ```

### Configuração do Ambiente Virtual (Recomendado)

```bash
# Criar ambiente virtual
python -m venv .venv

# Ativar (Linux/Mac)
source .venv/bin/activate

# Ativar (Windows)
.venv\Scripts\activate

# Instalar dependências
pip install gurobipy matplotlib pandas
```

---

## 🔨 Compilação

### Algoritmos C++ (Uso Direto e Benchmark)

Compile todos os algoritmos com os seguintes comandos:

```bash
# Algoritmos principais (para uso direto)
g++ -std=c++17 -O2 dijkstra.cpp -o dijkstra.exe
g++ -std=c++17 -O2 biDijkstra.cpp -o biDijkstra.exe
g++ -std=c++17 -O2 dialDijkstra.cpp -o dialDijkstra.exe
g++ -std=c++17 -O2 biDialDijkstra.cpp -o biDialDijkstra.exe

# Versões benchmark (com medição de tempo)
g++ -std=c++17 -O2 dijkstra_benchmark.cpp -o dijkstra_benchmark.exe
g++ -std=c++17 -O2 biDijkstra_benchmark.cpp -o biDijkstra_benchmark.exe
g++ -std=c++17 -O2 dialDijkstra_benchmark.cpp -o dialDijkstra_benchmark.exe
g++ -std=c++17 -O2 biDialDijkstra_benchmark.cpp -o biDialDijkstra_benchmark.exe
```

**Nota:** No Windows, os executáveis terão extensão `.exe`. No Linux/Mac, omita a extensão.

### Preparação dos Arquivos de Teste

Antes de executar, certifique-se de que as pastas `input/` e `output/` contêm os arquivos de teste fornecidos:

```
input/arq01.in ... arq10.in
output/arq01.out ... arq10.out
```

---

## ▶️ Execução Interativa (Para Avaliadores)

O sistema oferece uma interface interativa para facilitar a avaliação dos algoritmos.

### Iniciar o Sistema Interativo

```bash
# Ativar ambiente virtual (se estiver usando)
source .venv/bin/activate  # Linux/Mac
.venv\Scripts\activate     # Windows

# Executar interface interativa
python main.py
```

### Fluxo de Uso

1. **Entrada de Dados**: O sistema solicita que você digite a entrada no formato especificado:
   ```
   --- ENTRADA DE DADOS ---
   Digite a entrada conforme especificado:
     Linha 1: N M S D
     Próximas M linhas: U V W
   ```
   
   Após digitar a primeira linha (N M S D) e pressionar Enter, o sistema automaticamente lerá as próximas M linhas. Assim que todas as M arestas forem inseridas, o sistema prossegue automaticamente para a seleção do algoritmo.

2. **Seleção do Algoritmo**: Escolha o algoritmo digitando o número correspondente:
   ```
   --- SELEÇÃO DE ALGORITMO ---
   Digite o número do algoritmo que deseja utilizar:
   [1] Dijkstra Clássico
   [2] Dijkstra Bidirecional
   [3] Dial Dijkstra
   [4] BiDial Dijkstra
   [5] Programação Linear (Gurobi)
   
   Sua escolha: 
   ```

3. **Resultado**: O sistema exibe **apenas a distância mínima** encontrada:
   ```
   ======================================================================
   RESULTADO: 48.787
   ======================================================================
   ```
   
   Ou `TIMEOUT (>5min)` caso o algoritmo exceda 5 minutos (comum para PL em instâncias grandes).

4. **Continuar ou Sair**: O sistema pergunta se deseja testar novamente:
   ```
   --- CONTINUAR? ---
   [1] Sim, quero continuar testando
   [2] Não, quero sair
   
   Sua escolha:
   ```

### Execução Manual de Algoritmos Individuais

Todos os programas C++ também podem ser executados diretamente via stdin/stdout:

```bash
# Linux/Mac
cat input/arq01.in | ./dijkstra

# Windows
type input\arq01.in | dijkstra.exe
```

**Saída:** Apenas um número (a distância mínima) ou `INF` se não houver caminho.

```
48.787
```

---

## 📊 Sistema de Benchmark

O sistema de benchmark automatizado compara o desempenho de todos os algoritmos em todas as instâncias de teste.

### Como Funciona o Benchmark

O sistema `benchmark_runner.py` realiza as seguintes operações:

1. **Compilação Automática**: Utiliza as versões `*_benchmark.cpp` e `*_benchmark.py` dos algoritmos
2. **Medição Precisa**: Mede **apenas o tempo de execução do algoritmo**, excluindo:
   - Leitura da entrada
   - Construção das estruturas de dados
   - Qualquer I/O
3. **Validação de Resultados**: Compara com resultados esperados em `output/`
4. **Geração de Relatórios**: Cria CSVs e gráfico comparativo

### Executar Benchmark Completo

```bash
# Ativar ambiente virtual
source .venv/bin/activate  # Linux/Mac
.venv\Scripts\activate     # Windows

# Executar benchmark
python benchmark_runner.py
```

### O que o Benchmark Faz

Para cada arquivo de teste (`arq01.in` a `arq10.in`), o sistema:

1. Executa todos os 5 algoritmos
2. Mede o tempo de execução em **microssegundos**
3. Compara resultado obtido vs. resultado esperado
4. Calcula diferença absoluta e percentual
5. Salva dados em CSV individual
6. Gera gráfico comparativo único ao final

### Resultados Gerados

```
results/
├── arq01/
│   └── arq01_results.csv          # Tabela com resultados e tempos
├── arq02/
│   └── arq02_results.csv
├── ...
├── arq10/
│   └── arq10_results.csv
└── comparison_all_algorithms.png   # Gráfico comparativo geral
```

### Estrutura dos CSVs

```csv
Algoritmo,Resultado_Esperado,Resultado_Obtido,Diferenca_Absoluta,Diferenca_Percentual,Tempo_Execucao_us
Dijkstra Clássico,48.787,48.787,0.0,0.000000%,0
Dijkstra Bidirecional,48.787,48.787,0.0,0.000000%,0
Dial Dijkstra,48.787,49.0,0.213,0.436543%,997
BiDial Dijkstra,48.787,49.0,0.213,0.436543%,998
Programação Linear,48.787,48.787,0.0,0.000000%,2919
```

### Gráfico Comparativo

O arquivo `comparison_all_algorithms.png` mostra:

- **Eixo X**: Instâncias de teste (1 a 10)
- **Eixo Y**: Tempo de execução em milissegundos (escala linear)
- **Linhas coloridas**: Cada algoritmo com pontos conectados
- **Legenda**: Identificação de cada algoritmo

**Características do gráfico:**
- Pontos com tempo 0 (< 1μs) são exibidos
- PL aparece apenas em instâncias onde não deu timeout (1-6)
- Facilita visualizar qual algoritmo é mais rápido para cada instância

---

## 📄 Formato de Entrada/Saída

### Formato de Entrada

**Primeira linha:** `N M S D`
- `N` = número de vértices (numerados de 0 a N-1), onde 5×10⁴ ≤ N ≤ 5×10⁵
- `M` = número de arestas, onde 2.5×10⁴ ≤ M ≤ 2.5×10⁵
- `S` = vértice origem (source)
- `D` = vértice destino (destination)

**Próximas M linhas:** `U V W`
- `U` = vértice conectado pela aresta
- `V` = outro vértice conectado pela aresta
- `W` = custo para percorrer a aresta, onde 100.0 ≤ W ≤ 500.0

**Importante:** O grafo é **não direcionado**. Cada aresta conecta U e V em ambas as direções.

### Exemplo de Entrada

```
4 5 0 3
0 1 1.5
0 2 4.0
1 2 2.5
1 3 6.0
2 3 1.0
```

### Formato de Saída

**Saída:** Apenas um número (distância mínima) ou `INF` se não houver caminho.

**Exemplos:**
```
5.000000
```

```
48.787
```

```
INF
```

**Observação:** Algoritmos Dial e BiDial arredondam pesos para inteiros, então podem retornar valores ligeiramente diferentes (ex: 49.0 ao invés de 48.787).

---

## 🔍 Observações Importantes

### Medição de Tempo no Benchmark

- As versões `*_benchmark.cpp` e `*_benchmark.py` medem **apenas** o tempo de execução do algoritmo
- **Excluído da medição:**
  - Leitura da entrada (`stdin`)
  - Construção das estruturas de dados (grafo, listas de adjacência)
  - Escrita da saída (`stdout`)
- A medição usa `std::chrono::high_resolution_clock` (C++) e `time.perf_counter` (Python)
- Tempo reportado em **microssegundos** nos CSVs, convertido para milissegundos nos gráficos

### Aproximações dos Algoritmos Dial

- **Dial Dijkstra** e **BiDial Dijkstra** arredondam pesos para o **inteiro mais próximo**
- Exemplo: peso 48.787 → arredondado para 49
- Isso pode resultar em distâncias diferentes dos algoritmos exatos
- A diferença é pequena (tipicamente < 1%) mas deve ser considerada na análise

### Limitações do Dial em Instâncias Grandes

- Complexidade: O(V + E + N×C) onde C é o maior peso
- Para instâncias grandes (N > 60.000, C ≈ 500):
  - Cria ~30 milhões de buckets
  - Pode ser **mais lento** que Dijkstra clássico
- Eficiente apenas quando **C << N** (grafos densos com pesos pequenos)

### Timeout do Programação Linear

- PL tem timeout de **5 minutos** no sistema de benchmark
- Instâncias grandes (arq07-arq10) tipicamente excedem esse tempo
- Isso é esperado: LP solvers gerais são muito mais lentos que algoritmos especializados
- Resultados de PL aparecem apenas para instâncias pequenas (arq01-arq06) no gráfico

### Requisitos do Gurobi

- `plShortestPath.py` requer Gurobi Optimizer e licença válida
- Licença acadêmica gratuita disponível em: https://www.gurobi.com/academia/
- Biblioteca Python: `pip install gurobipy`
- Sem Gurobi, o algoritmo PL não funcionará

---

## 📝 Resumo dos Arquivos

### Arquivos Principais

- **`main.py`** - Interface interativa para avaliadores testarem algoritmos individualmente
- **`benchmark_runner.py`** - Sistema automatizado de benchmark com geração de relatórios

### Implementações C++

- **Versões para uso direto:** `dijkstra.cpp`, `biDijkstra.cpp`, `dialDijkstra.cpp`, `biDialDijkstra.cpp`
  - Leem de stdin, escrevem apenas resultado em stdout
  - Sem medição de tempo
  
- **Versões benchmark:** `*_benchmark.cpp`
  - Incluem medição precisa de tempo (excluindo I/O)
  - Tempo em stderr, resultado em stdout

### Implementações Python

- **`plShortestPath.py`** - Versão para uso direto
- **`plShortestPath_benchmark.py`** - Versão com medição de tempo

---

## 🤝 Informações do Projeto

**Disciplina:** MC558 - Projeto e Análise de Algoritmos II  
**Semestre:** 2º Semestre de 2025  
**RA:** 215041  
**Instituição:** Universidade Estadual de Campinas (Unicamp)