#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"

extern int yyparse();
extern FILE* yyin;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Upotreba: %s <ulazni_fajl>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Greška prilikom otvaranja fajla");
        return 1;
    }

    yyin = file;

    printf("==> Parsiranje fajla: %s\n\n", argv[1]);

    int result = yyparse();

    if (result == 0) {
        printf("\n==> Parsiranje uspješno!\n");
    } else {
        printf("\n==> Došlo je do greške pri parsiranju.\n");
    }

    fclose(file);
    return result;
}
