#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct datagrama {
    int32_t valor;
    int32_t prioridade;
    char *rotulo;
};
void swap(struct datagrama *a, struct datagrama *b);
void trocar (int32_t *x, int32_t *y);
int32_t particao(struct datagrama *ordenacao, int32_t i, int32_t j);
int32_t mediana(int32_t *Vetor, int32_t i, int32_t j);
int32_t lomuto (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
int32_t hoare ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
int32_t lom_med ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
int32_t hoare_med ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
int32_t lom_aleat ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
int32_t hoare_aleat ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
void quickLP (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
void quickLM (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
void quickLA (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
void quickHP (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
void quickHM (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
void quickHA (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas);
void quickSaida(struct datagrama *ordenacao, int32_t i, int32_t j);

struct datagrama ordenacao[6];

int32_t main(int argc, char* argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int quantidade;

    fscanf(input, "%d", &quantidade);

    for(int i=0; i<quantidade; i++) {
        ordenacao[0].rotulo = "LP";
        ordenacao[1].rotulo = "LM";
        ordenacao[2].rotulo = "LA";
        ordenacao[3].rotulo = "HP";
        ordenacao[4].rotulo = "HM";
        ordenacao[5].rotulo = "HA";
        
        for (int j=0; j<6; j++) {
            ordenacao[j].valor = 1;
            ordenacao[j].prioridade = j;
        }

        int32_t n;        
        fscanf(input, "%d", &n);
        int32_t *vet = malloc(n*sizeof(int32_t));
        for(int j=0; j<n; j++) fscanf(input, "%d", &vet[j]);

        int32_t *vetorAux = malloc(n*sizeof(int32_t));
        for(int j=0; j<n; j++) vetorAux[j] = vet[j];
        
        quickLP(vetorAux, 0, n-1, &ordenacao[0].valor);
        for(int j=0; j<n; j++) vetorAux[j] = vet[j];
        quickLM(vetorAux, 0, n-1, &ordenacao[1].valor);
        for(int j=0; j<n; j++) vetorAux[j] = vet[j];
        quickLA(vetorAux, 0, n-1, &ordenacao[2].valor);
        for(int j=0; j<n; j++) vetorAux[j] = vet[j];
        quickHP(vetorAux, 0, n-1, &ordenacao[3].valor);
        for(int j=0; j<n; j++) vetorAux[j] = vet[j];
        quickHM(vetorAux, 0, n-1, &ordenacao[4].valor);
        for(int j=0; j<n; j++) vetorAux[j] = vet[j];
        quickHA(vet, 0, n-1, &ordenacao[5].valor);
        quickSaida(ordenacao, 0, 5);

        if(i<quantidade-1) fprintf(output, "%d:N(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d)\n", i, n, 
        ordenacao[0].rotulo, ordenacao[0].valor, 
        ordenacao[1].rotulo, ordenacao[1].valor, 
        ordenacao[2].rotulo, ordenacao[2].valor, 
        ordenacao[3].rotulo, ordenacao[3].valor, 
        ordenacao[4].rotulo, ordenacao[4].valor, 
        ordenacao[5].rotulo, ordenacao[5].valor);
        else fprintf(output, "%d:N(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d)", i, n, 
        ordenacao[0].rotulo, ordenacao[0].valor, 
        ordenacao[1].rotulo, ordenacao[1].valor, 
        ordenacao[2].rotulo, ordenacao[2].valor, 
        ordenacao[3].rotulo, ordenacao[3].valor, 
        ordenacao[4].rotulo, ordenacao[4].valor, 
        ordenacao[5].rotulo, ordenacao[5].valor);

        free(vet);
    }
}

void swap(struct datagrama *a, struct datagrama *b) {
    struct datagrama temp = *a;
    *a = *b;
    *b = temp;
}


void trocar (int32_t *x, int32_t *y) {
    int32_t aux;
    aux = *x;
    *x = *y;
    *y = aux;
}

int32_t particao(struct datagrama *ordenacao, int32_t i, int32_t j) {
    struct datagrama pivot = ordenacao[j];
    int32_t x = i - 1;
    for (int32_t y = i; y <= j; y++) {
        if (ordenacao[y].valor < pivot.valor || (ordenacao[y].valor == pivot.valor && ordenacao[y].prioridade < pivot.prioridade)) {
            x++;
            swap(&ordenacao[x], &ordenacao[y]);
        }
    }
    swap(&ordenacao[x + 1], &ordenacao[j]);
    return x + 1;
}

int32_t mediana(int32_t *Vetor, int32_t i, int32_t j) {
    int32_t v1 = Vetor[i+(j-i+1)/4], v2 = Vetor[i+(j-i+1)/2], v3 = Vetor[i+(j-i+1)*3/4];

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

int32_t lomuto (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    int32_t P = Vetor [j] , x = i - 1 , y = i;
    for ( y = i ; y < j ; y ++)
    if( Vetor [y] <= P ) {
        trocar (&Vetor[++x] , &Vetor[y]);
        *contadorTrocas = *contadorTrocas + 1;
    }
    trocar (&Vetor[++x] , &Vetor[j]);
    *contadorTrocas = *contadorTrocas + 1;
    return x ;
}


int32_t hoare ( int32_t *Vetor  , int32_t i, int32_t j, int32_t *contadorTrocas) {
    int32_t P = Vetor[i], x = i - 1 , y = j + 1;
    while (1) {
        while ( Vetor[--y] > P);
        while ( Vetor[++x] < P);
        if( x < y ) {
            trocar (&Vetor[x], &Vetor[y]);
            *contadorTrocas = *contadorTrocas + 1;
        }
        else return y;
    }
}


int32_t lom_med ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    int32_t med = mediana(Vetor, i, j);
    trocar (&Vetor[j] , &Vetor[med]) ;
    *contadorTrocas = *contadorTrocas + 1;
    return lomuto (Vetor, i, j, contadorTrocas) ;
}


int32_t hoare_med ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    int32_t med = mediana(Vetor, i, j);
    trocar (&Vetor[i] , &Vetor[med]);
    *contadorTrocas = *contadorTrocas + 1;
    return hoare (Vetor, i, j, contadorTrocas) ;
}


int32_t lom_aleat ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    trocar (&Vetor[j], &Vetor [i + abs(Vetor [i]) % (j-i+1)]);
    *contadorTrocas = *contadorTrocas + 1;
    return lomuto (Vetor, i, j, contadorTrocas) ;
}


int32_t hoare_aleat ( int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    trocar (&Vetor[i], &Vetor[i+abs(Vetor[i])%(j-i+1)]);
    *contadorTrocas = *contadorTrocas + 1;
    return hoare (Vetor, i, j, contadorTrocas) ;
}

void quickLP (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    if( i < j ) {
        int32_t pivo = lomuto(Vetor, i, j, contadorTrocas);
        quickLP(Vetor, i, pivo - 1, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
        quickLP(Vetor, pivo + 1 , j, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
    }
}

void quickLM (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    if( i < j ) {
        int32_t pivo = lom_med (Vetor, i, j, contadorTrocas) ;
        quickLM(Vetor, i, pivo - 1, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
        quickLM(Vetor, pivo + 1, j, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
    }
}

void quickLA (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    if( i < j ) {
        int32_t pivo = lom_aleat (Vetor, i, j, contadorTrocas);
        quickLA(Vetor, i, pivo - 1, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
        quickLA(Vetor, pivo + 1, j, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
    }
}

void quickHP (int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    if( i < j ) {
        int32_t pivo = hoare (Vetor, i, j, contadorTrocas);
        quickHP(Vetor, i, pivo, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
        quickHP(Vetor, pivo + 1, j, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
    }
}

void quickHM(int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    if( i < j ) {
        int32_t pivo = hoare_med (Vetor, i, j, contadorTrocas);
        quickHM(Vetor, i, pivo, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
        quickHM(Vetor, pivo + 1, j, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
    }
}

void quickHA(int32_t *Vetor, int32_t i, int32_t j, int32_t *contadorTrocas) {
    if( i < j ) {
        int32_t pivo = hoare_aleat (Vetor, i, j, contadorTrocas);
        quickHA(Vetor, i, pivo, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
        quickHA(Vetor, pivo + 1, j, contadorTrocas);
        *contadorTrocas = *contadorTrocas + 1;
    }
}


void quickSaida(struct datagrama *ordenacao, int32_t i, int32_t j) {
    if (i < j) {
        int32_t pivot = particao(ordenacao, i, j);
        quickSaida(ordenacao, i, pivot - 1);
        quickSaida(ordenacao, pivot + 1, j);
    }
}