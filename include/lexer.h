#ifndef LEXER_H
#define LEXER_H

// Token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_ERROR,
    TOKEN_END
} token_type;

// Token structure
typedef struct {
    token_type type;
    char *value;
    int line;
} token;

// Lexer structure
typedef struct {
    char *input;
    int position;
    int line;
} lexer;

// Function prototypes
lexer *lexer_new(char *input);
void lexer_free(lexer *lxr);
token get_next_token(lexer *lxr);

#endif
