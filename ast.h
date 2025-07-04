#ifndef AST_H
#define AST_H

typedef enum {
    AST_IF_COMMAND,
    AST_CONDITION,
    AST_PROGRAM,
    AST_DECLARATIONS,
    AST_QUERY_DECLARATION,
    AST_COMMANDS,
    AST_EXEC_COMMAND,
    AST_ASSIGN_EXEC_COMMAND,
    AST_SET_OPERATION,
    AST_FOR_COMMAND,
    AST_QUERY,
    AST_TERM,
    AST_JUXTAPOSITION,
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
ASTNode* create_commands_node(ASTNode* command, ASTNode* next);
ASTNode* add_command(ASTNode* list, ASTNode* command);
ASTNode* create_exec_command_node(char* query_name);
ASTNode* create_assign_exec_command_node(char* target, char* query);
ASTNode* create_set_operation_node(char* target, char* left, char* op, char* right);
ASTNode* create_for_command_node(char* iterator, ASTNode* query_list, ASTNode* body);
ASTNode* create_query_list_node(char* name, ASTNode* next);
ASTNode* add_query_to_list(ASTNode* list, char* name);
ASTNode* create_query_node(ASTNode* terms);
ASTNode* create_term_node(char* term);
ASTNode* create_juxtaposition_node(ASTNode* left, ASTNode* right);
ASTNode* create_if_command_node(ASTNode* condition, ASTNode* body);
ASTNode* create_condition_node(char* type, char* arg1, char* arg2);

void print_ast(struct ASTNode* node, int indent);

#endif
