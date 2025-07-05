%{
#include "location.h"
#include "ast.h"
#include "parser.tab.h"
#include "errors.h"
#include "scanner.h"

int column = 1;
#define YY_USER_ACTION \
    yylloc.first_line   = yylloc.last_line   = yylineno; \
    yylloc.first_column = column;                        \
    yylloc.last_column  = column + yyleng - 1;           \
    column += yyleng;

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
YYLTYPE yylloc;
%}

%option noyywrap yylineno

%%

[ \t\r]+                ;                     // ignoriši whitespace
\n                      { column = 1; }

"QUERY"                 { YY_USER_ACTION; return QUERY; }
"RESULT_OF_QUERY"       { YY_USER_ACTION; return RESULT_OF_QUERY; }
"EXEC"                  { YY_USER_ACTION; return EXEC; }
"IF"                    { YY_USER_ACTION; return IF; }
"FOR"                   { YY_USER_ACTION; return FOR; }
"IN"                    { YY_USER_ACTION; return IN; }
"BEGIN"                 { YY_USER_ACTION; return BININGI; }
"END"                   { YY_USER_ACTION; return ENDERDIR; }

"EMPTY"                 { YY_USER_ACTION; return EMPTY; }
"NOT_EMPTY"             { YY_USER_ACTION; return NOT_EMPTY; }
"URL_EXISTS"            { YY_USER_ACTION; return URL_EXISTS; }

"++"                    { YY_USER_ACTION; return SET_UNION; }
"--"                    { YY_USER_ACTION; return SET_DIFFERENCE; }
"**"                    { YY_USER_ACTION; return SET_INTERSECTION; }

"+"                     { YY_USER_ACTION; return PLUS; }
"-"                     { YY_USER_ACTION; return MINUS; }
"*"                     { YY_USER_ACTION; return STAR; }
"|"                     { YY_USER_ACTION; return PIPE; }
"="                     { YY_USER_ACTION; return ASSIGN; }
":"                     { YY_USER_ACTION; return COLON; }
";"                     { YY_USER_ACTION; return SEMICOLON; }
","                     { YY_USER_ACTION; return COMMA; }
"<"                     { YY_USER_ACTION; return LANGLE; }
">"                     { YY_USER_ACTION; return RANGLE; }
"["                     { YY_USER_ACTION; return LBRACKET; }
"]"                     { YY_USER_ACTION; return RBRACKET; }
"("                     { YY_USER_ACTION; return LPAREN; }
")"                     { YY_USER_ACTION; return RPAREN; }

\"[^\"\n]*\"            {
                          YY_USER_ACTION;
                          yylval.string = strdup(yytext);
                          return STRING;
                        }

[a-zA-Z_][a-zA-Z0-9_]*   {
                          YY_USER_ACTION;
                          yylval.string = strdup(yytext);
                          return WORD;
                        }

.                       {
                          YY_USER_ACTION;
                          report_lexical_error("Nepoznat simbol", yylineno, column);
                        }

%%

