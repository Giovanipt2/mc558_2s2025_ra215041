# Script de compilação para todos os algoritmos
# Compila os algoritmos principais em algorithms/ e as versões benchmark em benchmarks/

Write-Host "=== COMPILANDO ALGORITMOS ===" -ForegroundColor Cyan
Write-Host ""

# Compilar algoritmos principais
Write-Host "Compilando algoritmos principais (algorithms/)..." -ForegroundColor Yellow
Set-Location algorithms

g++ -std=c++17 -O2 dijkstra.cpp -o dijkstra.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ dijkstra.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar dijkstra.cpp" -ForegroundColor Red }

g++ -std=c++17 -O2 biDijkstra.cpp -o biDijkstra.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ biDijkstra.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar biDijkstra.cpp" -ForegroundColor Red }

g++ -std=c++17 -O2 dialDijkstra.cpp -o dialDijkstra.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ dialDijkstra.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar dialDijkstra.cpp" -ForegroundColor Red }

g++ -std=c++17 -O2 biDialDijkstra.cpp -o biDialDijkstra.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ biDialDijkstra.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar biDialDijkstra.cpp" -ForegroundColor Red }

Set-Location ..

Write-Host ""
Write-Host "Compilando versões benchmark (benchmarks/)..." -ForegroundColor Yellow
Set-Location benchmarks

g++ -std=c++17 -O2 dijkstra_benchmark.cpp -o dijkstra_benchmark.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ dijkstra_benchmark.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar dijkstra_benchmark.cpp" -ForegroundColor Red }

g++ -std=c++17 -O2 biDijkstra_benchmark.cpp -o biDijkstra_benchmark.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ biDijkstra_benchmark.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar biDijkstra_benchmark.cpp" -ForegroundColor Red }

g++ -std=c++17 -O2 dialDijkstra_benchmark.cpp -o dialDijkstra_benchmark.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ dialDijkstra_benchmark.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar dialDijkstra_benchmark.cpp" -ForegroundColor Red }

g++ -std=c++17 -O2 biDialDijkstra_benchmark.cpp -o biDialDijkstra_benchmark.exe
if ($LASTEXITCODE -eq 0) { Write-Host "  ✓ biDialDijkstra_benchmark.exe" -ForegroundColor Green } else { Write-Host "  ✗ Erro ao compilar biDialDijkstra_benchmark.cpp" -ForegroundColor Red }

Set-Location ..

Write-Host ""
Write-Host "=== COMPILAÇÃO CONCLUÍDA ===" -ForegroundColor Cyan
Write-Host ""
Write-Host "Para executar a interface interativa:" -ForegroundColor White
Write-Host "  python algorithms/main.py" -ForegroundColor Gray
Write-Host ""
Write-Host "Para executar o benchmark completo:" -ForegroundColor White
Write-Host "  python benchmarks/benchmark_runner.py" -ForegroundColor Gray
Write-Host ""
