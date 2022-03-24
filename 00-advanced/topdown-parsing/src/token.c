#include <stdlib.h>
#include <string.h>

#include "token.h"
#include <stdio.h>

#include "error.h"

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
