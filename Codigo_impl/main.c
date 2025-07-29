#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "algoritmo_genetico.h"

int main() {
    int numeroCidades;
    int inputValid = 0;
    while(!inputValid)
    {
        printf("Digite o numero de cidades desejadas: ");
        if(scanf("%d", &numeroCidades) != 1 || numeroCidades <= 0) {
            system("cls");
            printf("Error: O numero de cidades deve ser um numero inteiro positivo!\n");
            printf("Tente Novamente!\n");
            Sleep(2000);
            system("cls");

            while(getchar() != '\n');
            continue;
        }
        char checkFinalChar = getchar(); // Verifica se o numero digitado pelo usuario é float

        if(checkFinalChar != '\n' && checkFinalChar != ' ') {
            system("cls");
            printf("Error: Digite apenas numeros inteiros!\n");
            printf("Tente Novamente!\n");
            Sleep(2000);
            system("cls");

            while(getchar() != '\n'); // Limpa o buffer de entrada
            continue;
        
        }
        inputValid = 1;
    
    }

    char nome[numeroCidades][100];// nomes das cidades
    char buffer[100]; // buffer para validar entrada
    int pesos[100][100];// matriz de pesos das arestas
    char specialChars[] = "~^´`!@#$%&*()+={}[]|\\:;\"'<>?,./-_"; // caracteres especiais para validação

    //Entrada dos nomes das cidades com validação
    for (int i = 0; i < numeroCidades; i++) {
        printf("Digite o nome da %d cidade:\n", i + 1);
        fgets(buffer, 100, stdin); // buffer para ler a entrada do usuário
        buffer[strcspn(buffer, "\n")] = '\0'; //remove o \n do final da string

        // Verifica para input vazio
        if (strlen(buffer) == 0) {
            system("cls");
            printf("Error: O nome da cidade nao pode estar vazio!\n");
            printf("Tente Novamente!\n");
            Sleep(2000);
            system("cls");
            i--;
            continue;
        }

        int isNumber = 0;

        for(int j = 0; buffer[j] != '\0'; j++) { // Verifica se o nome contém números, caracteres especiais ou se é igual ao anterior
            if (isdigit(buffer[j]) || strchr(specialChars, buffer[j]) || strcmp(nome[j], buffer) == 0) 
            {
                isNumber = 1;
                break;
            }
        }
        if(isNumber) { // Caso verdadeiro, imprime mensagem de erro e reinicia o loop
            system("cls");
            printf("Error: O nome da cidade nao pode conter numeros, caracteres especiais ou ser igual ao anterior!\n");
            printf("Tente Novamente!\n");
            Sleep(2000);
            system("cls");

            i--;
            continue;
        }

        strcpy(nome[i], buffer); // Caso contrário, copia o nome para o array
        memset(buffer, 0, sizeof(buffer)); // Limpa o buffer após copiar para evitar problemas
}

    Grafo* meuGrafo = criarGrafo(numeroCidades); // Chama a função que inicializa o grafo com o número de vértices fornecido.

    // Entrada dos pesos das arestas
    int peso;
    for (int i = 0; i < numeroCidades; i++) {
        for (int j = i + 1; j < numeroCidades; j++) {
            printf("Digite o peso da aresta de %s ate %s (-1 para nenhum):\n", nome[i], nome[j]);

            if(scanf("%d", &peso) != 1) // Verifica se a entrada é um número
            {
                system("cls");
                printf("Error: O peso da aresta so pode ser um numero!\n");
                printf("Tente Novamente!\n");
                Sleep(2000);
                system("cls");

                while (getchar() != '\n');
                j--;
                continue;
            }
            char checkFinalChar = getchar(); // Verifica se o numero digitado pelo usuario é float

            if(checkFinalChar != '\n' && checkFinalChar != ' ') {
                system("cls");
                printf("Error: Digite apenas numeros inteiros!\n");
                printf("Tente Novamente!\n");
                Sleep(2000);
                system("cls");

                while(getchar() != '\n'); // Limpa o buffer de entrada
                j--;
                continue;
            
            }

            if (peso != -1) { // Condição de parada para pesos válidos
                pesos[i][j] = pesos[j][i] = peso; // Preenche a matriz de pesos
                adicionarAresta(meuGrafo, i, j, peso); // Adiciona cada peso digitado no grafo
            } else {
                pesos[i][j] = pesos[j][i] = 0; 
            }
        }
    }

    printf("\nDistancias registradas:\n"); 
    for (int i = 0; i < numeroCidades; i++) { // Exibição das distâncias registradas
        for (int j = i + 1; j < numeroCidades; j++) {
            if (pesos[i][j] > 0) {
                printf("Distancia de %s ate %s: %d KM\n", nome[i], nome[j], pesos[i][j]);
            }
        }
    }
    Sleep(3000);
    system("cls");
    menu(meuGrafo, pesos, numeroCidades, nome); // Chama o menu para interagir com o usuário
    liberarGrafo(meuGrafo); // Libera a memória alocada para o grafo
    return 0;
}
