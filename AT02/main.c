#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int cod;
    char nome[31];
    char sexo;
    int idade;
    char especialidade[31];
    char telefone[15];
} Professor;

//============================== FILA DINÂMICA ==============================//
// Funções e estruturas de fila dinâmica, para armazenar os dados antes de
// ler o método de leitura/escrita na última linha do arquivo de entrada
typedef struct NoFila *PtrNoFila;

typedef struct NoFila{
    Professor p;
    PtrNoFila proximo;
} NoFila;

typedef struct {
    PtrNoFila inicio;
    PtrNoFila fim;
    int tamanho;
} FilaDinamica;

void iniciaFila(FilaDinamica *fila){
    fila->inicio = fila->fim = NULL;
    fila->tamanho = 0;
}

void enfileira(Professor x, FilaDinamica *fila){
    PtrNoFila aux;
    aux = (PtrNoFila) malloc(sizeof(NoFila));
    aux->p = x;

    if (fila->tamanho == 0) {
        fila->inicio = fila->fim = aux;
        aux->proximo = NULL;
    } else {
        aux->proximo = fila->fim->proximo;
        fila->fim->proximo = aux;
        fila->fim = aux;
    }
    fila->tamanho++;
}

void destruirFila(FilaDinamica *fila){
    PtrNoFila apagar;
    PtrNoFila temp = fila->inicio;

    while(temp!=NULL){
        apagar = temp;
        temp = temp->proximo;
        free(apagar);
    }
    fila->inicio = fila->fim = NULL;
}
//===========================================================================//

// Função usada na o arquivo de persistência gerado a partir da escrita com
// registros de tamanho fixo
void removeEspacosString(char *string){
    int i = 0, j = 1;

    while (string[i]){
        if ((string[i] == ' ' && string[j] == ' ') ||
        (string[i] == ' ' && string[j] == '\0')){
            string[i] = '\0';
            break;
        }
        i++;
        j++;
    }
}

void escreverRegistrosTamanhoFixo(FILE* arq, FilaDinamica *sistema){
    PtrNoFila temp;

    for (temp = sistema->inicio; temp != NULL; temp = temp->proximo){
        fprintf(arq, "%03d|%-30.30s|%c|%d|%-30.30s|%-14.14s|\n",
               temp->p.cod,
               temp->p.nome,
               temp->p.sexo,
               temp->p.idade,
               temp->p.especialidade,
               temp->p.telefone);
    }
}

void escreverRegistrosIndicadoresTamanho(FILE* arq, FilaDinamica *sistema){
    PtrNoFila temp;
    int indicador = 0;

    for (temp = sistema->inicio; temp != NULL; temp = temp->proximo){
        // Indicador soma os bytes usados no registro
        // Descontando 1 byte do campo 'cod', pois será representado com 3 dígitos.
        // Descontando 2 bytes do campo 'idade', pois será representado com 2 dígitos.
        // Somando 6 bytes referentes aos pipes (|).
        indicador = (sizeof(temp->p.cod)-1) + strlen(temp->p.nome) +
                sizeof(temp->p.sexo) + (sizeof(temp->p.idade)-2) +
                strlen(temp->p.especialidade) + strlen(temp->p.telefone) + 6;

        fprintf(arq, "%d %03d|%s|%c|%d|%s|%s|",
                indicador,
                temp->p.cod,
                temp->p.nome,
                temp->p.sexo,
                temp->p.idade,
                temp->p.especialidade,
                temp->p.telefone);
    }
}

void escreverRegistrosDelimitadores(FILE* arq, FilaDinamica *sistema){
    PtrNoFila temp;

    for (temp = sistema->inicio; temp != NULL; temp = temp->proximo){
        fprintf(arq, "%03d|%s|%c|%d|%s|%s|",
                temp->p.cod,
                temp->p.nome,
                temp->p.sexo,
                temp->p.idade,
                temp->p.especialidade,
                temp->p.telefone);

        // Imprimindo o delimitador '#' ao final de cada registro,
        // com exceção do último registro.
        if (temp->proximo != NULL) fprintf (arq, "#");
    }
}

