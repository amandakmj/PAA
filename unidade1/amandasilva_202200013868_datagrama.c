#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct datagrama {
    char **dadosPacote;
    int32_t tamanhoPacote;
    bool ordenado;
} datagramas;


//tentando a saida
int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int32_t n, quantidade, pacotesProcessados = 0;
    fscanf(input, "%d %d", &n, &quantidade);


    datagramas *pacotes = malloc(n*sizeof(datagramas));

    for (int i=0; i<n; i++) pacotes[i].ordenado = false;


    for (int i=0; i<n; i++) {
        int posicao, tamanhoFragmentos;
        fscanf(input, "%d %d", &posicao, &tamanhoFragmentos);

        pacotes[posicao].dadosPacote = malloc(tamanhoFragmentos*sizeof(char*));
        pacotes[posicao].tamanhoPacote = tamanhoFragmentos;

        for (int j=0; j<tamanhoFragmentos; j++) {
            pacotes[posicao].dadosPacote[j] = malloc(3*sizeof(char));
            fscanf(input, "%s", pacotes[posicao].dadosPacote[j]);
        }


        pacotes[posicao].ordenado = true;


        if ((i+1)%quantidade == 0 || i==n-1) {
            if (pacotes[pacotesProcessados].ordenado) {
                while (pacotes[pacotesProcessados].ordenado && pacotesProcessados < n) {
                    //troca
                    fprintf(output, "|");
                    for (int j=0; j<pacotes[pacotesProcessados].tamanhoPacote; j++) {
                        if (j > 0) {
                            fprintf(output, ",");
                        }
                        fprintf(output, "%s", pacotes[pacotesProcessados].dadosPacote[j]);
                    }
                    pacotesProcessados++;
                }
                fprintf(output, "|\n");
            }
        }
    }
}