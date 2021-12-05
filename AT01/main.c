#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <stdbool.h>
#include <math.h>

int *criaVetor(int *vetor, int tamVetor, char metodo, FILE *arqSaida){
    int i, j;
    vetor = (int*) malloc(tamVetor * sizeof(int));

    // c = crescente sequencialmente de 1 a tamVetor.
    if (metodo == 'c'){
        for (i=0, j=1; i<tamVetor; i++, j++){
            vetor[i] = j;
        }
        // c = decrescente sequencialmente de tamVetor a 1.
    } else if (metodo == 'd'){
        for (i=0, j=tamVetor; i<tamVetor; i++, j--){
            vetor[i]= j;
        }
        // r = randômico, com os valores entre 0 e 32000;
    } else if (metodo == 'r'){
        srand(time(NULL));

        for(i=0; i<tamVetor; i++){
            vetor[i] = rand() % 32000;
        }

    } else {
        printf("Erro: Arquivo fora do padrao esperado!\n");
        fprintf(arqSaida, "Arquivo Inválido!");
        exit(1);
    }

    return vetor;
}

void copiaVetor(int *vetorOriginal, int *vetorCopia, int tamVetor){
    for(int i = 0; i<tamVetor; i++){
        vetorCopia[i] = vetorOriginal[i];
    }
}

void imprimeVetor(int *vetor, int tamVetor, FILE *arqSaida){
    for (int i = 0; i < tamVetor; i++){
        fprintf(arqSaida, " %d", vetor[i]);
    }
    fprintf(arqSaida, ", ");
}

void insertionSort(int *vetor, int tamVetor, int *comparacoes){
    int i, j, aux;
    for (i = 1; i < tamVetor; i++){
        aux = vetor[i];
        j = i-1;

        while ((j >= 0) && (aux < vetor[j])){
            vetor[j+1] = vetor[j];
            j = j-1;
        }
        vetor[j+1] = aux;
        (*comparacoes)++;
    }
}

void selectionSort(int *vetor, int tamVetor, int *comparacoes) {
    int i, j, min, aux;
    for (i = 0; i < tamVetor-1; i++) {
        min = i;
        for (j = i + 1; j < tamVetor; j++) {
            if (vetor[j] < vetor[min]) {
                min = j;
            }
        }
        if (i != min) {
            aux = vetor[i];
            vetor[i] = vetor[min];
            vetor[min] = aux;
        }
        (*comparacoes)++;
    }
}

void bubbleSort(int *vetor, int tamVetor, int *comparacoes){
    bool trocou = true;
    int aux;
    while(trocou){
        trocou = false;
        for (int i = 0; i < tamVetor-1; i++){
            if (vetor[i] > vetor[i+1]){
                aux = vetor[i];
                vetor[i] = vetor[i+1];
                vetor[i+1] = aux;
                trocou = true;
            }
            (*comparacoes)++;
        }
    }
}

void merge(int *vetor, int inicio, int meio, int fim, int *comparacoes) {
    int *aux;
    int i, j, k;
    int p1, p2;
    int tamVetor;

    bool finalizado1 = false;
    bool finalizado2 = false;

    tamVetor = (fim - inicio) + 1;

    p1 = inicio;
    p2 = meio + 1;

    aux = (int*) malloc(tamVetor * sizeof(int));

    if (aux != NULL) {
        for (i = 0; i < tamVetor; i++) {
            if (!finalizado1 && !finalizado2) {
                if (vetor[p1] < vetor[p2]) {
                    aux[i] = vetor[p1++];
                    (*comparacoes)++;
                } else {
                    aux[i] = vetor[p2++];
                    (*comparacoes)++;
                }

                if (p1 > meio) finalizado1 = true;
                if (p2 > fim) finalizado2 = true;

            } else {
                if (!finalizado1) {
                    aux[i] = vetor[p1++];
                    (*comparacoes)++;
                } else {
                    aux[i] = vetor[p2++];
                    (*comparacoes)++;
                }
            }
        }
        for (j = 0, k = inicio; j < tamVetor; j++, k++) {
            vetor[k] = aux[j];
        }
    }
    free(aux);
}

void mergeSort(int *vetor, int inicio, int fim, int *comparacoes){
    int meio;
    if (inicio < fim) {
        meio = (int) floor((inicio + fim) / 2);
        mergeSort(vetor, inicio, meio, comparacoes);
        mergeSort(vetor, meio + 1, fim, comparacoes);
        merge(vetor, inicio, meio, fim, comparacoes);
    }
}

int particiona(int *vetor, int inicio, int fim, int *comparacoes){
    int esq = inicio;
    int dir = fim;
    int pivo = vetor[inicio];
    int aux;

    while (esq < dir){
        while (vetor[esq] <= pivo && esq <= fim){
            esq++;
            (*comparacoes)++;
        }
        while (vetor[dir] > pivo && dir >= inicio){
            dir--;
            (*comparacoes)++;
        }

        if (esq < dir){
            aux = vetor[esq];
            vetor[esq] = vetor[dir];
            vetor[dir] = aux;
        }
    }
    aux = vetor[dir];
    vetor[dir] = vetor[inicio];
    vetor[inicio] = aux;

    return dir;
}

