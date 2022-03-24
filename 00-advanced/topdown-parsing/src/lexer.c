#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "lexer.h"

#include <ctype.h>

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
