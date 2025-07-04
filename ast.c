#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

ASTNode* create_assign_exec_command_node(char* target, char* query) {
    ASTNode* node = new_node(AST_ASSIGN_EXEC_COMMAND, target, NULL, NULL);
    node->left = new_node(AST_EXEC_COMMAND, query, NULL, NULL);
    return node;
}

ASTNode* create_set_operation_node(char* target, char* left, char* op, char* right) {
    ASTNode* node = new_node(AST_SET_OPERATION, target, NULL, NULL);
    node->left = new_node(AST_TERM, left, NULL, NULL);
    node->right = new_node(AST_TERM, right, NULL, NULL);
    node->next = new_node(AST_TERM, op, NULL, NULL); // smještamo operator u .next polje
    return node;
}

ASTNode* create_if_command_node(ASTNode* condition, ASTNode* body) {
    return new_node(AST_IF_COMMAND, NULL, condition, body);
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

ASTNode* create_term_node(char* term) {
    return new_node(AST_TERM, term, NULL, NULL);
}

ASTNode* create_juxtaposition_node(ASTNode* left, ASTNode* right) {
    return new_node(AST_JUXTAPOSITION, NULL, left, right);
}

ASTNode* create_for_command_node(char* iterator, ASTNode* query_list, ASTNode* body) {
    ASTNode* node = new_node(AST_FOR_COMMAND, iterator, query_list, body);
    return node;
}

ASTNode* create_query_list_node(char* name, ASTNode* next) {
    ASTNode* node = new_node(AST_QUERY_LIST, name, NULL, NULL);
    node->next = next;
    return node;
}

ASTNode* add_query_to_list(ASTNode* list, char* name) {
    ASTNode* curr = list;
    while (curr->next) curr = curr->next;
    curr->next = create_query_list_node(name, NULL);
    return list;
}


void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void print_ast(ASTNode* node, int indent) {
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
        case AST_QUERY:
            printf("Query\n");
            print_ast(node->left, indent + 1);
            break;
        case AST_TERM:
            printf("Term: %s\n", node->value);
            break;
        case AST_JUXTAPOSITION:
            printf("Juxtaposition\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
            case AST_FOR_COMMAND:
                printf("ForCommand (iterator: %s)\n", node->value);
                printf("  Iterable:\n");
                print_ast(node->left, indent + 2);
                printf("  Body:\n");
                print_ast(node->right, indent + 2);
                break;
            case AST_QUERY_LIST:
                print_indent(indent);
                printf("QueryReference: %s\n", node->value);
                print_ast(node->next, indent);
                break;
            case AST_IF_COMMAND:
                printf("IfCommand\n");
                printf("  Condition:\n");
                print_ast(node->left, indent + 2);
                printf("  Body:\n");
                print_ast(node->right, indent + 2);
                break;
            case AST_CONDITION:
                printf("Condition: %s\n", node->value);
                if (node->left) print_ast(node->left, indent + 1);
                if (node->right) print_ast(node->right, indent + 1);
                break;


        default:
            printf("Nepoznat AST tip\n");
    }
}