void quickSort(int *vetor, int inicio, int fim, int *comparacoes){
    int pivo;
    if (inicio < fim){
    pivo = particiona(vetor, inicio, fim, comparacoes);
    quickSort(vetor, inicio, pivo-1, comparacoes);
    quickSort(vetor, pivo+1, fim, comparacoes);
    }
}

int main(int argc, char *argv[]){

    if (argc != 3) {
        printf("Erro: Faltam arquivos de entrada!\n");
        system("pause");
        exit(1);
    }

    FILE *arqEntrada, *arqSaida;
    arqEntrada = fopen(argv[1], "r");
    arqSaida = fopen(argv[2], "w+");

    int tamVetor;
    int comparacoes; // Contar número de comparações de cada algoritmo
    char metodo; // Médoto de geração do vetor: crescente, decrescente ou randômico
    int *vetorOriginal, *vetorCopia;
    int start, time_taken_millis; // Para calcular tempo de execução do algoritmo


    // Testando abertura do arquivo
    if (arqEntrada == NULL || arqSaida == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    // Verificando se o arquivo está vazio
    fseek(arqEntrada, 0, SEEK_END);
    if (ftell(arqEntrada)>0){
        rewind(arqEntrada);
    } else {
        fprintf(arqSaida, "Arquivo vazio!\n");
        printf("Erro: Arquivo de entrada vazio!\n");
        exit(1);
    }

    // Lendo o número da 1a linha do arquivo, para definir o tamanho do vetor
    fscanf(arqEntrada, "%d", &tamVetor);
    fgetc(arqEntrada);
    // Lendo o caractere da 2a linha do arquivo, para definir o método de geração do vetor
    metodo = fgetc(arqEntrada);

    // Criando o vetor
    vetorOriginal = criaVetor(vetorOriginal, tamVetor, metodo, arqSaida);

    vetorCopia = (int*) malloc(tamVetor * sizeof(int));
    // Criando um vetor cópia para o vetor original não ser alterado
    copiaVetor(vetorOriginal, vetorCopia, tamVetor);

    // Executando algoritmo Insertion Sort
    comparacoes = 0;
    start = clock();
    insertionSort(vetorCopia, tamVetor, &comparacoes);
    time_taken_millis = (int)((clock()-start)*1E3/CLOCKS_PER_SEC); // Calcular tempo de execução
    fprintf(arqSaida, "insertionSort:");
    imprimeVetor(vetorCopia, tamVetor, arqSaida);
    fprintf(arqSaida, "%d comp, %d ms\n", comparacoes, time_taken_millis);

    // Executando algoritmo Selection Sort
    copiaVetor(vetorOriginal, vetorCopia, tamVetor);
    comparacoes = 0;
    start = clock();
    selectionSort(vetorCopia, tamVetor, &comparacoes);
    time_taken_millis = (int)((clock()-start)*1E3/CLOCKS_PER_SEC);
    fprintf(arqSaida, "selectionSort:");
    imprimeVetor(vetorCopia, tamVetor, arqSaida);
    fprintf(arqSaida, "%d comp, %d ms\n", comparacoes, time_taken_millis);

    // Executando algoritmo Bubble Sort
    copiaVetor(vetorOriginal, vetorCopia, tamVetor);
    comparacoes = 0;
    start = clock();
    bubbleSort(vetorCopia, tamVetor, &comparacoes);
    time_taken_millis = (int)((clock()-start)*1E3/CLOCKS_PER_SEC);
    fprintf(arqSaida, "bubbleSort:");
    imprimeVetor(vetorCopia, tamVetor, arqSaida);
    fprintf(arqSaida, "%d comp, %d ms\n", comparacoes, time_taken_millis);

    // Executando algoritmo Merge Sort
    copiaVetor(vetorOriginal, vetorCopia, tamVetor);
    comparacoes = 0;
    start = clock();
    mergeSort(vetorCopia, 0, tamVetor-1, &comparacoes);
    time_taken_millis = (int)((clock()-start)*1E3/CLOCKS_PER_SEC);
    fprintf(arqSaida, "mergeSort:");
    imprimeVetor(vetorCopia, tamVetor, arqSaida);
    fprintf(arqSaida, "%d comp, %d ms\n", comparacoes, time_taken_millis);

    // Executando algoritmo Quick Sort
    copiaVetor(vetorOriginal, vetorCopia, tamVetor);
    comparacoes = 0;
    start = clock();
    quickSort(vetorCopia, 0, tamVetor-1, &comparacoes);
    time_taken_millis = (int)((clock()-start)*1E3/CLOCKS_PER_SEC);
    fprintf(arqSaida, "quickSort:");
    imprimeVetor(vetorCopia, tamVetor, arqSaida);
    fprintf(arqSaida, "%d comp, %d ms\n", comparacoes, time_taken_millis);

    free(vetorCopia);
    free(vetorOriginal);
    fclose(arqEntrada);
    fclose(arqSaida);

    printf("Arquivo criado com sucesso!\n");

    return 0;
}
