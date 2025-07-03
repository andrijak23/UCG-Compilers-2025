%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int yylex();
void yyerror(const char *s);
%}

%union {
    char* string;
    struct ASTNode* ast;
}

%token <string> WORD STRING
%token QUERY EXEC RESULT_OF_QUERY IF FOR IN BEGIN_BLOCK END
%token EMPTY NOT_EMPTY URL_EXISTS
%token PLUS MINUS STAR PIPE EQUAL COLON SEMICOLON COMMA
%token PLUSPLUS MINUSMINUS STARSTAR
%token LT GT LBRACKET RBRACKET LPAREN RPAREN

%type <ast> program declarations declaration commands command query terms term

%%

program
    : declarations commands {
        $$ = create_program_node($1, $2);
        printf("Program validan!\n");
        print_ast($$, 0);  // Dodaj ispis AST-a
    };

declarations
    : declaration                      { $$ = create_declarations_node($1, NULL); }
    | declarations declaration         { $$ = add_declaration($1, $2); }
    ;

declaration
    : QUERY WORD EQUAL query SEMICOLON {
        $$ = create_query_declaration_node($2, $4);
    }
    ;

commands
    : command                          { $$ = create_commands_node($1, NULL); }
    | commands command                 { $$ = add_command($1, $2); }
    ;

command
    : EXEC WORD SEMICOLON              { $$ = create_exec_command_node($2); }
    ;

query
    : LT terms GT                      { $$ = create_query_node($2); }
    ;

terms
    : term                             { $$ = $1; }
    | terms term                       { $$ = create_juxtaposition_node($1, $2); }
    ;

term
    : WORD                             { $$ = create_term_node($1); }
    | STRING                           { $$ = create_term_node($1); }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Greška: %s\n", s);
}
