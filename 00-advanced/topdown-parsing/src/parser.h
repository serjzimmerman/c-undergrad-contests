#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include <stdlib.h>

enum ast_node_type_e {
  NODE_OP     = 0,
  NODE_NUMBER = 1,
};

enum operation_e {
  OP_PLUS  = 3,
  OP_MINUS = 4,
  OP_TIMES = 5,
  OP_DIV   = 6,
};

typedef struct ast_node_s {
  struct ast_node_s   *left, *right;
  enum ast_node_type_e type;
  union {
    enum operation_e op;
    int              value;
  };
} ast_node_t;

ast_node_t *ast_tree_build(lexer_t *lex);
ast_node_t *parse_term(lexer_t *lex);
ast_node_t *parse_mult(lexer_t *lex);
ast_node_t *parse_expr(lexer_t *lex);

void ast_free(ast_node_t *node);
int  ast_evaluate(ast_node_t *root);

#endif