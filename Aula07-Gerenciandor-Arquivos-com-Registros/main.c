#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implemente uma função que simule o comando grep do Unix. A função
// receberá dois parâmetros:
// 1. uma string de consulta que deseja-se verificar sua existência e
// ocorrências no arquivo;
// 2. um arquivo texto com registros codificados usando \n como
// delimitador de de registros, e | como delimitador de campos;
//
// A saída da função são todos os registros onde há a ocorrência
// da string consultada.

#define TAMANHO 200

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Erro: Necessario informar uma string e um arquivo"
               "de entrada!\n");
        system("pause");
        exit(1);
    }

    FILE *arqEntrada;
    char linha[TAMANHO+1];
    int cont = 0;

    arqEntrada = fopen(argv[2], "r");

    if (arqEntrada == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    while(fgets(linha, TAMANHO, arqEntrada) != NULL ) {
        // strstr - Returns a pointer to the first occurrence of str2 in str1,
        // or a null pointer if str2 is not part of str1.
        // str1: C string containing the sequence of characters to match.
        // str2: C string to be scanned.
        if (strstr(linha, argv[1])) {
            printf("%s", linha);
            cont++;
        }
    }
    printf("Numero de ocorrencias: %d", cont);

    fclose(arqEntrada);
    return 0;
}
