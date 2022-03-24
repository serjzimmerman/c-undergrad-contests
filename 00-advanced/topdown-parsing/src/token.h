#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

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

token_t token_inits(enum token_type_e type, int value);

#define TOKN(value) token_inits(TOKEN_NUMBER, value)
#define TOKT(type)  token_inits(type, 0)

void token_print(FILE *fp, token_t token);

#endif