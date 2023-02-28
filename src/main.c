#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

int main() {
    // Create a new lexer and parser state
    lexer_state lexer = {0};
    parser_state parser = {0};

    // Set the input expression to be parsed
    const char *input = "3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3";
    init_lexer(&lexer, input);
    init_parser(&parser, &lexer);

    // Parse the input expression
    node *ast = parse_expression(&parser, PRECEDENCE_NONE);

    // Generate code for the expression and evaluate it
    double result = eval(ast);
    printf("%s = %g\n", input, result);

    // Free the AST
    free_ast(ast);

    return 0;
}
