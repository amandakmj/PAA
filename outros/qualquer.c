#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CONTAINERS 100000

typedef struct {
    char codigo[20];
    char cnpj[20];
    int peso;
    int divergencia_cnpj;
    int peso_diferenca;
    double percentual_peso;
    int ordem;
} Container;

int arredondar(double value) {
    if (value == (int)value) {
        return (int)value;
    }
    return (int)(value + 0.5);
}

double percentual(int peso_cadastrado, int peso_selecionado) {
    if (peso_cadastrado == 0) {
        return 0.0;
    }
    return abs(peso_selecionado - peso_cadastrado) * 100.0 / (double)peso_cadastrado;
}

void merge(Container arr[], int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    Container *I = malloc(n1 * sizeof(Container));
    Container *F = malloc(n2 * sizeof(Container));

    if (I == NULL || F == NULL) {
        printf("Erro ao alocar memória para as divisões do merge.\n");
        exit(1);
    }

    for (int i = 0; i < n1; i++)
        I[i] = arr[inicio + i];
    for (int i = 0; i < n2; i++)
        F[i] = arr[meio + 1 + i];

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2) {
        if (I[i].divergencia_cnpj != F[j].divergencia_cnpj) {
            if (I[i].divergencia_cnpj > F[j].divergencia_cnpj)
                arr[k++] = I[i++];
            else
                arr[k++] = F[j++];
        } else if (I[i].divergencia_cnpj) {
            if (I[i].ordem < F[j].ordem)
                arr[k++] = I[i++];
            else
                arr[k++] = F[j++];
        } else {
            int p1 = arredondar(I[i].percentual_peso);
            int p2 = arredondar(F[j].percentual_peso);
            if (p1 > p2)
                arr[k++] = I[i++];
            else if (p1 < p2)
                arr[k++] = F[j++];
            else {
                if (I[i].ordem < F[j].ordem)
                    arr[k++] = I[i++];
                else
                    arr[k++] = F[j++];
            }
        }
    }

    while (i < n1) {
        arr[k++] = I[i++];
    }

    while (j < n2) {
        arr[k++] = F[j++];
    }

    free(I);  
    free(F);  
}

void merge_sort(Container arr[], int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        merge_sort(arr, inicio, meio);
        merge_sort(arr, meio + 1, fim);
        merge(arr, inicio, meio, fim);
    }
}

void processar_triagem(Container cadastrados[], int n, Container selecionados[], int m) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (strcmp(selecionados[i].codigo, cadastrados[j].codigo) == 0) {
                if (strcmp(selecionados[i].cnpj, cadastrados[j].cnpj) != 0) {
                    selecionados[i].divergencia_cnpj = 1;
                }

                selecionados[i].peso_diferenca = abs(selecionados[i].peso - cadastrados[j].peso);
                selecionados[i].percentual_peso = percentual(cadastrados[j].peso, selecionados[i].peso);
                selecionados[i].ordem = j;
                break;
            }
        }
    }
    merge_sort(selecionados, 0, m - 1);
}

void exibir_resultados(FILE* output, Container cadastrados[], Container selecionados[], int m) {
    for (int i = 0; i < m; i++) {
        if (selecionados[i].divergencia_cnpj) {
            
            if (selecionados[i].ordem >= 0 && selecionados[i].ordem < MAX_CONTAINERS) {
                fprintf(output, "%s:%s<->%s\n", selecionados[i].codigo, cadastrados[selecionados[i].ordem].cnpj, selecionados[i].cnpj);
            }
        } else {
            int percentual_arredondado = arredondar(selecionados[i].percentual_peso);
            if (percentual_arredondado > 10) {
                fprintf(output, "%s:%dkg(%d%%)\n", selecionados[i].codigo, selecionados[i].peso_diferenca, percentual_arredondado);
            }
        }
    }
}

int main(int argc, char* argv[]) {
   
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

  
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    if (input == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    if (output == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return 1;
    }

    int n, m;
    Container cadastrados[MAX_CONTAINERS];
    Container selecionados[MAX_CONTAINERS];


    if (fscanf(input, "%d", &n) != 1 || n > MAX_CONTAINERS) {
        printf("Erro ao ler o número de containers cadastrados ou número excede o limite.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(input, "%s %s %d", cadastrados[i].codigo, cadastrados[i].cnpj, &cadastrados[i].peso) != 3) {
            printf("Erro ao ler os dados do container cadastrado.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
        cadastrados[i].divergencia_cnpj = 0;
        cadastrados[i].peso_diferenca = 0;
        cadastrados[i].percentual_peso = 0.0;
    }

 
    if (fscanf(input, "%d", &m) != 1 || m > MAX_CONTAINERS) {
        printf("Erro ao ler o número de containers selecionados ou número excede o limite.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    for (int i = 0; i < m; i++) {
        if (fscanf(input, "%s %s %d", selecionados[i].codigo, selecionados[i].cnpj, &selecionados[i].peso) != 3) {
            printf("Erro ao ler os dados do container selecionado.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
        selecionados[i].divergencia_cnpj = 0;
        selecionados[i].peso_diferenca = 0;
        selecionados[i].percentual_peso = 0.0;
    }

    processar_triagem(cadastrados, n, selecionados, m);
    exibir_resultados(output, cadastrados, selecionados, m);

    fclose(input);
    fclose(output);
    return 0;
}
