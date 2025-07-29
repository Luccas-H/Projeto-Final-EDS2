#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "algoritmo_genetico.h"

// Realiza o comparativo entre rotas
int comparar(const void* a, const void* b) {
    Individuo* ind1 = (Individuo*)a;
    Individuo* ind2 = (Individuo*)b;
    return ind1->custo - ind2->custo;
}

// Calcula o custo de cada rota criada, utilizando-se da matriz de adjacencias e do caminho 
int calcularCusto(int* caminho, int numCidades, int matriz[100][100]) {
    int custo = 0;
    for (int i = 0; i < numCidades - 1; i++) {
        custo += matriz[caminho[i]][caminho[i + 1]];
    }
    custo += matriz[caminho[numCidades - 1]][caminho[0]];
    return custo;
}

// Embaralha aleatoriamente os elementos de um vetor usado para gerar caminhos aleatórios
void embaralhar(int* vetor, int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}
// Gera a população inicial de indivíduos (rotas aleatórias)
Individuo* gerarPopulacaoInicial(int numCidades, int cidadeInicial) {
    Individuo* populacao = (Individuo*)malloc(TAM_POPULACAO * sizeof(Individuo));
    for (int i = 0; i < TAM_POPULACAO; i++) {
        populacao[i].caminho = (int*)malloc(numCidades * sizeof(int));
        // Gera vetor de 0 a n-1 exceto a cidadeInicial
        int idx = 0;
        for (int j = 0; j < numCidades; j++) {
            if (j != cidadeInicial) {
                populacao[i].caminho[idx++] = j;
            }
        }
        embaralhar(populacao[i].caminho, numCidades - 1);// Embaralha as cidades para formar rotas diferentes

        // Insere cidade inicial no início
        for (int j = numCidades - 1; j > 0; j--) {
            populacao[i].caminho[j] = populacao[i].caminho[j - 1];
        }
        populacao[i].caminho[0] = cidadeInicial;

        populacao[i].custo = 0;
    }
    return populacao;
}

// Calcula o custo (distância total) de cada indivíduo da população e ordena do melhor para o pior
void avaliarPopulacao(Individuo* populacao, int numCidades, int matriz[100][100]) {
    for (int i = 0; i < TAM_POPULACAO; i++) {
        populacao[i].custo = calcularCusto(populacao[i].caminho, numCidades, matriz);
    }
    qsort(populacao, TAM_POPULACAO, sizeof(Individuo), comparar);
}

// Realiza o cruzamento (crossover) entre dois pais para gerar um filho, mantendo a cidade inicial fixa e evitando repetições
void cruzar(Individuo pai1, Individuo pai2, Individuo* filho, int numCidades) {
    int inicio = rand() % numCidades;
    int fim = inicio + rand() % (numCidades - inicio);
    if (fim >= numCidades) fim = numCidades - 1;
    if (fim < inicio) {
        int temp = inicio;
        inicio = fim;
        fim = temp;
    }

    filho->caminho[0] = pai1.caminho[0]; // fixa a cidade inicial

    int* usado = (int*)calloc(numCidades, sizeof(int));
    usado[filho->caminho[0]] = 1;

    for (int i = inicio; i <= fim; i++) {
        filho->caminho[i] = pai1.caminho[i];
        usado[filho->caminho[i]] = 1;
    }

    int pos = 1;
    for (int i = 1; i < numCidades; i++) {
        if (!usado[pai2.caminho[i]]) {
            while (pos >= inicio && pos <= fim) pos = fim + 1;
            if (pos < numCidades)
                filho->caminho[pos++] = pai2.caminho[i];
        }
    }

    free(usado);
}

// Aplica mutação trocando duas cidades aleatórias (exceto a inicial) para introduzir diversidade genética
void mutar(Individuo* individuo, int numCidades) {
    if (((float)rand() / RAND_MAX) < TAXA_MUTACAO) {
        int a = 1 + rand() % (numCidades - 1);
        int b = 1 + rand() % (numCidades - 1);
        int temp = individuo->caminho[a];
        individuo->caminho[a] = individuo->caminho[b];
        individuo->caminho[b] = temp;
    }
}

/*
    Algoritmo Genético para resolver o Problema do Caixeiro Viajante (TSP)

    Etapas principais:
    - Geração da população inicial (rotas aleatórias)
    - Avaliação de cada rota (função de aptidão baseada na distância total)
    - Seleção dos melhores indivíduos (elitismo)
    - Cruzamento entre pais para formar novos indivíduos (filhos)
    - Mutação ocasional para evitar estagnação
    - Repetição do processo por várias gerações

    O objetivo é encontrar a menor rota possível que visita todas as cidades e retorna à origem.
*/

void algoritmoGenetico(Grafo* grafo, int matrizPesos[100][100], int numCidades, char nomes[][100], int cidadeInicial) {
    srand(time(NULL));// Em cada execução a forma aleatória é mantida!
    Individuo* populacao = gerarPopulacaoInicial(numCidades, cidadeInicial);//Cria um conjunto inicial de soluções (rotas), todas começando na cidade escolhida pelo usuário
    Individuo* novaGeracao = (Individuo*)malloc(TAM_POPULACAO * sizeof(Individuo));//Reserva memória para a nova geração (filhos) que será criada durante o processo evolutivo.

    for (int i = 0; i < TAM_POPULACAO; i++) {
        novaGeracao[i].caminho = (int*)malloc(numCidades * sizeof(int));
    }// Faz com que cada indivuduo seja um vetor de cidades(rota)

    // A cada geração, a população é avaliada, evoluída e substituída, Isso faz com que apenas a elite das rotas continue 
    for (int geracao = 0; geracao < NUM_GERACOES; geracao++) {
        avaliarPopulacao(populacao, numCidades, matrizPesos);

        // Elitismo
        for (int i = 0; i < TAM_POPULACAO / 2; i++) {
            for (int j = 0; j < numCidades; j++) {
                novaGeracao[i].caminho[j] = populacao[i].caminho[j];
            }
            novaGeracao[i].custo = populacao[i].custo;//Na nova geração metade(os melhores 50%) continuam na nova geração 
        }

        // Cruzamento + mutação
        for (int i = TAM_POPULACAO / 2; i < TAM_POPULACAO; i++) {
            int pai1 = rand() % (TAM_POPULACAO / 2);
            int pai2 = rand() % (TAM_POPULACAO / 2);
            cruzar(populacao[pai1], populacao[pai2], &novaGeracao[i], numCidades);
            mutar(&novaGeracao[i], numCidades);
        }

        // Troca os ponteiros, agora a nova geração vira a população atual
        Individuo* temp = populacao;
        populacao = novaGeracao;
        novaGeracao = temp;
    } 

    // Apos todo esse looping, ele avalia o melhor individuo(rota) gerado e copia
    avaliarPopulacao(populacao, numCidades, matrizPesos);
    Individuo melhor = populacao[0];

    //exibição dos resultados 
    printf("\nMelhor rota encontrada:\n");
    for (int i = 0; i < numCidades; i++) {
        printf("%s -> ", nomes[melhor.caminho[i]]);
    }
    printf("%s\n", nomes[melhor.caminho[0]]);
    printf("Distância total: %d KM\n", melhor.custo);

    for (int i = 0; i < TAM_POPULACAO; i++) {
        free(populacao[i].caminho);
        free(novaGeracao[i].caminho);
    }
    free(populacao);
    free(novaGeracao);
}
