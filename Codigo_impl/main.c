#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "algoritmo_genetico.h"

int main() {
    int numeroCidades;
    printf("Digite o numero de cidades desejadas: ");
    scanf("%d", &numeroCidades);
    getchar();

    char nome[numeroCidades][100];
    char buffer[100];
    int pesos[100][100];

    for (int i = 0; i < numeroCidades; i++) {
        printf("Digite o nome da %d cidade:\n", i + 1);
        fgets(buffer, 100, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        int isNumber = 1;

        for(int j = 0; buffer[j] != '\0'; j++) {
            if (!isdigit(buffer[j]))
            {
                isNumber = 0;
                break;
            }
        }

        if(isNumber) {
            system("cls");
            printf("Error: O nome da cidade nao pode ser um numero!\n");
            sleep(1.5);
            system("cls");

            i--;
            continue;
        }

        strcpy(nome[i], buffer);
}

    Grafo* meuGrafo = criarGrafo(numeroCidades);

    int peso;
    for (int i = 0; i < numeroCidades; i++) {
        for (int j = i + 1; j < numeroCidades; j++) {
            printf("Digite o peso da aresta de %s ate %s (-1 para nenhum):\n", nome[i], nome[j]);
            scanf("%d", &peso);

            if (peso != -1) {
                pesos[i][j] = pesos[j][i] = peso;
                adicionarAresta(meuGrafo, i, j, peso);
            } else {
                pesos[i][j] = pesos[j][i] = 0;
            }
        }
    }

    printf("\nDistancias registradas:\n");
    for (int i = 0; i < numeroCidades; i++) {
        for (int j = i + 1; j < numeroCidades; j++) {
            if (pesos[i][j] > 0) {
                printf("Distancia de %s ate %s: %d KM\n", nome[i], nome[j], pesos[i][j]);
            }
        }
    }

    menu(meuGrafo, pesos, numeroCidades, nome);
    liberarGrafo(meuGrafo);
    return 0;
}
