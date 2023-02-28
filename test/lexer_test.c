#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

void print_token(token tok) {
    switch (tok.type) {
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", tok.value);
            break;
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", tok.value);
            break;
        case TOKEN_STRING:
            printf("STRING: %s\n", tok.value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", tok.value);
            break;
        case TOKEN_DELIMITER:
            printf("DELIMITER: %s\n", tok.value);
            break;
        default:
            printf("UNKNOWN TOKEN TYPE\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Error: could not open file '%s'\n", argv[1]);
        return 1;
    }

    lexer_state state;
    init_lexer(fp, &state);

    token tok = get_next_token(&state);
    while (tok.type != TOKEN_EOF) {
        print_token(tok);
        tok = get_next_token(&state);
    }

    return 0;
}
