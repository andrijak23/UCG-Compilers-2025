// Ovaj fajl kombinuje funkcionalnosti iz tvog i prijateljevog `ast.c`, tako da odgovara parseru koji smo nadogradili.
// Imaće sve nove funkcije potrebne za punu funkcionalnost

#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static ASTNode* new_node(ASTNodeType type, const char* value, ASTNode* left, ASTNode* right) {
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

ASTNode* create_query_list_declaration_node(char* name, ASTNode* list) {
    return new_node(AST_QUERY_LIST_DECLARATION, name, list, NULL);
}

ASTNode* create_result_of_query_declaration_node(char* name) {
    return new_node(AST_RESULT_OF_QUERY_DECLARATION, name, NULL, NULL);
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

ASTNode* create_assign_exec_command_node(char* target, char* query) {
    ASTNode* node = new_node(AST_ASSIGN_EXEC_COMMAND, target, NULL, NULL);
    node->left = new_node(AST_EXEC_COMMAND, query, NULL, NULL);
    return node;
}

ASTNode* create_set_operation_node(char* target, char* left, char* op, char* right) {
    ASTNode* node = new_node(AST_SET_OPERATION, target, NULL, NULL);
    node->left = new_node(AST_TERM, left, NULL, NULL);
    node->right = new_node(AST_TERM, right, NULL, NULL);
    node->next = new_node(AST_TERM, op, NULL, NULL); // operator ide u next
    return node;
}

ASTNode* create_if_command_node(ASTNode* condition, ASTNode* body) {
    return new_node(AST_IF_COMMAND, NULL, condition, body);
}

ASTNode* create_for_command_node(char* iterator, ASTNode* query_list, ASTNode* body) {
    return new_node(AST_FOR_COMMAND, iterator, query_list, body);
}

ASTNode* create_condition_node(char* type, char* arg1, char* arg2) {
    ASTNode* cond = new_node(AST_CONDITION, type, NULL, NULL);
    cond->left = new_node(AST_TERM, arg1, NULL, NULL);
    if (arg2)
        cond->right = new_node(AST_TERM, arg2, NULL, NULL);
    return cond;
}

ASTNode* create_query_node(ASTNode* terms) {
    return new_node(AST_QUERY, NULL, terms, NULL);
}

ASTNode* create_query_reference_node(char* name) {
    return new_node(AST_QUERY_REFERENCE, name, NULL, NULL);
}

ASTNode* create_term_node(char* term) {
    return new_node(AST_TERM, term, NULL, NULL);
}

ASTNode* create_juxtaposition_node(ASTNode* left, ASTNode* right) {
    return new_node(AST_JUXTAPOSITION, NULL, left, right);
}

ASTNode* create_query_list_node(ASTNode* query, ASTNode* next) {
    ASTNode* node = new_node(AST_QUERY_LIST, NULL, query, NULL);
    node->next = next;
    return node;
}

ASTNode* add_query_to_list(ASTNode* list, ASTNode* query) {
    ASTNode* curr = list;
    while (curr->next) curr = curr->next;
    curr->next = create_query_list_node(query, NULL);
    return list;
}

ASTNode* create_binary_term_node(char* op, ASTNode* left, ASTNode* right) {
    return new_node(AST_BINARY_TERM, op, left, right);
}

ASTNode* create_terms_node(ASTNode* first, ASTNode* next) {
    ASTNode* node = new_node(AST_TERMS, NULL, first, NULL);
    node->next = next;
    return node;
}

ASTNode* add_term_to_concat(ASTNode* list, ASTNode* term) {
    ASTNode* curr = list;
    while (curr->next) curr = curr->next;
    curr->next = term;
    return list;
}

ASTNode* create_directive_node(char* key, char* value) {
    ASTNode* node = new_node(AST_DIRECTIVE, key, NULL, NULL);
    node->left = new_node(AST_TERM, value, NULL, NULL);
    return node;
}

ASTNode* create_term_directive_node(ASTNode* directive) {
    return new_node(AST_TERM, NULL, directive, NULL);
}

ASTNode* create_unary_term_node(char* op, ASTNode* term) {
    return new_node(AST_UNARY_TERM, op, term, NULL);
}

ASTNode* create_list_of_queries_node(ASTNode* list) {
    return new_node(AST_TERMS, NULL, list, NULL); // ili AST_LIST_OF_QUERIES ako definišeš novi tip
}

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void print_ast(struct ASTNode* node, int indent) {
    if (!node) return;

    print_indent(indent);
    switch (node->type) {
        case AST_PROGRAM:
            printf("Program\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case AST_DECLARATIONS:
            printf("Declarations\n");
            print_ast(node->left, indent + 1);
            print_ast(node->next, indent);
            break;
        case AST_QUERY_DECLARATION:
            printf("QueryDeclaration: %s\n", node->value);
            print_ast(node->left, indent + 1);
            break;
        case AST_QUERY_LIST_DECLARATION:
            printf("QueryListDeclaration: %s\n", node->value);
            print_ast(node->left, indent + 1);
            break;
        case AST_RESULT_OF_QUERY_DECLARATION:
            printf("ResultOfQuery: %s\n", node->value);
            break;
        case AST_COMMANDS:
            printf("Commands\n");
            print_ast(node->left, indent + 1);
            print_ast(node->next, indent);
            break;
        case AST_EXEC_COMMAND:
            printf("ExecCommand: %s\n", node->value);
            break;
        case AST_ASSIGN_EXEC_COMMAND:
            printf("AssignExecCommand: %s = EXEC ...\n", node->value);
            print_ast(node->left, indent + 1);
            break;
        case AST_SET_OPERATION:
            printf("SetOperation: %s = ... (%s)\n", node->value, node->next ? node->next->value : "?");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case AST_IF_COMMAND:
            printf("IfCommand\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case AST_FOR_COMMAND:
            printf("ForCommand: %s\n", node->value);
            printf("  Iterable:\n");
            print_ast(node->left, indent + 2);
            printf("  Body:\n");
            print_ast(node->right, indent + 2);
            break;
        case AST_CONDITION:
            printf("Condition: %s\n", node->value);
            if (node->left) print_ast(node->left, indent + 1);
            if (node->right) print_ast(node->right, indent + 1);
            break;
        case AST_QUERY:
            printf("Query\n");
            print_ast(node->left, indent + 1);
            break;
        case AST_QUERY_REFERENCE:
            printf("QueryReference: %s\n", node->value);
            break;
        case AST_JUXTAPOSITION:
            printf("Juxtaposition\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case AST_BINARY_TERM:
            printf("BinaryTerm: %s\n", node->value);
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case AST_UNARY_TERM:
            printf("UnaryTerm: %s\n", node->value);
            print_ast(node->left, indent + 1);
            break;
        case AST_TERMS:
            printf("Terms\n");
            print_ast(node->left, indent + 1);
            if (node->next) print_ast(node->next, indent);
            break;
        case AST_DIRECTIVE:
            printf("Directive: %s\n", node->value);
            print_ast(node->left, indent + 1);
            break;
        case AST_TERM:
            printf("Term: %s\n", node->value ? node->value : "(nested term)");
            if (node->left) print_ast(node->left, indent + 1);
            break;
        case AST_QUERY_LIST:
            printf("QueryList\n");
            print_ast(node->left, indent + 1);
            if (node->next) print_ast(node->next, indent);
            break;
        default:
            printf("Nepoznat AST tip: %d\n", node->type);
    }
}
