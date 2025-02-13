#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct porto {
    char codigo[12];
    char cnpjCadastrado[19];
    char cnpj[19];
    int32_t peso;
    int32_t percentualDiferenca;
    int32_t pesoDiferenca;
    int32_t indiceOriginal;
    int32_t erro;
} porto;

//precisei colocar varios merge
void mesclarListas(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j);
int32_t procurarCodigoBinario(porto *indInicio, int32_t n, char *codigo);
void mergesortOrdenar(porto *indInicio, porto *indFim, int32_t i, int32_t j);
void duplicarElementos(porto *indInicio, porto *indFim, int32_t n);
void duplicarElementos2(porto *indInicio, porto *indFim, int32_t n);
void mergesortOrdenar2(porto *indInicio, porto *indFim, int32_t i, int32_t j);
void mesclarListas2(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j);
void mesclarListas3(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j);
void mergesortOrdenar3(porto *indInicio, porto *indFim, int32_t i, int32_t j);
void mesclarListas4(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j);
void mergesortOrdenar4(porto *indInicio, porto *indFim, int32_t i, int32_t j);


int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int32_t n;
    fscanf(input, "%d", &n);
    
    porto *portos = malloc(n*sizeof(porto));
    porto *portosOrdenados = malloc(n*sizeof(porto));

    for (int i=0; i<n; i++) {
        fscanf(input, "%s %s %d", portos[i].codigo, portos[i].cnpj, &portos[i].peso);
        portos[i].indiceOriginal = i;
    }

    mergesortOrdenar(portosOrdenados, portos, 0, n-1);

    int32_t m;
    fscanf(input, "%d", &m);
    porto *portosTemporarios = malloc(m*sizeof(porto));
    porto *portosTemporariosOrdenados = malloc(m*sizeof(porto));
    
    int teste = 0;
    for (int i=0; i<m; i++) {
        fscanf(input, "%s %s %d", portosTemporarios[i].codigo, portosTemporarios[i].cnpj, &portosTemporarios[i].peso);
        int32_t index = procurarCodigoBinario(portosOrdenados, n, portosTemporarios[i].codigo);
        if (index != -1) {
            if(strcmp(portosOrdenados[index].cnpj, portosTemporarios[i].cnpj) != 0) {
                portosTemporarios[i].indiceOriginal = portosOrdenados[index].indiceOriginal;
                strcpy(portosTemporarios[i].cnpjCadastrado, portosOrdenados[index].cnpj);
                portosTemporarios[i].erro = 1;
            }
            else if (((int) round(abs(portosOrdenados[index].peso - portosTemporarios[i].peso)*100/ (float) portosOrdenados[index].peso)) > 10) {
                portosTemporarios[i].indiceOriginal = portosOrdenados[index].indiceOriginal;
                portosTemporarios[i].erro = 2;
                portosTemporarios[i].percentualDiferenca = (int) round(abs(portosOrdenados[index].peso - portosTemporarios[i].peso)*100/ (float) portosOrdenados[index].peso);
                portosTemporarios[i].pesoDiferenca = abs(portosOrdenados[index].peso - portosTemporarios[i].peso);
            }
            else portosTemporarios[i].erro = 0;
        }
        else printf("%d\n", ++teste);
    }
    mergesortOrdenar2(portosTemporariosOrdenados, portosTemporarios, 0, m-1);

    int32_t a=-1, b=0, c=0;

    while (portosTemporariosOrdenados[++a].erro == 0) b++;
    c=b;
    while (portosTemporariosOrdenados[++a].erro == 1) c++;

    porto portosPorCNPJ[c-b+1], portosPorPeso[m-c-1], portosPorCNPJ_sort[c-b+1], portosPorPeso_sort[m-c-1];
    for (int i=0; i<c-b+1; i++) {
        strcpy(portosPorCNPJ[i].codigo, portosTemporariosOrdenados[b+i].codigo);
        strcpy(portosPorCNPJ[i].cnpj, portosTemporariosOrdenados[b+i].cnpj);
        portosPorCNPJ[i].peso = portosTemporariosOrdenados[b+i].peso;
        portosPorCNPJ[i].indiceOriginal = portosTemporariosOrdenados[b+i].indiceOriginal;
        portosPorCNPJ[i].erro = portosTemporariosOrdenados[b+i].erro;
        strcpy(portosPorCNPJ[i].cnpjCadastrado, portosTemporariosOrdenados[b+i].cnpjCadastrado);
        portosPorCNPJ[i].percentualDiferenca = portosTemporariosOrdenados[b+i].percentualDiferenca;
        portosPorCNPJ[i].pesoDiferenca = portosTemporariosOrdenados[b+i].pesoDiferenca;
    }
    for (int i=0; i<m-c-1; i++) {
        strcpy(portosPorPeso[i].codigo, portosTemporariosOrdenados[c+1+i].codigo);
        strcpy(portosPorPeso[i].cnpj, portosTemporariosOrdenados[c+1+i].cnpj);
        portosPorPeso[i].peso = portosTemporariosOrdenados[c+1+i].peso;
        portosPorPeso[i].indiceOriginal = portosTemporariosOrdenados[c+1+i].indiceOriginal;
        portosPorPeso[i].erro = portosTemporariosOrdenados[c+1+i].erro;
        strcpy(portosPorPeso[i].cnpjCadastrado, portosTemporariosOrdenados[c+1+i].cnpjCadastrado);
        portosPorPeso[i].percentualDiferenca = portosTemporariosOrdenados[c+1+i].percentualDiferenca;
        portosPorPeso[i].pesoDiferenca = portosTemporariosOrdenados[c+1+i].pesoDiferenca;
    }
    
    mergesortOrdenar3(portosPorCNPJ_sort, portosPorCNPJ, 0, c-b);
    mergesortOrdenar4(portosPorPeso_sort, portosPorPeso, 0, m-c-2);

    for (int i=0; i<c-b+1; i++){
        fprintf(output, "%s:%s<->%s\n", portosPorCNPJ_sort[i].codigo, portosPorCNPJ_sort[i].cnpjCadastrado, portosPorCNPJ_sort[i].cnpj);
    }

    for (int i=0; i<m-c-1; i++){
        if(i==m-c-2) fprintf(output, "%s:%dkg (%d%%)", portosPorPeso_sort[i].codigo, portosPorPeso_sort[i].pesoDiferenca, portosPorPeso_sort[i].percentualDiferenca);
        else fprintf(output, "%s:%dkg(%d%%)\n", portosPorPeso_sort[i].codigo, portosPorPeso_sort[i].pesoDiferenca, portosPorPeso_sort[i].percentualDiferenca);
    }

}   

