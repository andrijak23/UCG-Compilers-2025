#ifndef AST_H
#define AST_H

typedef enum {
    AST_IF_COMMAND,
    AST_CONDITION,
    AST_PROGRAM,
    AST_DECLARATIONS,
    AST_QUERY_DECLARATION,
    AST_QUERY_LIST_DECLARATION,
    AST_RESULT_OF_QUERY_DECLARATION,
    AST_COMMANDS,
    AST_EXEC_COMMAND,
    AST_ASSIGN_EXEC_COMMAND,
    AST_SET_OPERATION,
    AST_FOR_COMMAND,
    AST_QUERY,
    AST_QUERY_REFERENCE,
    AST_TERM,
    AST_JUXTAPOSITION,
    AST_TERMS,
    AST_DIRECTIVE,
    AST_UNARY_TERM,
    AST_BINARY_TERM,
    AST_QUERY_LIST
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
} ASTNode;

ASTNode* create_program_node(ASTNode* declarations, ASTNode* commands);
ASTNode* create_declarations_node(ASTNode* declaration, ASTNode* next);
ASTNode* add_declaration(ASTNode* list, ASTNode* decl);
ASTNode* create_query_declaration_node(char* name, ASTNode* query);
ASTNode* create_query_list_declaration_node(char* name, ASTNode* list);
ASTNode* create_result_of_query_declaration_node(char* name);
ASTNode* create_commands_node(ASTNode* command, ASTNode* next);
ASTNode* add_command(ASTNode* list, ASTNode* command);
ASTNode* create_exec_command_node(char* query_name);
ASTNode* create_assign_exec_command_node(char* target, char* query);
ASTNode* create_set_operation_node(char* target, char* left, char* op, char* right);
ASTNode* create_for_command_node(char* iterator, ASTNode* query_list, ASTNode* body);
ASTNode* create_query_list_node(ASTNode* query, ASTNode* next);
ASTNode* add_query_to_list(ASTNode* list, ASTNode* query);
ASTNode* create_query_node(ASTNode* terms);
ASTNode* create_query_reference_node(char* name);
ASTNode* create_term_node(char* term);
ASTNode* create_juxtaposition_node(ASTNode* left, ASTNode* right);
ASTNode* create_if_command_node(ASTNode* condition, ASTNode* body);
ASTNode* create_condition_node(char* type, char* arg1, char* arg2);
ASTNode* create_binary_term_node(char* op, ASTNode* left, ASTNode* right);
ASTNode* create_terms_node(ASTNode* first, ASTNode* next);
ASTNode* add_term_to_concat(ASTNode* list, ASTNode* term);
ASTNode* create_directive_node(char* key, char* value);
ASTNode* create_term_directive_node(ASTNode* directive);
ASTNode* create_unary_term_node(char* op, ASTNode* term);
ASTNode* create_list_of_queries_node(ASTNode* list);

void print_ast(struct ASTNode* node, int indent);

#endif
