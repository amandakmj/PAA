#include <stdio.h>
#include <stdlib.h>

#define MAX_VETOR 1000
#define MAX_NUM_VETORES 100

typedef struct {
    int tamanho;
    int elementos[MAX_VETOR];
} Vetor;

// Função para trocar elementos
void trocar(int* a, int* b, int *swaps) {
    int temporaria = *a;
    *a = *b;
    *b = temporaria;
    (*swaps)++;
}

// Função para encontrar a mediana de 3 elementos
int mediana(int v1, int v2, int v3) {
    if (v1 > v2) {
        if (v2 > v3) return v2;
        else if (v3 > v1) return v1;
        else return v3;
    }
    else {
        if (v1 > v3) return v1;
        else if (v3 > v2) return v2;
        else return v3;
    }
}

// Função de particionamento de Lomuto (LP)
int LP(int arr[], int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++;

    int pivo = arr[indFinal], x = indInicio - 1, y = indInicio;
    for (y = indInicio; y < indFinal; y++)
        if (arr[y] <= pivo) 
            trocar(&arr[++x], &arr[y], swaps);
    trocar(&arr[++x], &arr[indFinal], swaps);
    return x;
}

// Função de particionamento de Lomuto com Mediana de 3 (LM)
int LM(int arr[], int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++;

    int n = indFinal - indInicio + 1;

    if (n < 3) {
        return LP(arr, indInicio, indFinal, swaps, calls);
    }

    int idx1 = indInicio + (n / 4);
    int idx2 = indInicio + (n / 2);
    int idx3 = indInicio + (3 * n / 4);

    int v1 = arr[idx1];
    int v2 = arr[idx2];
    int v3 = arr[idx3];

    int medianaEscolhida = mediana(v1, v2, v3);  // Usamos o valor diretamente
    
    int pivo;
    if (medianaEscolhida == v1) pivo = idx1;
    else if (medianaEscolhida == v2) pivo = idx2;
    else pivo = idx3;

    if (pivo != indFinal) {
        trocar(&arr[pivo], &arr[indFinal], swaps);
    }

    return LP(arr, indInicio, indFinal, swaps, calls);
}

// Função de particionamento de Hoare (HP)
int HP(int *arr, int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++;
    
    int pivo = arr[indInicio], x = indInicio - 1, y = indFinal + 1;
    while(1) {
        while(arr[--y] > pivo);
        while(arr[++x] < pivo);
        if (x < y) {
            trocar(&arr[x], &arr[y], swaps);
        }
        else {
            return y;
        }
    }
}

// Função de ordenação Quicksort com Lomuto
void quicksortLP(int *arr, int indInicio, int indFinal, int *swaps, int *calls) {
    if (indInicio < indFinal) {
        (*calls)++;
        int pivo = LP(arr, indInicio, indFinal, swaps, calls);
        quicksortLP(arr, indInicio, pivo - 1, swaps, calls);
        quicksortLP(arr, pivo + 1, indFinal, swaps, calls);
    }
}

// Função de ordenação Quicksort com Hoare
void quicksortHP(int *arr, int indInicio, int indFinal, int *swaps, int *calls) {
    if (indInicio < indFinal) {
        (*calls)++;
        int pivo = HP(arr, indInicio, indFinal, swaps, calls);
        quicksortHP(arr, indInicio, pivo, swaps, calls);
        quicksortHP(arr, pivo + 1, indFinal, swaps, calls);
    }
}

// Função para particionamento de Lomuto com Aleatório (LA)
int LA(int arr[], int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++;
    
    int randomIdx = indInicio + rand() % (indFinal - indInicio + 1);
    trocar(&arr[randomIdx], &arr[indFinal], swaps);
    return LP(arr, indInicio, indFinal, swaps, calls);
}

// Função de particionamento de Hoare com Mediana de 3 (HM)
int HM(int arr[], int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++;
    
    int n = indFinal - indInicio + 1;

    if (n < 3) {
        return LP(arr, indInicio, indFinal, swaps, calls);
    }

    int idx1 = indInicio + (n / 4);
    int idx2 = indInicio + (n / 2);
    int idx3 = indInicio + (3 * n / 4);

    int v1 = arr[idx1];
    int v2 = arr[idx2];
    int v3 = arr[idx3];

    int medianaEscolhida = mediana(v1, v2, v3);  // Usamos o valor diretamente
    
    int pivo;
    if (medianaEscolhida == v1) pivo = idx1;
    else if (medianaEscolhida == v2) pivo = idx2;
    else pivo = idx3;

    if (pivo != indFinal) {
        trocar(&arr[pivo], &arr[indFinal], swaps);
    }

    return HP(arr, indInicio, indFinal, swaps, calls);
}

// Função para particionamento de Hoare com Pivô Aleatório (HA)
int HA(int arr[], int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++;

    int randomIdx = indInicio + rand() % (indFinal - indInicio + 1);
    trocar(&arr[randomIdx], &arr[indFinal], swaps);
    return HP(arr, indInicio, indFinal, swaps, calls);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <entrada.txt> <saida.txt>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    if (input == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }
    if (output == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
        fclose(input);
        return 1;
    }

    int numVetores;
    Vetor vetores[MAX_NUM_VETORES];

    fscanf(input, "%d", &numVetores);

    for (int i = 0; i < numVetores; i++) {
        fscanf(input, "%d", &vetores[i].tamanho);
        if (vetores[i].tamanho > MAX_VETOR) {
            fprintf(stderr, "Tamanho do vetor %d excede o limite máximo de %d.\n", i, MAX_VETOR);
            fclose(input);
            fclose(output);
            return 1;
        }
        for (int j = 0; j < vetores[i].tamanho; j++) {
            fscanf(input, "%d", &vetores[i].elementos[j]);
        }
    }

    for (int i = 0; i < numVetores; i++) {
        int tamanho = vetores[i].tamanho;
        int *vetor = vetores[i].elementos;

        fprintf(output, "%d:N(%d)", i, tamanho);

        int swaps = 0, calls = 0;
        int temporaria[MAX_VETOR];

        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }

        quicksortLP(temporaria, 0, tamanho - 1, &swaps, &calls);
        fprintf(output, ",LP(%d)", swaps + calls);

        swaps = 0;
        calls = 0;
        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }

        quicksortLP(temporaria, 0, tamanho - 1, &swaps, &calls);
        fprintf(output, ",LM(%d)", swaps + calls);

        swaps = 0;
        calls = 0;
        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }

        quicksortHP(temporaria, 0, tamanho - 1, &swaps, &calls);
        fprintf(output, ",HP(%d)", swaps + calls);

        swaps = 0;
        calls = 0;
        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }

        quicksortHP(temporaria, 0, tamanho - 1, &swaps, &calls);
        fprintf(output, ",HM(%d)", swaps + calls);

        swaps = 0;
        calls = 0;
        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }

        quicksortLP(temporaria, 0, tamanho - 1, &swaps, &calls);
        fprintf(output, ",LA(%d)", swaps + calls);

        swaps = 0;
        calls = 0;
        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }

        quicksortHP(temporaria, 0, tamanho - 1, &swaps, &calls);
        fprintf(output, ",HA(%d)\n", swaps + calls);
    }

    fclose(input);
    fclose(output);

    return 0;
}