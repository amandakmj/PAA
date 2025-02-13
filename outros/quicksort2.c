#include <stdio.h>
#include <stdlib.h>

#define MAX_VETOR 1000
#define MAX_NUM_VETORES 100

typedef struct {
    int tamanho;
    int elementos[MAX_VETOR];
} Vetor;

// Protótipos
void trocar(int* a, int* b, int *swaps);
int LP(int arr[], int indInicio, int indFinal, int *swaps, int *calls);
int LM(int arr[], int indInicio, int indFinal, int *swaps, int *calls, int numVetores);
int HP(int arr[], int indInicio, int indFinal, int *swaps, int *calls);
void quicksortLP(int *arr, int indInicio, int indFinal, int *swaps, int *calls);
void quicksortHP(int *arr, int indInicio, int indFinal, int *swaps, int *calls);
int* mediana(int *v1, int *v2, int *v3);  // Protótipo atualizado

int main() {
    int numVetores;
    Vetor vetores[MAX_NUM_VETORES];

    // Leitura do número de vetores
    scanf("%d", &numVetores);

    // Leitura dos vetores
    for (int i = 0; i < numVetores; i++) {
        scanf("%d", &vetores[i].tamanho);
        for (int j = 0; j < vetores[i].tamanho; j++) {
            scanf("%d", &vetores[i].elementos[j]);
        }
    }

    // Processamento e ordenação dos vetores
    for (int i = 0; i < numVetores; i++) {
        int tamanho = vetores[i].tamanho;
        int *vetor = vetores[i].elementos;

        printf("%d:N(%d)", i, tamanho);

        int swaps = 0, calls = 0;
        int temporaria[MAX_VETOR];

        // Copiar o vetor original para manter consistência
        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }

        // Ordenação apenas com LP
        quicksortLP(temporaria, 0, tamanho - 1, &swaps, &calls);
        printf(",LP(%d)", swaps + calls); // Exibe os resultados para LP

        swaps = 0;
        calls = 0;
        quicksortLP(temporaria, 0, tamanho - 1, &swaps, &calls);  // Passando pelo vetor inteiro com LM
        printf(",LM(%d)", swaps + calls);


        swaps = 0;  // Resetar a contagem de trocas para HP
        calls = 0;  // Resetar a contagem de chamadas para HP

        for (int j = 0; j < tamanho; j++) {
            temporaria[j] = vetor[j];
        }   

    // Ordenação apenas com HP
        quicksortHP(temporaria, 0, tamanho - 1, &swaps, &calls);
        printf(",HP(%d)\n", swaps + calls); // Exibe os resultados para HP

    }

    return 0;
}

void trocar(int* a, int* b, int *swaps) {
    int temporaria = *a;
    *a = *b;
    *b = temporaria;
    (*swaps)++;
}


// Função LP para partição com contagem de chamadas e trocas
int LP(int arr[], int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++; // Incrementa a contagem de chamadas

    int pivo = arr[indFinal], x = indInicio - 1, y = indInicio;
    for (y = indInicio; y < indFinal; y++)
        if (arr[y] <= pivo) 
            trocar(&arr[++x], &arr[y], swaps);
    trocar(&arr[++x], &arr[indFinal], swaps);
    return x;
}

// Função LM para partição de Lomuto com escolha do pivô pela Mediana de 3
int LM(int arr[], int indInicio, int indFinal, int *swaps, int *calls, int numVetores) {
    (*calls)++;
    // Removendo (*calls)++, pois LP já contabiliza chamadas corretamente.

    int n = indFinal - indInicio + 1;

    // Verifica se o vetor tem pelo menos 3 elementos para aplicar a Mediana de 3
    if (n < 3) {
        return LP(arr, indInicio, indFinal, swaps, calls);
    }

    // Escolhendo os índices da Mediana de 3
    int idx1 = indInicio + (n / 4);
    int idx2 = indInicio + (n / 2);
    int idx3 = indInicio + (3 * n / 4);

    // Obtendo os valores correspondentes
    int v1 = arr[idx1];
    int v2 = arr[idx2];
    int v3 = arr[idx3];

    // Obtém o valor da mediana de 3
    int *medianaEscolhida = mediana(&v1, &v2, &v3);
    
    // Encontrando a posição do pivô na lista para colocá-lo no final
    int pivo;
    if (*medianaEscolhida == v1) pivo = idx1;
    else if (*medianaEscolhida == v2) pivo = idx2;
    else pivo = idx3;

    // Apenas troca se o pivô não estiver no lugar correto
    if (pivo != indFinal) {
        trocar(&arr[pivo], &arr[indFinal], swaps);
    }

    // Agora chamamos a partição de Lomuto normalmente
    return LP(arr, indInicio, indFinal, swaps, calls);
}




// Função HP para partição com contagem de chamadas e trocas
int HP(int *arr, int indInicio, int indFinal, int *swaps, int *calls) {
    (*calls)++; // Contabiliza chamada
    
    int pivo = arr[indInicio], x = indInicio - 1, y = indFinal + 1;
    while(1) {
        while(arr[--y] > pivo);
        while(arr[++x] < pivo);
        if (x < y) {
            trocar(&arr[x], &arr[y], swaps);  // Chama a função trocar que também conta os swaps
        }
        else {
            return y; // Retorna a posição de divisão
        }
    }
}


// Chama LP para partição com o pivô específico de Lomuto
void quicksortLP(int *arr, int indInicio, int indFinal, int *swaps, int *calls) {
    if (indInicio < indFinal) {
        (*calls)++;
        int pivo = LP(arr, indInicio, indFinal, swaps, calls);  // Usa LP
        quicksortLP(arr, indInicio, pivo - 1, swaps, calls);     // Chamada recursiva para a parte esquerda
        quicksortLP(arr, pivo + 1, indFinal, swaps, calls);      // Chamada recursiva para a parte direita
    }
}

// Chama HP para partição com o pivô específico de Hoare
void quicksortHP(int *arr, int indInicio, int indFinal, int *swaps, int *calls) {
    if (indInicio < indFinal) {
        (*calls)++;
        int pivo = HP(arr, indInicio, indFinal, swaps, calls);  // Usa HP
        quicksortHP(arr, indInicio, pivo, swaps, calls);        // Chamada recursiva para a parte esquerda
        quicksortHP(arr, pivo + 1, indFinal, swaps, calls);     // Chamada recursiva para a parte direita
    }
}

// Função mediana (com parâmetros alterados para v1, v2 e v3)
int* mediana(int *v1, int *v2, int *v3) {
    if(*v1 > *v2) {
        if (*v2 > *v3) return v2;
        else if (*v3 > *v1) return v1;
        else return v3;
    }
    else {
        if(*v1 > *v3) return v1;
        else if(*v3 > *v2) return v2;
        else return v3;
    }
}
