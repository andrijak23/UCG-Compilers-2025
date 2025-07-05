%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "errors.h"

int yylex();
void yyerror(const char *s);
extern int yylineno;
int syntax_error = 0;
%}

%union {
    char* string;
    struct ASTNode* ast;
}

%token <string> WORD STRING
%token QUERY EXEC RESULT_OF_QUERY IF FOR IN BININGI ENDERDIR
%token EMPTY NOT_EMPTY URL_EXISTS
%token PLUS MINUS STAR PIPE ASSIGN COLON SEMICOLON COMMA
%token SET_UNION SET_DIFFERENCE SET_INTERSECTION
%token LANGLE RANGLE LBRACKET RBRACKET LPAREN RPAREN

%type <ast> program declarations declaration commands command assign_command query term query_list condition list_of_queries query_expr concat_expr directive

%start program

%%

program
    : declarations commands {
        if (!syntax_error) {
            $$ = create_program_node($1, $2);
            printf("Program validan!\n");
            print_ast($$, 0);
        }
    };

declarations
    : declaration                      { $$ = create_declarations_node($1, NULL); }
    | declarations declaration         { $$ = add_declaration($1, $2); }
    ;

declaration
    : QUERY WORD ASSIGN query SEMICOLON {
        $$ = create_query_declaration_node($2, $4);
    }
    | QUERY WORD ASSIGN list_of_queries SEMICOLON {
        $$ = create_query_list_declaration_node($2, $4);
    }
    | RESULT_OF_QUERY WORD SEMICOLON {
        $$ = create_result_of_query_declaration_node($2);
    }
    ;

commands
    : command                          { $$ = create_commands_node($1, NULL); }
    | commands command                 { $$ = add_command($1, $2); }
    ;

command
    : EXEC WORD SEMICOLON              { $$ = create_exec_command_node($2); }
    | assign_command SEMICOLON         { $$ = $1; }
    | FOR WORD IN list_of_queries BININGI commands ENDERDIR {
        $$ = create_for_command_node($2, $4, $6);
    }
    | IF condition BININGI commands ENDERDIR {
        $$ = create_if_command_node($2, $4);
    }
    ;

assign_command
    : WORD ASSIGN EXEC WORD {
        $$ = create_assign_exec_command_node($1, $4);
    }
    | WORD ASSIGN WORD SET_UNION WORD {
        $$ = create_set_operation_node($1, $3, "SET_UNION", $5);
    }
    | WORD ASSIGN WORD SET_DIFFERENCE WORD {
        $$ = create_set_operation_node($1, $3, "SET_DIFFERENCE", $5);
    }
    | WORD ASSIGN WORD SET_INTERSECTION WORD {
        $$ = create_set_operation_node($1, $3, "SET_INTERSECTION", $5);
    }
    ;

query
    : LANGLE query_expr RANGLE         { $$ = create_query_node($2); }
    | WORD                             { $$ = create_query_reference_node($1); }
    ;

query_expr
    : concat_expr                      { $$ = $1; }
    | query_expr PIPE concat_expr      { $$ = create_binary_term_node("PIPE", $1, $3); }
    ;

concat_expr
    : term                             { $$ = $1; }
    | concat_expr term                 { $$ = create_juxtaposition_node($1, $2); }
    ;

term
    : WORD                             { $$ = create_term_node($1); }
    | STRING                           { $$ = create_term_node($1); }
    | directive                        { $$ = create_term_directive_node($1); }
    | PLUS term                        { $$ = create_unary_term_node("PLUS", $2); }
    | MINUS term                       { $$ = create_unary_term_node("MINUS", $2); }
    | STAR term                        { $$ = create_unary_term_node("STAR", $2); }
    ;

directive
    : WORD COLON WORD                  { $$ = create_directive_node($1, $3); }
    | WORD COLON STRING                { $$ = create_directive_node($1, $3); }
    ;

query_list
    : query                            { $$ = create_query_list_node($1, NULL); }
    | query_list COMMA query           { $$ = add_query_to_list($1, $3); }
    ;

list_of_queries
    : LBRACKET query_list RBRACKET     { $$ = create_list_of_queries_node($2); }
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
    report_syntax_error(s, yylineno, 0);
    syntax_error = 1;
}
