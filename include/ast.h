#ifndef AST_H
#define AST_H

typedef enum {
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_BINARY,
    NODE_UNARY,
    NODE_ASSIGNMENT,
    NODE_CONDITIONAL,
    NODE_LOOP,
} node_type;

typedef struct node {
    node_type type;
    union {
        double value;
        char *name;
    };
    struct node *left;
    struct node *right;
    struct node *condition;
    struct node *body;
    struct node *else_body;
    int line;
} node;

node *make_node(node_type type, node *left, node *right, int line);

void free_node(node *n);

#endif /* AST_H */
