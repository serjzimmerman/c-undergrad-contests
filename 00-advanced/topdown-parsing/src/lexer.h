#ifndef LEXER_H
#define LEXER_H

#include "stdlib.h"
#include "token.h"

typedef struct {
  const char *src;
  size_t      len;

  char   c;
  size_t i;

  token_t current;
} lexer_t;

lexer_t *lexer_init(const char *src);
void     lexer_free(lexer_t *lex);
token_t  lexer_get_next_token(lexer_t *lex);
token_t  lexer_get_current_token(lexer_t *lex);

#endif