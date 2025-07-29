#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "algoritmo_genetico.h"

int comparar(const void* a, const void* b) {
    Individuo* ind1 = (Individuo*)a;
    Individuo* ind2 = (Individuo*)b;
    return ind1->custo - ind2->custo;
}

int calcularCusto(int* caminho, int numCidades, int matriz[100][100]) {
    int custo = 0;
    for (int i = 0; i < numCidades - 1; i++) {
        custo += matriz[caminho[i]][caminho[i + 1]];
    }
    custo += matriz[caminho[numCidades - 1]][caminho[0]];
    return custo;
}

void embaralhar(int* vetor, int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

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
        embaralhar(populacao[i].caminho, numCidades - 1);

        // Insere cidade inicial no início
        for (int j = numCidades - 1; j > 0; j--) {
            populacao[i].caminho[j] = populacao[i].caminho[j - 1];
        }
        populacao[i].caminho[0] = cidadeInicial;

        populacao[i].custo = 0;
    }
    return populacao;
}

void avaliarPopulacao(Individuo* populacao, int numCidades, int matriz[100][100]) {
    for (int i = 0; i < TAM_POPULACAO; i++) {
        populacao[i].custo = calcularCusto(populacao[i].caminho, numCidades, matriz);
    }
    qsort(populacao, TAM_POPULACAO, sizeof(Individuo), comparar);
}

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

void mutar(Individuo* individuo, int numCidades) {
    if (((float)rand() / RAND_MAX) < TAXA_MUTACAO) {
        int a = 1 + rand() % (numCidades - 1);
        int b = 1 + rand() % (numCidades - 1);
        int temp = individuo->caminho[a];
        individuo->caminho[a] = individuo->caminho[b];
        individuo->caminho[b] = temp;
    }
}


void algoritmoGenetico(Grafo* grafo, int matrizPesos[100][100], int numCidades, char nomes[][100], int cidadeInicial) {
    srand(time(NULL));
    Individuo* populacao = gerarPopulacaoInicial(numCidades, cidadeInicial);
    Individuo* novaGeracao = (Individuo*)malloc(TAM_POPULACAO * sizeof(Individuo));

    for (int i = 0; i < TAM_POPULACAO; i++) {
        novaGeracao[i].caminho = (int*)malloc(numCidades * sizeof(int));
    }

    for (int geracao = 0; geracao < NUM_GERACOES; geracao++) {
        avaliarPopulacao(populacao, numCidades, matrizPesos);

        // Elitismo
        for (int i = 0; i < TAM_POPULACAO / 2; i++) {
            for (int j = 0; j < numCidades; j++) {
                novaGeracao[i].caminho[j] = populacao[i].caminho[j];
            }
            novaGeracao[i].custo = populacao[i].custo;
        }

        // Cruzamento + mutação
        for (int i = TAM_POPULACAO / 2; i < TAM_POPULACAO; i++) {
            int pai1 = rand() % (TAM_POPULACAO / 2);
            int pai2 = rand() % (TAM_POPULACAO / 2);
            cruzar(populacao[pai1], populacao[pai2], &novaGeracao[i], numCidades);
            mutar(&novaGeracao[i], numCidades);
        }

        // Troca
        Individuo* temp = populacao;
        populacao = novaGeracao;
        novaGeracao = temp;
    }

    avaliarPopulacao(populacao, numCidades, matrizPesos);
    Individuo melhor = populacao[0];

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
