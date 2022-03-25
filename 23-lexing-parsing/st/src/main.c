#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(DEBUG)

#define warning(format, ...) message("warning", format, ##__VA_ARGS__);
#define error(format, ...)                                                                                             \
  do {                                                                                                                 \
    message("fatal error", format, ##__VA_ARGS__);                                                                     \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)

#else
#define warning(format, ...)                                                                                           \
  do {                                                                                                                 \
  } while (0)
#define error(format, ...)                                                                                             \
  do {                                                                                                                 \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)
#endif

#define message(errtype, format, ...)                                                                                  \
  do {                                                                                                                 \
    fprintf(stderr, "%s: at line %d of %s in function %s\n" format, errtype, __LINE__, __FILE__, __PRETTY_FUNCTION__,  \
            ##__VA_ARGS__);                                                                                            \
  } while (0)

#define assertion(condition)                                                                                           \
  if (!(condition)) {                                                                                                  \
    do {                                                                                                               \
      message("Assertion (" #condition ") failed", "");                                                                \
      exit(EXIT_FAILURE);                                                                                              \
    } while (0);                                                                                                       \
  }

enum token_type_e {
  TOKEN_NUMBER      = 0,
  TOKEN_PAREN_LEFT  = 1,
  TOKEN_PAREN_RIGHT = 2,
  TOKEN_OP_PLUS     = 3,
  TOKEN_OP_MINUS    = 4,
  TOKEN_OP_TIMES    = 5,
  TOKEN_OP_DIV      = 6,
  TOKEN_INVALID     = -1,
  TOKEN_EOF         = -2,
};

typedef struct {
  enum token_type_e type;
  int               value;
} token_t;

#define TOKN(value) token_inits(TOKEN_NUMBER, value)
#define TOKT(type)  token_inits(type, 0)

token_t token_inits(enum token_type_e type, int value) {
  token_t token = {0};
  token.type    = type;
  token.value   = value;
  return token;
}

const char *const token_name[] = {"TOKEN_NUMBER", "PAREN_LEFT", "PAREN_RIGHT",   "OP_PLUS",  "OP_MINUS",
                                  "OP_TIMES",     "OP_DIV",     "TOKEN_INVALID", "TOKEN_EOF"};

const char *const token_get_name(token_t token) {
  return token_name[token.type];
}

void token_print(FILE *fp, token_t token) {
  if (token.type == TOKEN_NUMBER) {
    fprintf(fp, "type: %s, value: %d\n", token_get_name(token), token.value);
  } else {
    fprintf(fp, "type: %s\n", token_get_name(token));
  }
}

typedef struct {
  const char *src;
  size_t      len;

  char   c;
  size_t i;

  token_t current;
} lexer_t;

token_t lexer_get_next_token(lexer_t *lex);

lexer_t *lexer_init(const char *src) {
  lexer_t *lex;

  assertion(src);

  lex = calloc(1, sizeof(lexer_t));

  if (!lex) {
    return NULL;
  }

  lex->src = calloc(1, sizeof(char) * ((lex->len = strlen(src)) + 1));

  if (!lex->src) {
    free(lex);
    return lex;
  }

  memcpy(lex->src, src, lex->len);
  lex->c = *src;
  lexer_get_next_token(lex);

  return lex;
}

void lexer_free(lexer_t *lex) {
  assertion(lex);
  assertion(lex->src);

  free(lex->src);
  free(lex);
}

void lexer_next(lexer_t *lex) {
  assertion(lex);

  if (lex->i < lex->len && lex->c != '\0') {
    lex->c = lex->src[++lex->i];
  }
}

token_t lexer_get_number(lexer_t *lex) {
  const char *end, *begin;
  long        num;

  assertion(lex);

  begin = &lex->src[lex->i];

  if (lex->c == '-') {
    lexer_next(lex);
  }

  while (lex->c != '\0' && isdigit(lex->c)) {
    lexer_next(lex);
  }

  end = &lex->src[lex->i];
  num = strtol(begin, &end, 10);

  return TOKN(num);
}

token_t lexer_get_current_token(lexer_t *lex) {
  return lex->current;
}

token_t lexer_get_next_token(lexer_t *lex) {
  token_t tok = TOKT(TOKEN_EOF);

  assertion(lex);

  while (lex->c != '\0') {
    if (isspace(lex->c)) {
      lexer_next(lex);
      continue;
    }

    if (isdigit(lex->c)) {
      tok = lexer_get_number(lex);
      goto lexer_get_next_token_exit;
    }

    switch (lex->c) {
    case '(':
      tok = TOKT(TOKEN_PAREN_LEFT);
      break;
    case ')':
      tok = TOKT(TOKEN_PAREN_RIGHT);
      break;
    case '*':
      tok = TOKT(TOKEN_OP_TIMES);
      break;
    case '/':
      tok = TOKT(TOKEN_OP_DIV);
      break;
    case '+':
      tok = TOKT(TOKEN_OP_PLUS);
      break;
    case '-':
      tok = TOKT(TOKEN_OP_MINUS);
      break;
    default:
      tok = TOKT(TOKEN_INVALID);
      break;
    }

    lexer_next(lex);
    goto lexer_get_next_token_exit;
  }

lexer_get_next_token_exit:
  lex->current = tok;
  return tok;
}

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

  return 0;
}

char *getueof() {
  char *buffer;
  int   size = 256, i, c;

  buffer = calloc(size, sizeof(char));

  for (i = 0; (c = getchar()) != EOF; i++) {
    if (i == size) {
      buffer = realloc(buffer, (size *= 2));
    }

    buffer[i] = c;
  }

  return buffer;
}

int main() {
  token_t     token;
  char       *s;
  lexer_t    *lex;
  int         res;
  ast_node_t *node;

  s   = getueof();
  lex = lexer_init(s);

  node = parse_expr(lex);

  if (!node) {
    printf("ERROR\n");
    goto main_exit;
  }

  res = ast_evaluate(node);
  printf("%d\n", res);

main_exit:
  lexer_free(lex);
  ast_free(node);

  free(s);
  return 0;
}