void lerRegistrosTamanhoFixo(FILE *arqSaida, FILE *arqPersist, Professor p){
    rewind(arqSaida);
    char c;

    c = fgetc(arqSaida);

    while (c != EOF) {
        fscanf(arqSaida, "%d|%[^|]|%c|%d|%[^|]|%[^|]|\n",
               &p.cod,
               p.nome,
               &p.sexo,
               &p.idade,
               p.especialidade,
               p.telefone);

        removeEspacosString(p.nome);
        removeEspacosString(p.especialidade);

        fprintf(arqPersist, "{%03d,%s,%c,%d,%s,%s}\n",
                p.cod,
                p.nome,
                p.sexo,
                p.idade,
                p.especialidade,
                p.telefone);

        c = fgetc(arqSaida);
    }
}
void lerRegistrosIndicadoresTamanho(FILE *arqSaida, FILE *arqPersist, Professor p){
    rewind(arqSaida);
    char string[200], *subString;
    int tamanho, i;

    while(1){
        fscanf(arqSaida, "%d ", &tamanho);
        if (feof(arqSaida))
            break;
        fgets(string, tamanho+1, arqSaida);
        subString = strtok(string, "|");

        while (subString != NULL){
            for (i = 1; i < 7; i++){
                if (i == 1) p.cod = atoi(subString);
                if (i == 2) strcpy(p.nome, subString);
                if (i == 3) p.sexo = subString[0];
                if (i == 4) p.idade = atoi(subString);
                if (i == 5) strcpy(p.especialidade, subString);
                if (i == 6) strcpy(p.telefone, subString);
                subString = strtok(NULL, "|");
            }
        }
        fprintf(arqPersist, "{%03d,%s,%c,%d,%s,%s}\n",
                p.cod,
                p.nome,
                p.sexo,
                p.idade,
                p.especialidade,
                p.telefone);
    }
}

void lerRegistrosDelimitadores(FILE *arqSaida, FILE *arqPersist, Professor p) {
    rewind(arqSaida);
    char c;

    c = fgetc(arqSaida);

    while (c != EOF) {
        fscanf(arqSaida, "%d|%[^|]|%c|%d|%[^|]|%[^|]|#",
               &p.cod,
               p.nome,
               &p.sexo,
               &p.idade,
               p.especialidade,
               p.telefone);

        fprintf(arqPersist, "{%03d,%s,%c,%d,%s,%s}\n",
                p.cod,
                p.nome,
                p.sexo,
                p.idade,
                p.especialidade,
                p.telefone);

        c = fgetc(arqSaida);
    }
}

int main(int argc, char *argv[]){

    if (argc != 4) {
        printf("Erro: Faltam argumentos!\n");
        system("pause");
        exit(1);
    }

    FILE *arqEntrada, *arqSaida, *arqPersist;
    FilaDinamica sistema;
    Professor p;
    char c;

    arqEntrada = fopen(argv[1], "r");
    arqSaida = fopen(argv[2], "w+");
    arqPersist = fopen(argv[3], "w+");

    // Testando abertura do arquivo
    if (arqEntrada == NULL || arqSaida == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    iniciaFila(&sistema);

    c = fgetc(arqEntrada);

    if (c == EOF) {
        printf("Aquivo de entrada vazio!\n");
        fprintf(arqSaida, "Aquivo de entrada vazio!\n");
        fprintf(arqPersist, "Aquivo de entrada vazio!\n");
        system("pause");
        exit(1);
    }

    while (c != EOF){
        switch(c){
            case '{':
                fscanf(arqEntrada, "%d,%30[^,],%c,%d,%30[^,],%14[^}]}\n",
                &p.cod,
                p.nome,
                &p.sexo,
                &p.idade,
                p.especialidade,
                p.telefone);

                // 'cod' com limite de 3 dígitos (Valor máx.: 999)
                if (p.cod > 999) p.cod = 999;
                // 'idade' com limite de 2 dígitos (Valor máx.: 99)
                if (p.idade > 99) p.idade = 99;

                enfileira(p, &sistema);
                break;
            case '1':
                escreverRegistrosTamanhoFixo(arqSaida, &sistema);
                lerRegistrosTamanhoFixo(arqSaida, arqPersist, p);
                c = fgetc(arqEntrada);
                break;
            case '2':
                escreverRegistrosIndicadoresTamanho(arqSaida, &sistema);
                lerRegistrosIndicadoresTamanho(arqSaida, arqPersist, p);
                c = fgetc(arqEntrada);
                break;
            case '3':
                escreverRegistrosDelimitadores(arqSaida, &sistema);
                lerRegistrosDelimitadores(arqSaida, arqPersist, p);
                c = fgetc(arqEntrada);
                break;
            default:
                fprintf(arqSaida, "Arquivo de entrada fora do padrao esperado!\n");
                fprintf(arqPersist, "Arquivo de entrada fora do padrao esperado!\n");
                exit(1);
        }
        c = fgetc(arqEntrada);
    }

    destruirFila(&sistema);

    fclose(arqEntrada);
    fclose(arqSaida);
    fclose(arqPersist);

    printf("Arquivos criados com sucesso!\n");

    return 0;
}
