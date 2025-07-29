#include <stdio.h>
#include <stdlib.h>
#include "algoritmo_genetico.h"

Grafo* criarGrafo(int vertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;

    for (int i = 0; i < vertices; i++) {
        grafo->listaAdjacencia[i] = NULL;
        for (int j = 0; j < vertices; j++) {
            grafo->matrizAdjacencia[i][j] = 0;
        }
    }

    return grafo;
}

No* criarNo(int destino, int peso) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->destino = destino;
    novoNo->peso = peso;
    novoNo->proximo = NULL;
    return novoNo;
}

void adicionarAresta(Grafo* grafo, int origem, int destino, int peso) {
    No* atual = grafo->listaAdjacencia[origem];
    while (atual != NULL) {
        if (atual->destino == destino) return;
        atual = atual->proximo;
    }

    No* novo = criarNo(destino, peso);
    novo->proximo = grafo->listaAdjacencia[origem];
    grafo->listaAdjacencia[origem] = novo;

    No* novo2 = criarNo(origem, peso);
    novo2->proximo = grafo->listaAdjacencia[destino];
    grafo->listaAdjacencia[destino] = novo2;
}

void imprimirGrafo(Grafo* grafo, char nomeBairro[][100]) {
    printf("Grafo (lista de adjacência):\n");
    for (int i = 0; i < grafo->numVertices; i++) {
        No* temp = grafo->listaAdjacencia[i];
        printf("%s -> ", nomeBairro[i]);
        while (temp) {
            printf("%s(%d) ", nomeBairro[temp->destino], temp->peso);
            temp = temp->proximo;
        }
        printf("\n");
    }
}

void liberarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        No* atual = grafo->listaAdjacencia[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(grafo);
}

void menu(Grafo* grafo, int pesos[100][100], int numeroCidades, char nomeBairro[][100]) {
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Imprimir Grafo\n");
        printf("2. Escolher cidade e rodar algoritmo genético\n");
        printf("3. Sair\n");

        printf("Digite sua escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                imprimirGrafo(grafo, nomeBairro);
                break;
            case 2: {
                int cidadeInicial;
                printf("\nEscolha a cidade de partida (digite o número):\n");
                for (int i = 0; i < numeroCidades; i++) {
                    printf("%d - %s\n", i, nomeBairro[i]);
                }
                printf("Sua escolha: ");
                scanf("%d", &cidadeInicial);

                algoritmoGenetico(grafo, pesos, numeroCidades, nomeBairro, cidadeInicial);
                break;
            }
            case 3:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 3);
}