void mesclarListas(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j){    
    int32_t indPrimario = i, indSecundario = m + 1, k = i;
    while (indPrimario <= m && indSecundario <= j) {
        if (strcmp(indFim[indPrimario].codigo, indFim[indSecundario].codigo)<=0) {
            strcpy(indInicio[k].codigo, indFim[indPrimario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indPrimario].cnpj);
            indInicio[k].peso = indFim[indPrimario].peso;
            indInicio[k++].indiceOriginal = indFim[indPrimario++].indiceOriginal;
        }
        else {
            strcpy(indInicio[k].codigo, indFim[indSecundario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indSecundario].cnpj);
            indInicio[k].peso = indFim[indSecundario].peso;
            indInicio[k++].indiceOriginal = indFim[indSecundario++].indiceOriginal;
        }
    }
    if (indPrimario > m) duplicarElementos(&indInicio[k], &indFim[indSecundario], j - indSecundario + 1);
    else duplicarElementos(&indInicio[k], &indFim[indPrimario], m - indPrimario + 1);
    duplicarElementos(&indFim[i], &indInicio[i], j - i + 1);
}


int32_t procurarCodigoBinario(porto *indInicio, int32_t n, char *codigo) {
    int32_t i = 0, j = n - 1, m;
    while (i <= j) {
        m = i + (j - i) / 2;
        if (strcmp(indInicio[m].codigo, codigo) == 0) return m;
        else if (strcmp(indInicio[m].codigo, codigo) < 0) i = m + 1;
        else j = m - 1;
    }
    return -1;
}


void mergesortOrdenar(porto *indInicio, porto *indFim, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesortOrdenar(indInicio, indFim, i, m);
        mergesortOrdenar(indInicio, indFim, m + 1, j);
        mesclarListas(indInicio, indFim, i, m, j);
    }
}


void duplicarElementos(porto *indInicio, porto *indFim, int32_t n) {
    for (int32_t i = 0; i < n; i++) {
        strcpy(indInicio[i].codigo, indFim[i].codigo);
        strcpy(indInicio[i].cnpj, indFim[i].cnpj);
        indInicio[i].peso = indFim[i].peso;
        indInicio[i].indiceOriginal = indFim[i].indiceOriginal;
    }
}


