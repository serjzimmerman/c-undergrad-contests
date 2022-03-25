#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "token.h"

#include "error.h"

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
  token_t  token;
  char    *s;
  lexer_t *lex;

  s   = getueof();
  lex = lexer_init(s);

  ast_node_t *node = parse_expr(lex);

  if (!node || lexer_get_current_token(lex).type != TOKEN_EOF) {
    printf("ERROR\n");
    goto main_exit;
  }

  int res = ast_evaluate(node);
  printf("%d\n", res);

main_exit:
  lexer_free(lex);
  ast_free(node);

  free(s);
  return 0;
}