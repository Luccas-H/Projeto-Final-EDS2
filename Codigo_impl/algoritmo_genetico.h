#ifndef ALGORITMO_GENETICO_H

#define TAM_POPULACAO 100
#define TAXA_MUTACAO 0.05
#define NUM_GERACOES 500

/* Individuos funcionam como rotas */
typedef struct {
    int* caminho;
    int custo; //Valor da distância total da rota(exemplo)
} Individuo;

typedef struct No {
    int destino;
    int peso;
    struct No* proximo;
} No;

typedef struct {
    int numVertices;
    int matrizAdjacencia[100][100];
    No* listaAdjacencia[100];
} Grafo;

// grafo.c
// grafo.c
Grafo* criarGrafo(int vertices);
No* criarNo(int destino, int peso);
void adicionarAresta(Grafo* grafo, int origem, int destino, int peso);
void imprimirGrafo(Grafo* grafo, char nomeBairro[][100]);
void liberarGrafo(Grafo* grafo);
void menu(Grafo* grafo, int pesos[100][100], int numeroCidades, char nomeBairro[][100]);

// algoritmo_genetico.c
void algoritmoGenetico(Grafo* grafo, int matrizPesos[100][100], int numCidades, char nomes[][100], int cidadeInicial);
Individuo* gerarPopulacaoInicial(int numCidades, int cidadeInicial);
int comparar(const void* a, const void* b);
int calcularCusto(int* caminho, int numCidades, int matriz[100][100]);
void embaralhar(int* vetor, int tamanho);
void avaliarPopulacao(Individuo* populacao, int numCidades, int matriz[100][100]);
void cruzar(Individuo pai1, Individuo pai2, Individuo* filho, int numCidades);
void mutar(Individuo* individuo, int numCidades);

#endif