void duplicarElementos2(porto *indInicio, porto *indFim, int32_t n) {
    for (int32_t i = 0; i < n; i++) {
        strcpy(indInicio[i].codigo, indFim[i].codigo);
        strcpy(indInicio[i].cnpj, indFim[i].cnpj);
        indInicio[i].peso = indFim[i].peso;
        strcpy(indInicio[i].cnpjCadastrado, indFim[i].cnpjCadastrado);
        indInicio[i].percentualDiferenca = indFim[i].percentualDiferenca;
        indInicio[i].pesoDiferenca = indFim[i].pesoDiferenca;
        indInicio[i].erro = indFim[i].erro;
        indInicio[i].indiceOriginal = indFim[i].indiceOriginal;
    }
}


void mergesortOrdenar2(porto *indInicio, porto *indFim, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesortOrdenar2(indInicio, indFim, i, m);
        mergesortOrdenar2(indInicio, indFim, m + 1, j);
        mesclarListas2(indInicio, indFim, i, m, j);
    }
}


void mesclarListas2(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j){    
    int32_t indPrimario = i, indSecundario = m + 1, k = i;
    while (indPrimario <= m && indSecundario <= j) {
        if (indFim[indPrimario].erro<indFim[indSecundario].erro) {
            strcpy(indInicio[k].codigo, indFim[indPrimario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indPrimario].cnpj);
            indInicio[k].peso = indFim[indPrimario].peso;
            strcpy(indInicio[k].cnpjCadastrado, indFim[indPrimario].cnpjCadastrado);
            indInicio[k].percentualDiferenca = indFim[indPrimario].percentualDiferenca;
            indInicio[k].pesoDiferenca = indFim[indPrimario].pesoDiferenca;
            indInicio[k].erro = indFim[indPrimario].erro;
            indInicio[k++].indiceOriginal = indFim[indPrimario++].indiceOriginal;
        }
        else {
            strcpy(indInicio[k].codigo, indFim[indSecundario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indSecundario].cnpj);
            indInicio[k].peso = indFim[indSecundario].peso;
            strcpy(indInicio[k].cnpjCadastrado, indFim[indSecundario].cnpjCadastrado);
            indInicio[k].percentualDiferenca = indFim[indSecundario].percentualDiferenca;
            indInicio[k].pesoDiferenca = indFim[indSecundario].pesoDiferenca;
            indInicio[k].erro = indFim[indSecundario].erro;
            indInicio[k++].indiceOriginal = indFim[indSecundario++].indiceOriginal;
        }
    }
    if (indPrimario > m) duplicarElementos2(&indInicio[k], &indFim[indSecundario], j - indSecundario + 1);
    else duplicarElementos2(&indInicio[k], &indFim[indPrimario], m - indPrimario + 1);
    duplicarElementos2(&indFim[i], &indInicio[i], j - i + 1);
}


void mesclarListas3(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j){    
    int32_t indPrimario = i, indSecundario = m + 1, k = i;
    while (indPrimario <= m && indSecundario <= j) {
        if (indFim[indPrimario].indiceOriginal<indFim[indSecundario].indiceOriginal) {
            strcpy(indInicio[k].codigo, indFim[indPrimario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indPrimario].cnpj);
            indInicio[k].peso = indFim[indPrimario].peso;
            strcpy(indInicio[k].cnpjCadastrado, indFim[indPrimario].cnpjCadastrado);
            indInicio[k].percentualDiferenca = indFim[indPrimario].percentualDiferenca;
            indInicio[k].pesoDiferenca = indFim[indPrimario].pesoDiferenca;
            indInicio[k].erro = indFim[indPrimario].erro;
            indInicio[k++].indiceOriginal = indFim[indPrimario++].indiceOriginal;
        }
        else {
            strcpy(indInicio[k].codigo, indFim[indSecundario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indSecundario].cnpj);
            indInicio[k].peso = indFim[indSecundario].peso;
            strcpy(indInicio[k].cnpjCadastrado, indFim[indSecundario].cnpjCadastrado);
            indInicio[k].percentualDiferenca = indFim[indSecundario].percentualDiferenca;
            indInicio[k].pesoDiferenca = indFim[indSecundario].pesoDiferenca;
            indInicio[k].erro = indFim[indSecundario].erro;
            indInicio[k++].indiceOriginal = indFim[indSecundario++].indiceOriginal;
        }
    }
    if (indPrimario > m) duplicarElementos2(&indInicio[k], &indFim[indSecundario], j - indSecundario + 1);
    else duplicarElementos2(&indInicio[k], &indFim[indPrimario], m - indPrimario + 1);
    duplicarElementos2(&indFim[i], &indInicio[i], j - i + 1);
}

