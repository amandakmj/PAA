#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_CONTAINERS 100000
#define MAX_HASH_SIZE 200003

typedef struct {
    char codigo[20];
    char cnpj[20];
    int peso;
    int divergencia_cnpj;
    int peso_diferenca;
    double percentual_peso;
    int ordem;
} Container;

typedef struct {
    char codigo[20];
    int indice;
} HashEntry;

typedef struct {
    HashEntry *tabela[MAX_HASH_SIZE];
} HashTable;

int arredondar(double value) {
    return (int)(value + 0.5);
}

double percentual(int peso_cadastrado, int peso_selecionado) {
    return (peso_cadastrado == 0) ? 0.0 : abs(peso_selecionado - peso_cadastrado) * 100.0 / peso_cadastrado;
}

int hash_code(const char *codigo) {
    int hash = 0;
    while (*codigo) {
        hash = (hash * 31 + *codigo++) % MAX_HASH_SIZE;
    }
    return hash;
}

void insert_hash(HashTable *ht, const char *codigo, int indice) {
    int index = hash_code(codigo);
    while (ht->tabela[index] != NULL) {
        index = (index + 1) % MAX_HASH_SIZE;
    }
    HashEntry *entry = malloc(sizeof(HashEntry));
    strcpy(entry->codigo, codigo);
    entry->indice = indice;
    ht->tabela[index] = entry;
}

int search_hash(HashTable *ht, const char *codigo) {
    int index = hash_code(codigo);
    while (ht->tabela[index] != NULL) {
        if (strcmp(ht->tabela[index]->codigo, codigo) == 0) {
            return ht->tabela[index]->indice;
        }
        index = (index + 1) % MAX_HASH_SIZE;
    }
    return -1;
}

void merge(Container arr[], int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    Container *I = malloc(n1 * sizeof(Container));
    Container *F = malloc(n2 * sizeof(Container));

    for (int i = 0; i < n1; i++) 
        I[i] = arr[inicio + i];
    for (int i = 0; i < n2; i++) 
        F[i] = arr[meio + 1 + i];

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2) {
        if (I[i].divergencia_cnpj != F[j].divergencia_cnpj) {
            arr[k++] = (I[i].divergencia_cnpj > F[j].divergencia_cnpj) ? I[i++] : F[j++];
        } else if (I[i].divergencia_cnpj) {
            arr[k++] = (I[i].ordem < F[j].ordem) ? I[i++] : F[j++];
        } else {
            int p1 = arredondar(I[i].percentual_peso);
            int p2 = arredondar(F[j].percentual_peso);
            if (p1 != p2) {
                arr[k++] = (p1 > p2) ? I[i++] : F[j++];
            } else {
                arr[k++] = (I[i].ordem < F[j].ordem) ? I[i++] : F[j++];
            }
        }
    }

    while (i < n1) arr[k++] = I[i++];
    while (j < n2) arr[k++] = F[j++];

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
    HashTable ht;
    for (int i = 0; i < MAX_HASH_SIZE; i++) {
        ht.tabela[i] = NULL;
    }

    for (int i = 0; i < n; i++) {
        insert_hash(&ht, cadastrados[i].codigo, i);
    }

    for (int i = 0; i < m; i++) {
        int indice = search_hash(&ht, selecionados[i].codigo);
        if (indice != -1) {
            selecionados[i].ordem = indice;

            if (strcmp(selecionados[i].cnpj, cadastrados[indice].cnpj) != 0) {
                selecionados[i].divergencia_cnpj = 1;
            } else {
                int diferenca = selecionados[i].peso - cadastrados[indice].peso;
                selecionados[i].peso_diferenca = abs(diferenca);
                selecionados[i].percentual_peso = percentual(cadastrados[indice].peso, selecionados[i].peso);
            }
        }
    }
    merge_sort(selecionados, 0, m - 1);
}

void exibir_resultados(FILE* output, Container cadastrados[], Container selecionados[], int m) {
    for (int i = 0; i < m; i++) {
        if (selecionados[i].divergencia_cnpj) {
            fprintf(output, "%s:%s<->%s\n", selecionados[i].codigo, cadastrados[selecionados[i].ordem].cnpj, selecionados[i].cnpj);
        } else {
            int percentual_arredondado = arredondar(selecionados[i].percentual_peso);
            if (percentual_arredondado > 10) {
                fprintf(output, "%s:%dkg(%d%%)\n", selecionados[i].codigo, selecionados[i].peso_diferenca, percentual_arredondado);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int n, m;
    fscanf(input, "%d", &n);

    Container *cadastrados = malloc(n * sizeof(Container));
    for (int i = 0; i < n; i++) {
        fscanf(input, "%s %s %d", cadastrados[i].codigo, cadastrados[i].cnpj, &cadastrados[i].peso);
    }

    fscanf(input, "%d", &m);

    Container *selecionados = malloc(m * sizeof(Container));
    for (int i = 0; i < m; i++) {
        fscanf(input, "%s %s %d", selecionados[i].codigo, selecionados[i].cnpj, &selecionados[i].peso);
    }

    processar_triagem(cadastrados, n, selecionados, m);
    exibir_resultados(output, cadastrados, selecionados, m);

    free(cadastrados);
    free(selecionados);
    fclose(input);
    fclose(output);
    return 0;
}
