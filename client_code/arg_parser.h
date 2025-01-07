#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <stdint.h>


void args_add_key(const char *key);


void args_parse(int argc, char *argv[]);
void args_print();

const char *args_gets(const char *key);
int         args_geti(const char *key);


#endif // ARG_PARSER_H
