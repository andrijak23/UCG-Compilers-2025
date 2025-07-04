%{
#include "parser.tab.h"
#include <string.h>

int line_number = 1;
%}

%%

[ \t\r]+                ;
\n                      { line_number++; }

"QUERY"                 { return QUERY; }
"RESULT_OF_QUERY"       { return RESULT_OF_QUERY; }
"EXEC"                  { return EXEC; }
"IF"                    { return IF; }
"FOR"                   { return FOR; }
"IN"                    { return IN; }
"BEGIN"                 { return BEGIN_BLOCK; }
"END"                   { return END; }
"EMPTY"                 { return EMPTY; }
"NOT_EMPTY"             { return NOT_EMPTY; }
"URL_EXISTS"            { return URL_EXISTS; }

"++"                    { return PLUSPLUS; }
"--"                    { return MINUSMINUS; }
"**"                    { return STARSTAR; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return STAR; }
"|"                     { return PIPE; }
"="                     { return EQUAL; }
":"                     { return COLON; }
";"                     { return SEMICOLON; }
","                     { return COMMA; }

"<"                     { return LT; }
">"                     { return GT; }
"["                     { return LBRACKET; }
"]"                     { return RBRACKET; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }

\"([^\\\"]|\\.)*\"      {
                          yylval.string = strdup(yytext);
                          return STRING;
                        }

[a-zA-Z_][a-zA-Z0-9_]*   {
                          yylval.string = strdup(yytext);
                          return WORD;
                        }

.                       {
                          printf("Nepoznat znak: %s\n", yytext);
                        }

%%

int yywrap() {
    return 1;
}
