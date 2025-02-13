#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct data {
    int32_t valor;
    int32_t prioridade;
    char *rotulo;
};

int32_t lomuto (int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t LM ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t LA ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quicksLP (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quickLM (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quickLA (int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t hoare ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t LM ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
int32_t LA ( int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quickHP (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quickHM (int32_t * V , int32_t i , int32_t j, int32_t *cont);
void quickHA (int32_t * V , int32_t i , int32_t j, int32_t *cont);


void printVector (int32_t *v, int32_t n, char *s);
void trocar (int32_t *x, int32_t *y);
int32_t mediana(int32_t *V, int32_t i, int32_t j);
int32_t aleatorio(int32_t *V, int32_t i, int32_t j);
void quickSortOutput(struct data *lista, int32_t i, int32_t j);
int32_t particao(struct data *lista, int32_t i, int32_t j);
void swap(struct data *a, struct data *b);

struct data lista[6];

int32_t main(int argc, char* argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int quantidade;

    fscanf(input, "%d", &quantidade);

    for(int i=0; i<quantidade; i++) {
        lista[0].rotulo = "LP";
        lista[1].rotulo = "LM";
        lista[2].rotulo = "LA";
        lista[3].rotulo = "HP";
        lista[4].rotulo = "HM";
        lista[5].rotulo = "HA";
        
        for (int j=0; j<6; j++) {
            lista[j].valor = 1;
            lista[j].prioridade = j;
        }

        int32_t n;        
        fscanf(input, "%d", &n);
        int32_t *v = malloc(n*sizeof(int32_t));

        for(int j=0; j<n; j++) fscanf(input, "%d", &v[j]);

        int32_t *vAux = malloc(n*sizeof(int32_t));
        for(int j=0; j<n; j++) vAux[j] = v[j];
        
        quicksLP(vAux, 0, n-1, &lista[0].valor);
       
        for(int j=0; j<n; j++) vAux[j] = v[j];
        quickLM(vAux, 0, n-1, &lista[1].valor);
        for(int j=0; j<n; j++) vAux[j] = v[j];
        quickLA(vAux, 0, n-1, &lista[2].valor);
        for(int j=0; j<n; j++) vAux[j] = v[j];
        quickHP(vAux, 0, n-1, &lista[3].valor);
        for(int j=0; j<n; j++) vAux[j] = v[j];
        quickHM(vAux, 0, n-1, &lista[4].valor);
        for(int j=0; j<n; j++) vAux[j] = v[j];
        quickHA(v, 0, n-1, &lista[5].valor);
       
        quickSortOutput(lista, 0, 5);

        if(i<quantidade-1) fprintf(output, "%d:N(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d)\n", i, n, lista[0].rotulo, lista[0].valor, lista[1].rotulo, lista[1].valor, lista[2].rotulo, lista[2].valor, lista[3].rotulo, lista[3].valor, lista[4].rotulo, lista[4].valor, lista[5].rotulo, lista[5].valor);
        else fprintf(output, "%d:N(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d)", i, n, lista[0].rotulo, lista[0].valor, lista[1].rotulo, lista[1].valor, lista[2].rotulo, lista[2].valor, lista[3].rotulo, lista[3].valor, lista[4].rotulo, lista[4].valor, lista[5].rotulo, lista[5].valor);
        free(v);
    }
}

int32_t lomuto (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Declaração do pivô e índices
    int32_t P = V [ j ] , x = i - 1 , y = i;
    // Particionando o vetor pelo pivô
    for ( y = i ; y < j ; y ++)
    if( V [ y ] <= P ) {
        trocar (& V [++ x ] , & V [y]);
        *cont = *cont + 1;
    }
    // Posicionando o pivô no vetor
    trocar (& V [++ x ] , & V [ j ]);
    *cont = *cont + 1;
    // Retornando índice do pivô
    return x ;
}

void quicksLP (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = lomuto(V , i , j, cont);
        // Divisão em subvetores
        quicksLP(V , i , pivo - 1, cont);
        *cont = *cont + 1;
        quicksLP(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t LM ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatorioório
    int32_t med = mediana(V, i, j);
    
    trocar (& V [ j ] , & V [med]) ;
    *cont = *cont + 1;
    // Chamada do particionamento
    return lomuto (V , i , j, cont) ;
}

void quickLM (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = LM (V , i , j, cont) ;
        // Divisão em subvetores
        quickLM(V , i , pivo - 1, cont);
        *cont = *cont + 1;
        quickLM(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t LA ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatorioório
    trocar (& V [ j ] , & V [i + abs(V [i]) % (j-i+1)]);
    *cont = *cont + 1;
    // Chamada do particionamento
    return lomuto (V , i , j, cont) ;
}

void quickLA (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = LA (V , i , j, cont);
        // Divisão em subvetores
        quickLA(V , i , pivo - 1, cont);
        *cont = *cont + 1;
        quickLA(V , pivo + 1 , j, cont);
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
            trocar (& V [ x ] , & V [ y ]);
            *cont = *cont + 1;
        }
        else return y;
    }
}
// 955 -32 1 9
void quickHP (int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = hoare (V , i , j, cont);
        // Divisão em subvetores
        quickHP(V , i , pivo, cont);
        *cont = *cont + 1;
        quickHP(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t LM ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatorioório
    int32_t med = mediana(V, i, j);

    trocar (& V [ i ] , & V [ med ]);
    *cont = *cont + 1;
    // Chamada do particionamento
    return hoare (V , i , j, cont) ;
}

void quickHM(int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = LM (V , i , j, cont);
        // Divisão em subvetores
        quickHM(V , i , pivo, cont);
        *cont = *cont + 1;
        quickHM(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

int32_t LA ( int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Troca do pivô por aleatorioório
    trocar (& V [ i ] , & V [i + abs(V[i]) % (j-i+1)]);
    *cont = *cont + 1;
    // Chamada do particionamento
    return hoare (V , i , j, cont) ;
}

void quickHA(int32_t * V , int32_t i , int32_t j, int32_t *cont) {
    // Caso base
    if( i < j ) {
        // Particionamento do vetor
        int32_t pivo = LA (V , i , j, cont);
        // Divisão em subvetores
        quickHA(V , i , pivo, cont);
        *cont = *cont + 1;
        quickHA(V , pivo + 1 , j, cont);
        *cont = *cont + 1;
    }
}

void trocar (int32_t *x, int32_t *y) {
    int32_t aux;
    aux = *x;
    *x = *y;
    *y = aux;
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

void printVector (int32_t *v, int32_t n, char *s) {
    printf("%s: ", s);
    for(int i=0; i<n; i++) {
        printf("%d, ", v[i]);
    }
    printf("\n-----------------------------------\n");
}

void quickSortOutput(struct data *lista, int32_t i, int32_t j) {
    if (i < j) {
        int32_t pivot = particao(lista, i, j);
        quickSortOutput(lista, i, pivot - 1);
        quickSortOutput(lista, pivot + 1, j);
    }
}

int32_t particao(struct data *lista, int32_t i, int32_t j) {
    struct data pivot = lista[j];
    int32_t x = i - 1;

    for (int32_t y = i; y <= j; y++) {
        if (lista[y].valor < pivot.valor || (lista[y].valor == pivot.valor && lista[y].prioridade < pivot.prioridade)) {
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