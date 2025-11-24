# MC558 - Projeto de Implementação: Algoritmos de Caminho Mais Curto

Este repositório contém a implementação e análise comparativa de cinco algoritmos para o problema de caminho mais curto em grafos direcionados com pesos não-negativos.

**Disciplina:** MC558 - Projeto e Análise de Algoritmos II  
**Semestre:** 2º Semestre de 2025  
**RA:** 215041

## 📋 Índice

- [Visão Geral](#visão-geral)
- [Estrutura do Repositório](#estrutura-do-repositório)
- [Algoritmos Implementados](#algoritmos-implementados)
- [Requisitos](#requisitos)
- [Compilação](#compilação)
- [Execução](#execução)
- [Benchmark](#benchmark)
- [Geração de Gráficos](#geração-de-gráficos)
- [Formato de Entrada/Saída](#formato-de-entradasaída)

---

## 🎯 Visão Geral

Este projeto implementa e compara cinco abordagens diferentes para resolver o problema de caminho mais curto:

1. **Dijkstra Clássico** - Implementação padrão com priority queue
2. **Dial's Algorithm** - Otimização do Dijkstra para pesos inteiros pequenos
3. **Dijkstra Bidirecional** - Busca simultânea da origem e do destino
4. **Dial Bidirecional** - Combinação da busca bidirecional com Dial
5. **Programação Linear** - Formulação como problema de fluxo (requer Gurobi)

O projeto inclui um harness de benchmark para comparação de desempenho e scripts para geração de gráficos e análises.

---

## 📁 Estrutura do Repositório

```
.
├── README.md                  # Este arquivo
├── .gitignore                 # Regras para ignorar arquivos
├── ra215041.pdf              # Relatório do projeto (até 3 páginas)
│
├── dijkstra.cpp              # Algoritmo de Dijkstra clássico
├── dialDijkstra.cpp          # Algoritmo de Dial (pesos inteiros)
├── biDijkstra.cpp            # Dijkstra bidirecional
├── biDialDijkstra.cpp        # Dial bidirecional
├── plShortestPath.cpp        # Formulação por programação linear
│
├── bench/                    # Sistema de benchmark
│   ├── bench.cpp             # Harness de benchmark
│   ├── run_bench.sh          # Script para executar benchmarks
│   └── results/              # CSVs e plots gerados (gitignored)
│
├── scripts/                  # Scripts auxiliares
│   ├── compile_all.sh        # Compila todos os programas
│   └── plot_results.py       # Gera gráficos dos resultados
│
├── docs/                     # Documentação
│   └── dependencies.md       # Instruções detalhadas de instalação
│
├── input/                    # Arquivos de entrada (local, gitignored)
└── output/                   # Arquivos de saída esperados (local, gitignored)
```

---

## 🔬 Algoritmos Implementados

### 1. Dijkstra Clássico (`dijkstra.cpp`)

- **Estrutura de dados:** Priority queue (min-heap)
- **Pesos:** Double (ponto flutuante)
- **Complexidade:** O((V + E) log V)
- **Uso:** Grafos gerais com pesos não-negativos

### 2. Dial's Algorithm (`dialDijkstra.cpp`)

- **Estrutura de dados:** Buckets circulares
- **Pesos:** Inteiros (arredondamento de doubles)
- **Complexidade:** O(V + E + C·V) onde C é o maior peso
- **Uso:** Grafos com pesos inteiros pequenos
- **Observação:** Pesos são arredondados, pode gerar aproximação

### 3. Dijkstra Bidirecional (`biDijkstra.cpp`)

- **Estrutura de dados:** Duas priority queues (forward e backward)
- **Pesos:** Double
- **Complexidade:** O((V + E) log V) - mais rápido na prática
- **Uso:** Grafos grandes onde busca em ambas direções é vantajosa

### 4. Dial Bidirecional (`biDialDijkstra.cpp`)

- **Estrutura de dados:** Buckets circulares para ambas direções
- **Pesos:** Inteiros
- **Complexidade:** O(V + E + C·V)
- **Uso:** Combinação das vantagens de Dial e busca bidirecional

### 5. Programação Linear (`plShortestPath.cpp`)

- **Formulação:** Problema de fluxo unitário
- **Solver:** Gurobi Optimizer
- **Variáveis:** x_e ∈ [0,1] para cada aresta
- **Objetivo:** Minimizar Σ w_e · x_e
- **Uso:** Validação teórica, não otimizado para performance

---

## 💻 Requisitos

### Obrigatórios

- **Compilador:** GCC/G++ com suporte a C++17
- **Sistema:** Linux, macOS ou Windows (com MSYS2/MinGW)

### Opcionais

- **Gurobi Optimizer** (para `plShortestPath.cpp`)
  - Licença acadêmica gratuita disponível
  - Alternativas: GLPK, CBC (requer adaptação do código)
- **Python 3.6+** com matplotlib e pandas (para gerar gráficos)
- **Google Benchmark** (para medições mais rigorosas)

**Consulte [`docs/dependencies.md`](docs/dependencies.md) para instruções detalhadas de instalação.**

---

## 🔨 Compilação

### Compilação Rápida

Use o script que compila todos os programas automaticamente:

```bash
bash scripts/compile_all.sh
```

### Compilação Manual

#### Algoritmos básicos (sem dependências externas)

```bash
g++ -std=c++17 -O2 dijkstra.cpp -o dijkstra
g++ -std=c++17 -O2 dialDijkstra.cpp -o dialDijkstra
g++ -std=c++17 -O2 biDijkstra.cpp -o biDijkstra
g++ -std=c++17 -O2 biDialDijkstra.cpp -o biDialDijkstra
```

#### Programação Linear (com Gurobi)

```bash
# Configure GUROBI_HOME primeiro
export GUROBI_HOME=/opt/gurobi120/linux64

# Compile com Gurobi (ajuste o número da versão)
g++ -std=c++17 -O2 -DUSE_GUROBI plShortestPath.cpp \
    -I${GUROBI_HOME}/include \
    -L${GUROBI_HOME}/lib \
    -lgurobi_c++ -lgurobi120 \
    -o plShortestPath
```

**Nota:** O script `compile_all.sh` detecta automaticamente a versão do Gurobi.

---

## ▶️ Execução

Todos os programas leem da entrada padrão (`stdin`) e escrevem na saída padrão (`stdout`).

### Formato de Execução

```bash
cat input/arquivo.in | ./programa
```

### Exemplos

```bash
# Dijkstra clássico
cat input/arq01.in | ./dijkstra

# Dial
cat input/arq02.in | ./dialDijkstra

# Bidirecional
cat input/arq03.in | ./biDijkstra

# Dial Bidirecional
cat input/arq04.in | ./biDialDijkstra

# Programação Linear (se Gurobi estiver instalado)
cat input/arq05.in | ./plShortestPath
```

### Saída

Cada programa imprime **apenas um número**: a distância mínima do vértice origem ao destino, ou `INF` se não houver caminho.

```
42.500000
```

ou

```
INF
```

---

## 📊 Benchmark

O sistema de benchmark compara o desempenho de todos os algoritmos.

### Executar Benchmark Completo

```bash
# Compila bench.cpp e executa para todos os arquivos em input/
bash bench/run_bench.sh
```

### Configurações

- **Número de repetições:** Editável em `run_bench.sh` (padrão: 10)
- **Arquivos de entrada:** Todos os `*.in` em `input/`
- **Resultados:** Salvos em `bench/results/` como CSVs

### Benchmark Manual

```bash
# Compile o benchmark
g++ -std=c++17 -O2 bench/bench.cpp -o bench/bench

# Execute para um arquivo específico
./bench/bench input/arq01.in 10 bench/results/results_arq01.csv
```

### Estrutura dos CSVs

```csv
algorithm,input_file,distance,time_seconds,repetition
dijkstra,input/arq01.in,42.500000,0.001234,1
dialDijkstra,input/arq01.in,43,0.000987,1
...
```

---

## 📈 Geração de Gráficos

Após executar o benchmark, gere gráficos comparativos:

```bash
python3 scripts/plot_results.py
```

### Gráficos Gerados

Para cada arquivo de entrada:

1. **Tempo de Execução** (`*_execution_time.png`)
   - Gráfico de barras comparando tempo médio ± desvio padrão

2. **Distâncias Obtidas** (`*_distances.png`)
   - Comparação das distâncias retornadas por cada algoritmo

3. **Distribuição de Tempo** (`*_time_distribution.png`)
   - Boxplot mostrando variabilidade dos tempos

4. **Tabela Resumo** (`*_summary.txt`)
   - Estatísticas detalhadas em formato texto

### Localização

Todos os gráficos são salvos em: `bench/results/plots/`

---

## 📄 Formato de Entrada/Saída

### Formato de Entrada

**Primeira linha:** `N M S D`
- `N` = número de vértices (0 a N-1)
- `M` = número de arestas
- `S` = vértice origem
- `D` = vértice destino

**Próximas M linhas:** `U V W`
- `U` = vértice origem da aresta
- `V` = vértice destino da aresta
- `W` = peso da aresta (double)

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

**Saída:** Um único número (distância mínima) ou `INF`

```
5.000000
```

---

## 🔍 Observações Importantes

### Medição de Tempo

- As medições de tempo devem ser feitas **após** o carregamento do grafo em memória
- O programa de benchmark (`bench.cpp`) segue esta diretriz
- Os programas individuais não imprimem tempo (apenas a distância)

### Aproximações

- **Dial** e **Dial Bidirecional** arredondam pesos para inteiros
- Isso pode resultar em distâncias diferentes dos algoritmos exatos
- Compare os resultados no CSV gerado pelo benchmark

### Limitações do Dial

- Se o maior peso `C` for muito grande (> 10^6), a memória pode ser excessiva
- Um aviso é impresso no stderr se `C` for muito grande
- Considere usar Dijkstra clássico para grafos com pesos muito variados

### Gurobi

- `plShortestPath.cpp` **requer** Gurobi para funcionar
- Se compilado sem `-DUSE_GUROBI`, o programa imprime instruções de instalação
- Alternativas open-source (GLPK, CBC) requerem adaptação do código

---

## 📚 Documentação Adicional

- **[docs/dependencies.md](docs/dependencies.md)** - Instruções detalhadas de instalação
- **Cabeçalhos dos arquivos `.cpp`** - Documentação inline de cada algoritmo
- **Enunciado do projeto** - Consulte `MC558_Projeto_Implementacao_Versao1.pdf`

---

## 🧪 Testes Recomendados

### Casos de Teste Básicos

1. **Grafo pequeno conectado** - Verifica corretude básica
2. **Grafo desconectado** - Verifica detecção de `INF`
3. **Grafo com peso zero** - Testa casos extremos
4. **Grafo denso** - Compara desempenho com muitas arestas
5. **Grafo esparso** - Avalia vantagem de algoritmos bidirecionais

### Configurações de Performance

Para benchmarks mais precisos:

```bash
# Aumente o limite de arquivos abertos
ulimit -n 4096

# Execute múltiplas vezes e calcule média/mediana
for i in {1..20}; do
    bash bench/run_bench.sh
done
```

---

## 🤝 Contribuições e Uso

Este é um projeto acadêmico para a disciplina MC558. O código é fornecido como referência para estudos.

**Autor:** RA 215041  
**Instituição:** Unicamp  
**Disciplina:** MC558 - Projeto e Análise de Algoritmos II