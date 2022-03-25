#include "parser.h"
#include "error.h"
#include "lexer.h"
#include <stdlib.h>

// expr ::= mult {+, -} expr | mult
// mult ::= term {*, /} mult | term
// term ::= ( expr ) | number

ast_node_t *parse_term(lexer_t *lex);
ast_node_t *parse_expr(lexer_t *lex);
ast_node_t *parse_mult(lexer_t *lex);

ast_node_t *node_init() {
  ast_node_t *node;
  node = calloc(1, sizeof(ast_node_t));
  return node;
}

void ast_free(ast_node_t *node) {
  if (!node) {
    return;
  }

  if (node->left) {
    ast_free(node->left);
  }
  if (node->right) {
    ast_free(node->right);
  }

  free(node);
}

#define ITOKEOF(tok) ((tok).type == TOKEN_EOF)

ast_node_t *parse_expr(lexer_t *lex) {
  ast_node_t      *node = NULL, *left = NULL, *right = NULL, *prev = NULL;
  token_t          tok;
  enum operation_e op;

  if (lexer_get_current_token(lex).type == TOKEN_EOF) {
    goto parse_expr_error;
  }

  left = parse_mult(lex);
  if (!left) {
    free(left);
    goto parse_expr_error;
  }

  tok = lexer_get_current_token(lex);

  if (tok.type == TOKEN_OP_PLUS || tok.type == TOKEN_OP_MINUS) {
    while (tok.type == TOKEN_OP_PLUS || tok.type == TOKEN_OP_MINUS) {
      op = tok.type;
      if (lexer_get_next_token(lex).type == TOKEN_EOF) {
        goto parse_expr_error;
      }
      right = parse_mult(lex);
      if (!right) {
        goto parse_expr_error;
      }
      node = node_init();
      if (!node) {
        goto parse_expr_error;
      }

      node->op    = op;
      node->left  = left;
      node->right = right;

      left = node;
      tok  = lexer_get_current_token(lex);
    }

    return node;
  } else {
    return left;
  }

parse_expr_error:
  ast_free(node);
  return NULL;
}

ast_node_t *parse_mult(lexer_t *lex) {
  ast_node_t      *node = NULL, *left = NULL, *right = NULL;
  token_t          tok;
  enum operation_e op;

  if (lexer_get_current_token(lex).type == TOKEN_EOF) {
    goto parse_mult_error;
  }

  left = parse_term(lex);
  if (!left) {
    free(left);
    goto parse_mult_error;
  }

  tok = lexer_get_current_token(lex);

  if (tok.type == TOKEN_OP_TIMES || tok.type == TOKEN_OP_DIV) {
    while (tok.type == TOKEN_OP_TIMES || tok.type == TOKEN_OP_DIV) {
      op = tok.type;
      if (lexer_get_next_token(lex).type == TOKEN_EOF) {
        goto parse_mult_error;
      }
      right = parse_term(lex);
      if (!right) {
        goto parse_mult_error;
      }
      node = node_init();
      if (!node) {
        goto parse_mult_error;
      }

      node->op    = op;
      node->left  = left;
      node->right = right;

      left = node;
      tok  = lexer_get_current_token(lex);
    }

    return node;
  } else {
    return left;
  }

  return node;

parse_mult_error:
  free(right);

  ast_free(node);
  return NULL;
}

ast_node_t *parse_term(lexer_t *lex) {
  ast_node_t *node = NULL;
  token_t     tok;

  if ((tok = lexer_get_current_token(lex)).type == TOKEN_EOF) {
    goto parse_term_error;
  }

  node = node_init();
  if (!node) {
    goto parse_term_error;
  }

  switch (tok.type) {
  case TOKEN_NUMBER:
    node->type  = NODE_NUMBER;
    node->value = tok.value;
    break;
  case TOKEN_PAREN_LEFT:
    node->type = NODE_OP;
    /* Expect "(" */
    if (lexer_get_next_token(lex).type == TOKEN_EOF) {
      goto parse_term_error;
    }
    /* Parse expression */
    node = parse_expr(lex);
    if (!node) {
      goto parse_term_error;
    }
    /* Expect ")" */
    if (lexer_get_current_token(lex).type != TOKEN_PAREN_RIGHT) {
      goto parse_term_error;
    }
    break;
  default:
    goto parse_term_error;
  }

  lexer_get_next_token(lex);

parse_term_exit:
  return node;

parse_term_error:
  free(node);
  return NULL;
}

int ast_evaluate(ast_node_t *root) {
  assertion(root);

  if (root->type == NODE_NUMBER) {
    return root->value;
  } else if (root->type == NODE_OP) {
    switch (root->op) {
    case OP_PLUS:
      return ast_evaluate(root->left) + ast_evaluate(root->right);
    case OP_MINUS:
      return ast_evaluate(root->left) - ast_evaluate(root->right);
    case OP_TIMES:
      return ast_evaluate(root->left) * ast_evaluate(root->right);
    case OP_DIV:
      return ast_evaluate(root->left) / ast_evaluate(root->right);
    }
  }
}