#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_DECLARATIONS,
    AST_QUERY_DECLARATION,
    AST_COMMANDS,
    AST_EXEC_COMMAND,
    AST_QUERY,
    AST_TERM,
    AST_JUXTAPOSITION
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
} ASTNode;

// Kreiranje čvorova
ASTNode* create_program_node(ASTNode* declarations, ASTNode* commands);
ASTNode* create_declarations_node(ASTNode* declaration, ASTNode* next);
ASTNode* add_declaration(ASTNode* list, ASTNode* decl);
ASTNode* create_query_declaration_node(char* name, ASTNode* query);
ASTNode* create_commands_node(ASTNode* command, ASTNode* next);
ASTNode* add_command(ASTNode* list, ASTNode* command);
ASTNode* create_exec_command_node(char* query_name);
ASTNode* create_query_node(ASTNode* terms);
ASTNode* create_term_node(char* term);
ASTNode* create_juxtaposition_node(ASTNode* left, ASTNode* right);

void print_ast(struct ASTNode* node, int indent);


#endif
