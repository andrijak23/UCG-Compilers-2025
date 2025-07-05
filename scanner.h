#ifndef SCANNER_H
#define SCANNER_H

#include "parser.tab.h"      /* za def tokena i YYLTYPE */

int yylex(void);
extern char *yytext;
extern int yylineno;
extern YYLTYPE yylloc;

#endif
