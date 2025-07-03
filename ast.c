#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* new_node(ASTNodeType type, const char* value, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = left;
    node->right = right;
    node->next = NULL;
    return node;
}

ASTNode* create_program_node(ASTNode* declarations, ASTNode* commands) {
    return new_node(AST_PROGRAM, NULL, declarations, commands);
}

ASTNode* create_declarations_node(ASTNode* declaration, ASTNode* next) {
    ASTNode* node = new_node(AST_DECLARATIONS, NULL, declaration, NULL);
    node->next = next;
    return node;
}

ASTNode* add_declaration(ASTNode* list, ASTNode* decl) {
    ASTNode* curr = list;
    while (curr->next) curr = curr->next;
    curr->next = decl;
    return list;
}

ASTNode* create_query_declaration_node(char* name, ASTNode* query) {
    return new_node(AST_QUERY_DECLARATION, name, query, NULL);
}

ASTNode* create_commands_node(ASTNode* command, ASTNode* next) {
    ASTNode* node = new_node(AST_COMMANDS, NULL, command, NULL);
    node->next = next;
    return node;
}

ASTNode* add_command(ASTNode* list, ASTNode* command) {
    ASTNode* curr = list;
    while (curr->next) curr = curr->next;
    curr->next = command;
    return list;
}

ASTNode* create_exec_command_node(char* query_name) {
    return new_node(AST_EXEC_COMMAND, query_name, NULL, NULL);
}

ASTNode* create_query_node(ASTNode* terms) {
    return new_node(AST_QUERY, NULL, terms, NULL);
}

ASTNode* create_term_node(char* term) {
    return new_node(AST_TERM, term, NULL, NULL);
}

ASTNode* create_juxtaposition_node(ASTNode* left, ASTNode* right) {
    return new_node(AST_JUXTAPOSITION, NULL, left, right);
}

#include <stdio.h>

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;

    print_indent(indent);
    switch (node->type) {
        case AST_PROGRAM:
            printf("Program\n");
            print_ast(node->left, indent + 1);  // declarations
            print_ast(node->right, indent + 1); // commands
            break;
        case AST_DECLARATIONS:
            printf("Declarations\n");
            print_ast(node->left, indent + 1);  // declaration
            print_ast(node->next, indent);      // next declaration
            break;
        case AST_QUERY_DECLARATION:
            printf("QueryDeclaration: %s\n", node->value);
            print_ast(node->left, indent + 1);  // query
            break;
        case AST_COMMANDS:
            printf("Commands\n");
            print_ast(node->left, indent + 1);  // command
            print_ast(node->next, indent);      // next command
            break;
        case AST_EXEC_COMMAND:
            printf("ExecCommand: %s\n", node->value);
            break;
        case AST_QUERY:
            printf("Query\n");
            print_ast(node->left, indent + 1);  // terms
            break;
        case AST_TERM:
            printf("Term: %s\n", node->value);
            break;
        case AST_JUXTAPOSITION:
            printf("Juxtaposition\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        default:
            printf("Nepoznat AST tip\n");
    }
}
