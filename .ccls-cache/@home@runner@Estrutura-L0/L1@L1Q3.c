#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 100

void processalinha(char * linha){
    int tipo = 0;
    int numero;
    float numerof;
    char * copia = malloc(sizeof(linha));

    // strcpy(copia, linha);

    char * token = strtok(linha, " ");

    while(token != NULL){
        if(strcmp(token, "LE") == 0){
            tipo = 1;
        } else if(strcmp(token, "LI") == 0){
            tipo = 2;
        }

        if(tipo == 1){
            numero = atoi(token);
            if(numero != 0 || strcmp(token, "0") == 0){
                printf("\nnumero = %d\n", numero);
            }
        } else if(tipo == 2){
            numerof = atof(token);
            if(numerof != 0 || strcmp(token, "0") == 0){
                printf("\nnumero = %.3g\n", numerof);
            }
        }

        token = strtok(NULL, " ");
    }

    free(copia);
}

int main(){
    FILE * input;
    char * linha = malloc(MAX_CHAR * sizeof(char));

    input = fopen("L1Q3.in", "r");

    if(input){
        fgets(linha, MAX_CHAR, input);

        processalinha(linha);

    }else{
        perror("Não foi possível abrir o arquivo de entrada.\n");
    }

    free(linha);
    fclose(input);

    return 0;
}