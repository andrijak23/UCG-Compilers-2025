%{



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


int yylex();
void yyerror(const char *s);
extern int yylineno;
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

%type <ast> program declarations declaration commands command assign_command query terms term query_list condition
%type <string> set_operator

%%

program
    : declarations commands {
        $$ = create_program_node($1, $2);
        printf("Program validan!\n");
        print_ast($$, 0);
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
    | assign_command SEMICOLON         { $$ = $1; }
    | FOR WORD IN LBRACKET query_list RBRACKET BEGIN_BLOCK commands END {
        $$ = create_for_command_node($2, $5, $8);
    }
    | IF condition BEGIN_BLOCK commands END {
    $$ = create_if_command_node($2, $4);
}
    ;


assign_command
    : WORD EQUAL EXEC WORD             { $$ = create_assign_exec_command_node($1, $4); }
    | WORD EQUAL WORD set_operator WORD {
        $$ = create_set_operation_node($1, $3, $4, $5);
    }
    ;

set_operator
    : PLUSPLUS                         { $$ = strdup("++"); }
    | MINUSMINUS                       { $$ = strdup("--"); }
    | STARSTAR                         { $$ = strdup("**"); }
    ;

query
    : LT terms GT                      { $$ = create_query_node($2); }
    ;

query_list
    : WORD                             { $$ = create_query_list_node($1, NULL); }
    | query_list COMMA WORD            { $$ = add_query_to_list($1, $3); }
    ;

terms
    : term                             { $$ = $1; }
    | terms term                       { $$ = create_juxtaposition_node($1, $2); }
    ;

term
    : WORD                             { $$ = create_term_node($1); }
    | STRING                           { $$ = create_term_node($1); }
    ;

condition
    : EMPTY LPAREN WORD RPAREN         { $$ = create_condition_node("EMPTY", $3, NULL); }
    | NOT_EMPTY LPAREN WORD RPAREN     { $$ = create_condition_node("NOT_EMPTY", $3, NULL); }
    | URL_EXISTS LPAREN WORD COMMA STRING RPAREN {
        $$ = create_condition_node("URL_EXISTS", $3, $5);
    }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Greska: %s (na liniji %d)\n", s, yylineno);
}
