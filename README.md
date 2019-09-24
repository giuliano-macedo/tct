# Trabalhos Da Disciplina Teoria Da Computação - UFMS - CPPP
Comparar heuristicas para o problema do caixeiro viajante, utilizando como critério 
o tempo de processamento, qualidade da resposta e memória gasta por processo (memória residente máxima)
para a heuristica gulosa baseada no item com maior custo benefício,GRASP e tabu.
## Pré requisitos
* g++ >= 5
* make
* python3
* pip

## instalação
```shell
pip install -r requirements.txt
```
opcionalmente também pode compilar os códigos fontes C++, porém no código em python ele irá compilar sozinho
na primeira vez.
```shell
cd src/core
make
```
## Uso
```shell
cd src
python3 main.py
```
após disso o programa irá compilar se necessário os arquivos fontes em C++, e também gerar os arquivos de resultado .csv, e depois mostrar os gráficos dos resultados.
para mais opções use o seguinte comando
```shell
python3 main.py --help
```