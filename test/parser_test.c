#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdbool.h>

#include "parser.h"

// Helper function to check if two nodes are equal
bool node_equal(node * n1, node * n2) {
  if (n1 == NULL || n2 == NULL) {
    return n1 == n2;
  }

  if (n1 -> type != n2 -> type) {
    return false;
  }

  if (strcmp(n1 -> name, n2 -> name) != 0) {
    return false;
  }

  if (n1 -> line != n2 -> line) {
    return false;
  }

  if (!node_equal(n1 -> left, n2 -> left)) {
    return false;
  }

  if (!node_equal(n1 -> right, n2 -> right)) {
    return false;
  }

  return true;
}

// Helper function to check if two arrays of nodes are equal
bool node_array_equal(node ** a1, node ** a2, int n) {
  for (int i = 0; i < n; i++) {
    if (!node_equal(a1[i], a2[i])) {
      return false;
    }
  }

  return true;
}

// Helper function to print a node
void print_node(node * n) {
  if (n == NULL) {
    printf("NULL");
    return;
  }

  printf("{ type: %d, name: %s, line: %d, left: ", n -> type, n -> name, n -> line);
  print_node(n -> left);
  printf(", right: ");
  print_node(n -> right);
  printf(" }");
}

int main() {
  const char * input = "x = 2 + 3 * 4; y = (2 + 3) * 4;";
  lexer_state lexer = init_lexer(input);
  parser_state parser = {
    lexer,
    get_next_token( & lexer)
  };

  // Expected output for the above input
  node * expected_output[] = {
    &
    (node) {
      .type = NODE_ASSIGNMENT, .name = "x", .line = 1,
        .right = & (node) {
          .type = NODE_BINARY, .name = "+", .line = 1,
            .left = & (node) {
              .type = NODE_NUMBER, .name = "2", .line = 1
            },
            .right = & (node) {
              .type = NODE_BINARY, .name = "*", .line = 1,
                .left = & (node) {
                  .type = NODE_NUMBER, .name = "3", .line = 1
                },
                .right = & (node) {
                  .type = NODE_NUMBER, .name = "4", .line = 1
                }
            }
        }
    },
    &
    (node) {
      .type = NODE_ASSIGNMENT, .name = "y", .line = 1,
        .right = & (node) {
          .type = NODE_BINARY, .name = "*", .line = 1,
            .left = & (node) {
              .type = NODE_BINARY, .name = "+", .line = 1,
                .left = & (node) {
                  .type = NODE_NUMBER, .name = "2", .line = 1
                },
                .right = & (node) {
                  .type = NODE_NUMBER, .name = "3", .line = 1
                }
            },
            .right = & (node) {
              .type = NODE_NUMBER, .name = "4", .line = 1
            }
        }
    }
  };

  // Parse the input and check if the output matches the expected output
  node ** output = parse( & parser);
  int num_nodes = 2;
  if (!node_array_equal(output, expected_output, num_nodes)) {
    printf("Test failed\nExpected: ");
    print_node(expected_output[0]);
    printf(", ");
    print_node(expected_output[1]);
    printf("\nActual:   ");
    print_node(output[0]);
    printf(", ");
    print_node(output[1]);
    printf("\n");
    return 1;
  }

  printf("Test passed\n");
  return 0;
}
