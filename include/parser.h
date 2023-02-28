#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

typedef struct parser_state {
    lexer_state lexer;
    token current_token;
} parser_state;

node *parse_program(parser_state *state);

#endif /* PARSER_H */
