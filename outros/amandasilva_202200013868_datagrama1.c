#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_VETOR 512

typedef struct {
    int32_t numero_pacote;
    int32_t tamanho_pacote;
    int32_t elementos[MAX_VETOR];
} Pacote;

void trocar(Pacote* a, Pacote* b) {
    Pacote temp = *a;
    *a = *b;
    *b = temp;
}

// Ordenação dos pacotes pelo número do pacote usando Bubble Sort
void ordenar_pacotes(Pacote* pacotes, int32_t num_pacotes) {
    for (int32_t i = 0; i < num_pacotes - 1; i++) {
        for (int32_t j = 0; j < num_pacotes - i - 1; j++) {
            if (pacotes[j].numero_pacote > pacotes[j + 1].numero_pacote) {
                trocar(&pacotes[j], &pacotes[j + 1]);
            }
        }
    }
}

// Ordenação dos elementos dentro de um pacote em ordem crescente
void ordenar_elementos(int32_t* elementos, int32_t tamanho) {
    for (int32_t i = 0; i < tamanho - 1; i++) {
        for (int32_t j = 0; j < tamanho - i - 1; j++) {
            if (elementos[j] > elementos[j + 1]) {
                int32_t temp = elementos[j];
                elementos[j] = elementos[j + 1];
                elementos[j + 1] = temp;
            }
        }
    }
}

// Impressão formatada dos pacotes
void imprimir_pacotes(Pacote* pacotes, int32_t num_pacotes, FILE* output) {
    for (int32_t i = 0; i < num_pacotes; i++) {
        ordenar_elementos(pacotes[i].elementos, pacotes[i].tamanho_pacote); // Ordena os elementos dentro do pacote
    }

    for (int32_t i = 0; i < num_pacotes; i++) {
        fprintf(output, "|"); // Começa o pacote

        for (int32_t j = 0; j < pacotes[i].tamanho_pacote; j++) {
            fprintf(output, "%02X", pacotes[i].elementos[j]); // Imprime o elemento

            if (j < pacotes[i].tamanho_pacote - 1) {
                fprintf(output, ","); // Separador entre elementos
            }
        }

        fprintf(output, "|"); // Fecha o pacote
    }

    fprintf(output, "\n");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    if (!input || !output) {
        printf("Erro ao abrir arquivos.\n");
        if (input) fclose(input);
        if (output) fclose(output);
        return 1;
    }

    int32_t num_pacotes, quantidade_pacotes_recebidos;
    if (fscanf(input, "%d %d", &num_pacotes, &quantidade_pacotes_recebidos) != 2 || num_pacotes <= 0) {
        printf("Erro ao ler o número de pacotes ou a quantidade de pacotes recebidos.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    Pacote* pacotes = (Pacote*)malloc(num_pacotes * sizeof(Pacote));
    if (!pacotes) {
        printf("Erro de alocação de memória.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    for (int32_t i = 0; i < num_pacotes; i++) {
        if (fscanf(input, "%d %d", &pacotes[i].numero_pacote, &pacotes[i].tamanho_pacote) != 2 ||
            pacotes[i].tamanho_pacote < 0 || pacotes[i].tamanho_pacote > MAX_VETOR) {
            printf("Erro ao ler número e tamanho do pacote %d.\n", i);
            free(pacotes);
            fclose(input);
            fclose(output);
            return 1;
        }
        for (int32_t j = 0; j < pacotes[i].tamanho_pacote; j++) {
            if (fscanf(input, "%X", &pacotes[i].elementos[j]) != 1) {
                printf("Erro ao ler elemento %d do pacote %d.\n", j, i);
                free(pacotes);
                fclose(input);
                fclose(output);
                return 1;
            }
        }
    }

    ordenar_pacotes(pacotes, num_pacotes);  // Ordena os pacotes pelo número do pacote antes da saída
    imprimir_pacotes(pacotes, num_pacotes, output);

    free(pacotes);
    fclose(input);
    fclose(output);

    return 0;
}