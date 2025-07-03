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
        perror("Ne mogu otvoriti fajl");
        return 1;
    }

    yyin = file;

    printf("Pokrecem parsiranje fajla: %s\n\n", argv[1]);

    if (yyparse() == 0) {
        printf("\nParsiranje uspjesno!\n");
    } else {
        printf("\nParsiranje nije uspjelo.\n");
    }

    fclose(file);
    return 0;
}
