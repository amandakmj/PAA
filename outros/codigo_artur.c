#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MAX_CONTAINERS 1000

typedef struct {
    char codigo[20];
    char cnpj[20];
    int peso;
    int indiceOriginal;
} Container;

Container bancoDeDados[] = {
        {"QOZJ7913219", "34.699.211/9365-11", 13822, 0, 0.0},
        {"FCCU4584578", "50.503.434/5731-28", 16022, 0, 0.0},
        {"KTAJ0603546", "20.500.522/6013-58", 25279, 0, 0.0},
        {"ZYHU3978783", "43.172.263/4442-14", 24543, 0, 0.0},
        {"IKQZ7582839", "51.743.446/1183-18", 12160, 0, 0.0},
        {"HAAZ0273059", "25.699.428/4746-79", 16644, 0, 0.0}
    };

Container entradaDeDados[] = {
        {"ZYHU3978783", "43.172.263/4442-14", 29765, 0, 0.0},
        {"IKQZ7582839", "51.743.446/1113-18", 18501, 0, 0.0},
        {"KTAJ0603546", "20.500.522/6113-58", 17842, 0, 0.0},
        {"QOZJ7913219", "34.699.211/9365-11", 16722, 0, 0.0},
        {"FCCU4584578", "50.503.434/5731-28", 16398, 0, 0.0}
    };
    
int ehIgualCNPJ(const char *valorDoBanco, const char *valorDaEntrada) {
    if (strcmp(valorDoBanco, valorDaEntrada) == 0) {
        return 1;
    }
    return 0;
}

int ehIgualPeso(int valorDoBanco, int valorDaEntrada) {
    if(valorDoBanco != valorDaEntrada) {
        return 0
    }
    return 1;
}

Container* comparaCNPJ(Container* banco, Container* entrada, int tamanhoBanco, int tamanhoEntrada, int* resultadoTamanho) {

    Container* resultado = (Container*)malloc(tamanhoBanco * tamanhoEntrada * sizeof(Container));
    if (!resultado) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    int pos = 0;
    for (int i = 0; i < tamanhoEntrada; i++) {
        for (int j = 0; j < tamanhoBanco; j++) {
            if (!ehIgualCNPJ(banco[j].cnpj, entrada[i].cnpj)) {
                resultado[pos] = banco[j];
                pos++;
            }
        }
    }

    *resultadoTamanho = pos;
    return resultado; 
}

Container* comparaPeso(Container* banco, Container* entrada, int tamanhoBanco, int tamanhoEntrada, int* resultadoTamanho) {

    Container* resultado = (Container*)malloc(tamanhoBanco * tamanhoEntrada * sizeof(Container));
    if (!resultado) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    int pos = 0;
    for (int i = 0; i < tamanhoEntrada; i++) {
        for (int j = 0; j < tamanhoBanco; j++) {
            if (!ehIgualPeso(banco[j].peso, entrada[i].peso)) {
                resultado[pos] = banco[j];
                pos++;
            }
        }
    }

    *resultadoTamanho = pos;
    return resultado; 
}

int comparaPorIndiceOriginal(const void *a, const void *b) {
    Container *c1 = (Container *)a;
    Container *c2 = (Container *)b;
    return c1->indiceOriginal - c2->indiceOriginal;
}


void mergeWithId(Container arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Container leftArr[n1], rightArr[n2];

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

   
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].indiceOriginal <= rightArr[j].indiceOriginal) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void mergeWithWeight(Container arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Container leftArr[n1], rightArr[n2];

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

   
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].peso < rightArr[j].peso) {
            arr[k] = leftArr[i];
            i++;
        } if (leftArr[i].peso > rightArr[j].peso) {
            arr[k] = rightArr[j];
            j++;
        } if (leftArr[i].peso == rightArr[j].peso) {
    if (leftArr[i].indiceOriginal <= rightArr[j].indiceOriginal) {
        arr[k] = leftArr[i];
        i++;
    } else {
        arr[k] = rightArr[j];
        j++;
    }
}

        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}



void mergeSortId(Container arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortId(arr, left, mid);
        mergeSortId(arr, mid + 1, right);

        mergeWithId(arr, left, mid, right);
    }
}

void mergeSortWeight(Container arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortWeight(arr, left, mid);
        mergeSortWeight(arr, mid + 1, right);

        mergeWithWeight(arr, left, mid, right);
    }
}