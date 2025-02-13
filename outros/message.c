#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct data {
    int32_t value;
    int32_t priority;
};

void quicksort_LP (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quicksort_LM (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quicksort_LA (int32_t * V , int32_t i , int32_t j, int32_t *cont);

int32_t lomuto (int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t lomuto_med ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t lomuto_rand ( int32_t * V , int32_t i , int32_t j, int32_t *cont);

void quicksort_HP (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quicksort_HM (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quicksort_HA (int32_t * V , int32_t i , int32_t j, int32_t *cont);

int32_t hoare ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t hoare_med ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t hoare_rand ( int32_t * V , int32_t i , int32_t j, int32_t *cont);

void trocar (void *x, void *y, size_t tamanho);
int32_t mediana(int32_t *V, int32_t i, int32_t j);
void quickSortOutput(struct data *lista, int32_t i, int32_t j);
int32_t partition(struct data *lista, int32_t i, int32_t j);
void swap(struct data *a, struct data *b);

struct data lista[6];

int32_t main(int argc, char* argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int quant;

    fscanf(input, "%d", &quant);

    for(int i=0; i<quant; i++) {
        
        for (int j=0; j<6; j++) {
            lista[j].value = 1;
            lista[j].priority = j;
        }

        int32_t n;        
        fscanf(input, "%d", &n);
        int32_t *v = malloc(n*sizeof(int32_t));

        for(int j=0; j<n; j++) fscanf(input, "%d", &v[j]);

        int32_t *vAux = malloc(n*sizeof(int32_t));
        for(int j=0; j<n; j++) vAux[j] = v[j];
        
        quicksort_LP(vAux, 0, n-1, &lista[0].value);
        for(int j=0; j<n; j++) vAux[j] = v[j];

        quicksort_LM(vAux, 0, n-1, &lista[1].value);
        for(int j=0; j<n; j++) vAux[j] = v[j];
        
        quicksort_LA(vAux, 0, n-1, &lista[2].value);
        for(int j=0; j<n; j++) vAux[j] = v[j];

        quicksort_HP(vAux, 0, n-1, &lista[3].value);
        for(int j=0; j<n; j++) vAux[j] = v[j];

        quicksort_HM(vAux, 0, n-1, &lista[4].value);
        for(int j=0; j<n; j++) vAux[j] = v[j];

        quicksort_HA(v, 0, n-1, &lista[5].value);
        quickSortOutput(lista, 0, 5);

        if(i<quant-1) fprintf(output, "%d: N(%d) LP(%d) LM(%d) LA(%d) HP(%d) HM(%d) HA(%d)\n", i, n, lista[0].value, lista[1].value, lista[2].value, lista[3].value, lista[4].value, lista[5].value);
        else fprintf(output, "%d: N(%d) LP(%d) LM(%d) LA(%d) HP(%d) HM(%d) HA(%d)", i, n, lista[0].value, lista[1].value, lista[2].value, lista[3].value, lista[4].value, lista[5].value);
        free(v);
    }
}

int32_t lomuto (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Declaração do pivô e índices
    int32_t P = V [ j ] , x = i - 1 , y = i;
    // Particionando o vetor pelo pivô
    for ( y = i ; y < j ; y ++)
    if( V [ y ] <= P ) {
        trocar (& V [++ x ] , & V [y], sizeof(int32_t));
        *cont = *cont + 1;
    }
    // Posicionando o pivô no vetor
    trocar (& V [++ x ] , & V [ j ], sizeof(int32_t));
    *cont = *cont + 1;
    // Retornando índice do pivô
    return x ;
}

void quicksort_LP (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = lomuto(V , i , j, cont);
        // Divisão em subvetores
        quicksort_LP(V , i , pivo - 1, cont);
        *cont = *cont + 1;
        quicksort_LP(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t lomuto_med ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatório
    int32_t med = mediana(V, i, j);
    
    trocar (& V [ j ] , & V [med], sizeof(int32_t)) ;
    *cont = *cont + 1;
    // Chamada do particionamento
    return lomuto (V , i , j, cont) ;
}

void quicksort_LM (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = lomuto_med (V , i , j, cont) ;
        // Divisão em subvetores
        quicksort_LM(V , i , pivo - 1, cont);
        *cont = *cont + 1;
        quicksort_LM(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t lomuto_rand ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatório
    trocar (& V [ j ] , & V [i + abs(V [i]) % (j-i+1)], sizeof(int32_t));
    *cont = *cont + 1;
    // Chamada do particionamento
    return lomuto (V , i , j, cont) ;
}

void quicksort_LA (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = lomuto_rand (V , i , j, cont);
        // Divisão em subvetores
        quicksort_LA(V , i , pivo - 1, cont);
        *cont = *cont + 1;
        quicksort_LA(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t hoare ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Declaração do pivô e índices
    int32_t P = V [ i ] , x = i - 1 , y = j + 1;
    // Particionando o vetor pelo pivô
    while (1) {
        while ( V [--y] > P);
        while ( V [++x] < P);
        if( x < y ) {
            trocar (& V [ x ] , & V [ y ], sizeof(int32_t));
            *cont = *cont + 1;
        }
        else return y;
    }
}
// 955 -32 1 9
void quicksort_HP (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = hoare (V , i , j, cont);
        // Divisão em subvetores
        quicksort_HP(V , i , pivo, cont);
        *cont = *cont + 1;
        quicksort_HP(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t hoare_med ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatório
    int32_t med = mediana(V, i, j);

    trocar (& V [ i ] , & V [ med ], sizeof(int32_t));
    *cont = *cont + 1;
    // Chamada do particionamento
    return hoare (V , i , j, cont) ;
}

void quicksort_HM(int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = hoare_med (V , i , j, cont);
        // Divisão em subvetores
        quicksort_HM(V , i , pivo, cont);
        *cont = *cont + 1;
        quicksort_HM(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t hoare_rand ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatório
    trocar (& V [ i ] , & V [i + abs(V[i]) % (j-i+1)], sizeof(int32_t));
    *cont = *cont + 1;
    // Chamada do particionamento
    return hoare (V , i , j, cont) ;
}

void quicksort_HA(int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = hoare_rand (V , i , j, cont);
        // Divisão em subvetores
        quicksort_HA(V , i , pivo, cont);
        *cont = *cont + 1;
        quicksort_HA(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

void trocar(void *a, void *b, size_t tamanho) {
    void *temp = malloc(tamanho); 
    if (temp == NULL) return;      
    
    memcpy(temp, a, tamanho);      
    memcpy(a, b, tamanho);         
    memcpy(b, temp, tamanho);      

    free(temp);                   
}

int32_t mediana(int32_t *V, int32_t i, int32_t j) {
    int32_t v1 = V[i+(j-i+1)/4], v2 = V[i+(j-i+1)/2], v3 = V[i+(j-i+1)*3/4];

    if(v1==v3 || v1==v2){
        return i+(j-i+1)/4;
    } else if(v2==v3){
        return i+(j-i+1)/2;
    } else if(v1<v2) {
        if(v1>v3) return i+(j-i+1)/4;
        else if (v2<v3) return i+(j-i+1)/2;
        else return i+(j-i+1)*3/4;
    } else {
        if(v2>v3) return i+(j-i+1)/2;
        else if (v1<v3) return i+(j-i+1)/4;
        else return i+(j-i+1)*3/4;
    }
}

void quickSortOutput(struct data *lista, int32_t i, int32_t j) {
    if (i < j) {
        int32_t pivot = partition(lista, i, j);
        quickSortOutput(lista, i, pivot - 1);
        quickSortOutput(lista, pivot + 1, j);
    }
}

int32_t partition(struct data *lista, int32_t i, int32_t j) {
    struct data pivot = lista[j];
    int32_t x = i - 1;

    for (int32_t y = i; y <= j; y++) {
        if (lista[y].value < pivot.value || (lista[y].value == pivot.value && lista[y].priority < pivot.priority)) {
            x++;
            swap(&lista[x], &lista[y]);
        }
    }
    swap(&lista[x + 1], &lista[j]);
    return x + 1;
}

void swap(struct data *a, struct data *b) {
    struct data temp = *a;
    *a = *b;
    *b = temp;
}