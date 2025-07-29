#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "algoritmo_genetico.h"

// Cria um grafo com vertices, com estruturas internas preparadas para armazenar arestas
Grafo* criarGrafo(int vertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;
    // Inicializa a lista de adjacência e a matriz de adjacência
    for (int i = 0; i < vertices; i++) {
        grafo->listaAdjacencia[i] = NULL;
        for (int j = 0; j < vertices; j++) {
            grafo->matrizAdjacencia[i][j] = 0;
        }
    }

    return grafo;
}

// Cria um nó que representa uma aresta na lista de adjacência, com destino e peso definidos
No* criarNo(int destino, int peso) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->destino = destino;
    novoNo->peso = peso;
    novoNo->proximo = NULL;
    return novoNo;
}

// Insere uma aresta bidirecional entre dois vértices na lista de adjacência, com verificação para não duplicar conexões
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

// Imprime a lista de adjacências do grafo criado, contendo nome e pesos
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

//Libera o espaço de memória alocado na criação do grafo
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

/*
    Fornecer um menu interativo para o usuário:

    Ver o grafo;
    Escolher a cidade de partida;
    Rodar o algoritmo genético;
    Encerrar o programa.
*/

void menu(Grafo* grafo, int pesos[100][100], int numeroCidades, char nomeBairro[][100]) {


    int opcao;
    do {
        int menuInput= 0;
        while(!menuInput) { // Verifica se a entrada pro menu é válida
            printf("=====================================\n");
            printf("  BEM-VINDO AO ALGORITMO GENETICO!\n");
            printf("        Otimizacao de Rotas \n");
            printf("   Problema do Caixeiro Viajante \n");
            printf("=====================================\n\n");

            printf("Menu:\n");
            printf("1. Imprimir Grafo\n");
            printf("2. Escolher cidade e rodar algoritmo genetico\n");
            printf("3. Sair\n");

            printf("Digite sua escolha: ");
            if(scanf("%d", &opcao) != 1) {
                system("cls");
                printf("Error: Digite apenas numeros inteiros!\n");
                printf("Tente Novamente!\n");
                Sleep(2000);
                system("cls");
                while(getchar() != '\n'); // Limpa o buffer de entrada
                continue;
            }
            char checkFinalCharMenu = getchar(); // Verifica se o numero digitado pelo usuario é float
            
            if(checkFinalCharMenu != '\n' && checkFinalCharMenu != ' ') {
                system("cls");
                printf("Error: Digite apenas numeros inteiros!\n");
                printf("Tente Novamente!\n");
                Sleep(2000);
                system("cls");
                
                while(getchar() != '\n'); // Limpa o buffer de entrada
                continue;
                
            }
            menuInput = 1;
        }
        system("cls");
        
        switch (opcao) {
            case 1:
                imprimirGrafo(grafo, nomeBairro);
                system("pause");
                system("cls");
                break;
            case 2: {
                int cidadeInicial;
                int checkFinalCharPartida;
                int inputValid = 0;

                while(!inputValid) { // Verifica se a entrada da cidade inicial é válida
                    printf("Escolha a cidade de partida (digite o numero):\n");
                    for (int i = 0; i < numeroCidades; i++) {
                        printf("%d -> %s\n", i, nomeBairro[i]);
                    }
                    printf("Sua escolha: ");
                    // Verifica se a entrada é um número inteiro
                    if(scanf("%d", &cidadeInicial) != 1 || cidadeInicial < 0 || cidadeInicial >= numeroCidades) { 
                        system("cls");
                        printf("Error: Cidade invalida! Tente novamente.\n");
                        Sleep(2000);
                        system("cls");
                        while(getchar() != '\n');
                        continue;
                    }
                    inputValid = 1;
                }

                algoritmoGenetico(grafo, pesos, numeroCidades, nomeBairro, cidadeInicial);
                system("pause");
                system("cls");
                break;
            }
            case 3:
                printf("Encerrando...\n");
                liberarGrafo(grafo); // Libera a memória alocada para o grafo
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 3);
}