void mergesortOrdenar3(porto *indInicio, porto *indFim, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesortOrdenar3(indInicio, indFim, i, m);
        mergesortOrdenar3(indInicio, indFim, m + 1, j);
        mesclarListas3(indInicio, indFim, i, m, j);
    }
}


void mesclarListas4(porto *indInicio, porto *indFim, int32_t i, int32_t m, int32_t j){    
    int32_t indPrimario = i, indSecundario = m + 1, k = i;
    while (indPrimario <= m && indSecundario <= j) {
        if (indFim[indPrimario].percentualDiferenca==indFim[indSecundario].percentualDiferenca) {
            if (indFim[indPrimario].indiceOriginal<indFim[indSecundario].indiceOriginal) {
                strcpy(indInicio[k].codigo, indFim[indPrimario].codigo);
                strcpy(indInicio[k].cnpj, indFim[indPrimario].cnpj);
                indInicio[k].peso = indFim[indPrimario].peso;
                strcpy(indInicio[k].cnpjCadastrado, indFim[indPrimario].cnpjCadastrado);
                indInicio[k].percentualDiferenca = indFim[indPrimario].percentualDiferenca;
                indInicio[k].pesoDiferenca = indFim[indPrimario].pesoDiferenca;
                indInicio[k].erro = indFim[indPrimario].erro;
                indInicio[k++].indiceOriginal = indFim[indPrimario++].indiceOriginal;
            }
            else {
                strcpy(indInicio[k].codigo, indFim[indSecundario].codigo);
                strcpy(indInicio[k].cnpj, indFim[indSecundario].cnpj);
                indInicio[k].peso = indFim[indSecundario].peso;
                strcpy(indInicio[k].cnpjCadastrado, indFim[indSecundario].cnpjCadastrado);
                indInicio[k].percentualDiferenca = indFim[indSecundario].percentualDiferenca;
                indInicio[k].pesoDiferenca = indFim[indSecundario].pesoDiferenca;
                indInicio[k].erro = indFim[indSecundario].erro;
                indInicio[k++].indiceOriginal = indFim[indSecundario++].indiceOriginal;
            }
        } else if (indFim[indPrimario].percentualDiferenca>indFim[indSecundario].percentualDiferenca) {
            strcpy(indInicio[k].codigo, indFim[indPrimario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indPrimario].cnpj);
            indInicio[k].peso = indFim[indPrimario].peso;
            strcpy(indInicio[k].cnpjCadastrado, indFim[indPrimario].cnpjCadastrado);
            indInicio[k].percentualDiferenca = indFim[indPrimario].percentualDiferenca;
            indInicio[k].pesoDiferenca = indFim[indPrimario].pesoDiferenca;
            indInicio[k].erro = indFim[indPrimario].erro;
            indInicio[k++].indiceOriginal = indFim[indPrimario++].indiceOriginal;
        } else {
            strcpy(indInicio[k].codigo, indFim[indSecundario].codigo);
            strcpy(indInicio[k].cnpj, indFim[indSecundario].cnpj);
            indInicio[k].peso = indFim[indSecundario].peso;
            strcpy(indInicio[k].cnpjCadastrado, indFim[indSecundario].cnpjCadastrado);
            indInicio[k].percentualDiferenca = indFim[indSecundario].percentualDiferenca;
            indInicio[k].pesoDiferenca = indFim[indSecundario].pesoDiferenca;
            indInicio[k].erro = indFim[indSecundario].erro;
            indInicio[k++].indiceOriginal = indFim[indSecundario++].indiceOriginal;
        }
    }
    if (indPrimario > m) duplicarElementos2(&indInicio[k], &indFim[indSecundario], j - indSecundario + 1);
    else duplicarElementos2(&indInicio[k], &indFim[indPrimario], m - indPrimario + 1);
    duplicarElementos2(&indFim[i], &indInicio[i], j - i + 1);
}


void mergesortOrdenar4(porto *indInicio, porto *indFim, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesortOrdenar4(indInicio, indFim, i, m);
        mergesortOrdenar4(indInicio, indFim, m + 1, j);
        mesclarListas4(indInicio, indFim, i, m, j);
    }
}
