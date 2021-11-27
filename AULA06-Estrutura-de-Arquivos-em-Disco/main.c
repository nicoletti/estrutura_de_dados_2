#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        char last[11];
        char first[11];
        char address[16];
        char city[16];
        char state[3];
        char zip[10];
    } Person;

// método 1
void writeFixedLengths(FILE *file, Person obj){
    fprintf(file,"%-10.10s", obj.last);
    fprintf(file,"%-10.10s", obj.first);
    fprintf(file,"%-15.15s", obj.address);
    fprintf(file,"%-15.15s", obj.city);
    fprintf(file,"%-2.2s", obj.state);
    fprintf(file,"%-9.9s\n", obj.zip);
}

// método 2
void writeLengthIndicators(FILE *file, Person obj){
    fprintf(file,"%02d%s", strlen(obj.last), obj.last);
    fprintf(file,"%02d%s", strlen(obj.first), obj.first);
    fprintf(file,"%02d%s", strlen(obj.address), obj.address);
    fprintf(file,"%02d%s", strlen(obj.city), obj.city);
    fprintf(file,"%02d%s", strlen(obj.state), obj.state);
    fprintf(file,"%02d%s\n", strlen(obj.zip), obj.zip);
};

// método 4
void writeKeywordTags(FILE *file, Person obj){
    fprintf(file,"last=%s", obj.last);
    fprintf(file,"|first=%s", obj.first);
    fprintf(file,"|address=%s", obj.address);
    fprintf(file,"|city=%s", obj.city);
    fprintf(file,"|state=%s", obj.state);
    fprintf(file,"|zip=%s\n", obj.zip);
};


int main() {
    Person obj[2];
    FILE *file;

    file = fopen("file.txt", "w+");

    strcpy(obj[0].last, "Ames");
    strcpy(obj[0].first, "Mary");
    strcpy(obj[0].address, "123 Maple");
    strcpy(obj[0].city, "Stillwater");
    strcpy(obj[0].state, "OK");
    strcpy(obj[0].zip, "74075");

    strcpy(obj[1].last, "Mason");
    strcpy(obj[1].first, "Alan");
    strcpy(obj[1].address, "90 Eastate");
    strcpy(obj[1].city, "Ada");
    strcpy(obj[1].state, "OK");
    strcpy(obj[1].zip, "74820");

    fprintf(file, "Método 1:\n");
    writeFixedLengths(file, obj[0]);
    writeFixedLengths(file, obj[1]);
    fprintf(file, "\n");

    fprintf(file, "Método 2:\n");
    writeLengthIndicators(file, obj[0]);
    writeLengthIndicators(file, obj[1]);
    fprintf(file, "\n");

    fprintf(file, "Método 4:\n");
    writeKeywordTags(file, obj[0]);
    writeKeywordTags(file, obj[1]);

    fclose(file);
}
