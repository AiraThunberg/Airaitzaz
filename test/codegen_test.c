#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include "parser.h"

#include "codegen.h"

// Helper function to check if two strings are equal
bool string_equal(const char * s1,
  const char * s2) {
  return strcmp(s1, s2) == 0;
}

int main() {
  const char * input = "x = 2 + 3 * 4; y = (2 + 3) * 4;";
  lexer_state lexer = init_lexer(input);
  parser_state parser = {
    lexer,
    get_next_token( & lexer)
  };
  node * ast = parse( & parser);
  // Expected output for the above input
  const char * expected_output = "mov eax, 2\n"
  "mov ebx, 3\n"
  "mov ecx, 4\n"
  "mul ebx, ecx\n"
  "add eax, ebx\n"
  "mov x, eax\n"
  "mov eax, 2\n"
  "mov ebx, 3\n"
  "add eax, ebx\n"
  "mov ebx, 4\n"
  "mul eax, ebx\n"
  "mov y, eax\n";

  // Generate assembly code from the AST
  char * output = generate_code(ast);

  // Check if the output matches the expected output
  if (string_equal(output, expected_output)) {
    printf("CodeGen Test Passed\n");
  } else {
    printf("CodeGen Test Failed\n");
    printf("Expected output:\n%s\n", expected_output);
    printf("Actual output:\n%s\n", output);
  }

  // Free the memory used by the AST and the generated code
  free_ast(ast);
  free(output);

  return 0;
}
