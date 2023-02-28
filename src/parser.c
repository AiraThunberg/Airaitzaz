#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdbool.h>

#include "lexer.h"

#include "parser.h"

// Define the list of operator precedence levels
typedef enum {
  PRECEDENCE_NONE,
  PRECEDENCE_ASSIGNMENT, // =
  PRECEDENCE_CONDITIONAL, // ?:
  PRECEDENCE_OR, // ||
  PRECEDENCE_AND, // &&
  PRECEDENCE_BIT_OR, // |
  PRECEDENCE_BIT_XOR, // ^
  PRECEDENCE_BIT_AND, // &
  PRECEDENCE_EQUALITY, // == !=
  PRECEDENCE_RELATIONAL, // < > <= >=
  PRECEDENCE_SHIFT, // << >>
  PRECEDENCE_ADDITIVE, // + -
  PRECEDENCE_MULTIPLICATIVE, // * / %
  PRECEDENCE_UNARY, // ! ~ ++ -- + -
  PRECEDENCE_POSTFIX // . [] ()
}
precedence;

// Define the list of AST node types
typedef enum {
  NODE_NONE,
  NODE_BLOCK,
  NODE_EXPRESSION,
  NODE_VARIABLE_DECLARATION,
  NODE_ASSIGNMENT,
  NODE_CONDITIONAL,
  NODE_LOGICAL_OR,
  NODE_LOGICAL_AND,
  NODE_BIT_OR,
  NODE_BIT_XOR,
  NODE_BIT_AND,
  NODE_EQUALITY,
  NODE_RELATIONAL,
  NODE_SHIFT,
  NODE_ADDITIVE,
  NODE_MULTIPLICATIVE,
  NODE_UNARY,
  NODE_POSTFIX,
  NODE_VARIABLE,
  NODE_NUMBER,
  NODE_STRING,
  NODE_CALL
}
node_type;

// Define the AST node structure
typedef struct node_t {
  node_type type;
  struct node_t * left;
  struct node_t * right;
  struct node_t * condition;
  struct node_t * then_branch;
  struct node_t * else_branch;
  char * name;
  char * value;
  int line;
}
node;

// Define the parser state
typedef struct {
  lexer_state lexer;
  token current_token;
}
parser_state;

// Initialize the parser state
void init_parser(parser_state * state, char * input) {
  init_lexer( & state -> lexer, input);
  state -> current_token = get_next_token( & state -> lexer);
}

// Parse an expression with the given precedence level or higher
node * parse_expression(parser_state * state, precedence prec) {
  node * left = NULL;
  node * right = NULL;

  // Parse the left-hand side of the expression
  switch (state -> current_token.type) {
  case TOKEN_IDENTIFIER:
    left = (node * ) malloc(sizeof(node));
    left -> type = NODE_VARIABLE;
    left -> name = strdup(state -> current_token.value);
    left -> line = state -> lexer.line;
    state -> current_token = get_next_token( & state -> lexer);
    break;
  case TOKEN_NUMBER:
    left = (node * ) malloc(sizeof(node));
    left -> type = NODE_NUMBER;
    left -> value = strdup(state -> current_token.value);
    left -> line = state -> lexer.line;
    state -> current_token = get_next_token( & state -> lexer);
    break;
  case TOKEN_STRING:
    left = (node * ) malloc(sizeof(node));
    left -> type = NODE_STRING;
    left -> value = strdup(state -> current_token.value);
    left -> line = state -> lexer.line;
    state -> current_token = get_next_token( & state -> lexer);
    break;
  case TOKEN_OPERATOR:
    if ( * state -> current_token.value == '(') {
      state -> current_token = get_next_token( & state -> lexer);
      left = parse_expression(state, PRECEDENCE_NONE);
      if (state -> current_token.type != TOKEN_DELIMITER || * state -> current_token.value != ')') {
        fprintf(stderr, "Syntax error: expected ')' on line %d\n", state -> lexer.line);
        exit(1);
      }
      state -> current_token = get_next_token( & state -> lexer);
    } else if ( * state -> current_token.value == '+' || * state -> current_token.value == '-') {
      left = (node * ) malloc(sizeof(node));
      left -> type = NODE_UNARY;
      left -> name = state -> current_token.value;
      left -> line = state -> lexer.line;
      state -> current_token = get_next_token( & state -> lexer);
      left -> right = parse_expression(state, PRECEDENCE_UNARY);
    } else if ( * state -> current_token.value == '!') {
      left = (node * ) malloc(sizeof(node));
      left -> type = NODE_UNARY;
      left -> name = state -> current_token.value;
      left -> line = state -> lexer.line;
      state -> current_token = get_next_token( & state -> lexer);
      left -> right = parse_expression(state, PRECEDENCE_NONE);
    } else {
      fprintf(stderr, "Syntax error: unexpected operator '%c' on line %d\n", * state -> current_token.value, state -> lexer.line);
      exit(1);
    }
    break;
  default:
    fprintf(stderr, "Syntax error: unexpected token '%s' on line %d\n", state -> current_token.value, state -> lexer.line);
    exit(1);
  } // Parse the right-hand side of the expression if there is one
  while (state -> current_token.type == TOKEN_OPERATOR && get_precedence(state -> current_token.value) >= prec) {
    token op = state -> current_token;
    state -> current_token = get_next_token( & state -> lexer);
    right = parse_expression(state, get_precedence(op.value) + 1);

    node * new_node = (node * ) malloc(sizeof(node));
    new_node -> type = get_node_type(op.value);
    new_node -> left = left;
    new_node -> right = right;
    new_node -> line = state -> lexer.line;
    left = new_node;
  }

  return left;
}

// Parse a statement
node * parse_statement(parser_state * state) {
  node * stmt = NULL;
  switch (state -> current_token.type) {
  case TOKEN_IDENTIFIER:
    stmt = (node * ) malloc(sizeof(node));
    stmt -> type = NODE_ASSIGNMENT;
    stmt -> name = strdup(state -> current_token.value);
    stmt -> line = state -> lexer.line;
    state -> current_token = get_next_token( & state -> lexer);
    if (state -> current_token.type != TOKEN_OPERATOR || * state -> current_token.value != '=') {
      fprintf(stderr, "Syntax error: expected '=' on line %d\n", state -> lexer.line);
      exit(1);
    }
    state -> current_token = get_next_token( & state -> lexer);
    stmt -> right = parse_expression(state, PRECEDENCE_NONE);
    break;
  default:
    fprintf(stderr, "Syntax error: unexpected token '%s' on line %d\n", state -> current_token.value, state -> lexer.line);
    exit(1);
  }

  if (state -> current_token.type != TOKEN_DELIMITER || * state -> current_token.value != ';') {
    fprintf(stderr, "Syntax error: expected ';' on line %d\n", state -> lexer.line);
    exit(1);
  }
  state -> current_token = get_next_token( & state -> lexer);

  return stmt;
}

// Parse a block of statements
node * parse_block(parser_state * state) {
  node * block = (node * ) malloc(sizeof(node));
  block -> type = NODE_BLOCK;
  block -> line = state -> lexer.line;
  // Parse statements until the end of the block
  while (state -> current_token.type != TOKEN_KEYWORD || strcmp(state -> current_token.value, "end") != 0) {
    node * stmt = parse_statement(state);
    if (stmt != NULL) {
      add_child(block, stmt);
    }
  }

  // Consume the end keyword
  state -> current_token = get_next_token( & state -> lexer);

  return block;
}
