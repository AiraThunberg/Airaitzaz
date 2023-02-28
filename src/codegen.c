#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "codegen.h"

void codegen_expression(node *expr) {
    switch (expr->type) {
        case NODE_INTEGER:
            printf("push %d\n", expr->value);
            break;
        case NODE_IDENTIFIER:
            printf("push %s\n", expr->name);
            break;
        case NODE_BINARY:
            codegen_expression(expr->left);
            codegen_expression(expr->right);
            switch (*expr->name) {
                case '+':
                    printf("add\n");
                    break;
                case '-':
                    printf("sub\n");
                    break;
                case '*':
                    printf("mul\n");
                    break;
                case '/':
                    printf("div\n");
                    break;
                default:
                    fprintf(stderr, "Error: unsupported binary operator '%s'\n", expr->name);
                    exit(1);
            }
            break;
        case NODE_UNARY:
            codegen_expression(expr->right);
            switch (*expr->name) {
                case '+':
                    break;
                case '-':
                    printf("neg\n");
                    break;
                case '!':
                    printf("not\n");
                    break;
                default:
                    fprintf(stderr, "Error: unsupported unary operator '%s'\n", expr->name);
                    exit(1);
            }
            break;
        default:
            fprintf(stderr, "Error: unsupported expression type '%d'\n", expr->type);
            exit(1);
    }
}

void codegen_statement(node *stmt) {
    switch (stmt->type) {
        case NODE_ASSIGNMENT:
            codegen_expression(stmt->right);
            printf("pop %s\n", stmt->name);
            break;
        default:
            fprintf(stderr, "Error: unsupported statement type '%d'\n", stmt->type);
            exit(1);
    }
}

void codegen(node *root) {
    if (!root) {
        return;
    }

    switch (root->type) {
        case NODE_STATEMENT:
            codegen_statement(root);
            break;
        case NODE_EXPRESSION:
            codegen_expression(root);
            printf("pop\n");
            break;
        case NODE_BLOCK:
            for (int i = 0; i < root->n_children; i++) {
                codegen(root->children[i]);
            }
            break;
        default:
            fprintf(stderr, "Error: unsupported root node type '%d'\n", root->type);
            exit(1);
    }
}
