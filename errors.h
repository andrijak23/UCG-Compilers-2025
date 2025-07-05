#ifndef ERRORS_H
#define ERRORS_H

void report_syntax_error(const char *msg, int line, int col);
void report_lexical_error(const char *msg, int line, int col);
void report_lexical_error(const char* message, int line, int column);


#endif 