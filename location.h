#ifndef LOCATION_H
#define LOCATION_H

// Bison/Flex location 
typedef struct YYLTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} YYLTYPE;

#define YYLTYPE_IS_DECLARED 1

// AST nodes
typedef YYLTYPE Location;

// Extern deklaracija
extern YYLTYPE yylloc;

#endif // LOCATION_H
