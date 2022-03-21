#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define min(a, b)                                                                                                      \
  ({                                                                                                                   \
    __typeof__(a) _a = (a);                                                                                            \
    __typeof__(b) _b = (b);                                                                                            \
    _a < _b ? _a : _b;                                                                                                 \
  })

#define max(a, b)                                                                                                      \
  ({                                                                                                                   \
    __typeof__(a) _a = (a);                                                                                            \
    __typeof__(b) _b = (b);                                                                                            \
    _a > _b ? _a : _b;                                                                                                 \
  })

#define DEBUG
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

#define token_stack_assert(stack)                                                                                      \
  do {                                                                                                                 \
    assertion(stack);                                                                                                  \
    assertion(stack->stack);                                                                                           \
    assertion(stack->i >= -1);                                                                                         \
  } while (0)

#define token_queue_assert(queue)                                                                                      \
  do {                                                                                                                 \
    assertion(queue);                                                                                                  \
    assertion(queue->array);                                                                                           \
  } while (0)

struct token_t {
  long value;

  enum {
    TOKEN_INTEGER = 0,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_TIMES,
    TOKEN_DIV,
    TOKEN_INVALID,
    TOKEN_EOF,
  } id;
};

struct token_queue_t {
  struct token_t **array;
  size_t capacity;
  ssize_t front, rear, size;
};

struct token_queue_t *token_queue_init();
void token_queue_enqueue(struct token_queue_t *queue, struct token_t *token);
struct token_t *token_queue_dequeue(struct token_queue_t *queue);
void token_queue_free(struct token_queue_t *queue);

const char *const token_name[] = {"NUMBER", "LBRAC", "RBRAC",         "MINUS",    "PLUS",
                                  "MUL",    "DIV",   "TOKEN_INVALID", "TOKEN_EOF"};

const char *const token_id_to_string(int id) {
  return token_name[id];
}

void token_print(struct token_t *token) {
  printf("type: %s, value: %ld\n", token_id_to_string(token->id), token->value);
}

/* Initialize token with value and id */
struct token_t *token_init(int value, int id) {
  struct token_t *token = calloc(1, sizeof(struct token_t));

  assertion(token);

  token->id = id;
  token->value = value;

  return token;
}

/* Queue is scuffed TODO[]: fix queue */
#define DEFAULT_QUEUE_SIZE 1024
struct token_queue_t *token_queue_init() {
  struct token_queue_t *queue = calloc(1, sizeof(struct token_queue_t));

  assertion(queue);

  queue->capacity = DEFAULT_QUEUE_SIZE;
  queue->front = queue->size = 0;
  queue->rear = queue->capacity - 1;

  queue->array = calloc(DEFAULT_QUEUE_SIZE, sizeof(struct token_t *));
  assertion(queue->array);

  return queue;
}

int token_queue_is_full(struct token_queue_t *queue) {
  token_queue_assert(queue);

  return ((size_t)queue->size == queue->capacity);
}

int token_queue_is_empty(struct token_queue_t *queue) {
  token_queue_assert(queue);

  return (queue->size == 0);
}

void token_queue_free(struct token_queue_t *queue) {
  token_queue_assert(queue);

  while (!token_queue_is_empty(queue)) {
    free(token_queue_dequeue(queue));
  }

  free(queue->array);
  free(queue);
}

void token_queue_enqueue(struct token_queue_t *queue, struct token_t *token) {
  token_queue_assert(queue);
  assertion(token);

  if (token_queue_is_full(queue)) {
    queue->capacity = queue->capacity * 2;
    queue->array = realloc(queue->array, queue->capacity * sizeof(struct token_t *));
  }

  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = token;

  queue->size = queue->size + 1;
}

struct token_t *token_queue_dequeue(struct token_queue_t *queue) {
  struct token_t *token;

  token_queue_assert(queue);

  if (token_queue_is_empty(queue)) {
    return NULL;
  }

  token = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;

  return token;
}

struct token_stack_t {
  struct token_t **stack;
  size_t stack_size;
  ssize_t i;
};

enum {
  NON_ASSOCIATIVE,
  LEFT_ASSOCIATIVE,
  RIGHT_ASSOCIATIVE,
};

const int token_precedence[] = {0, 0, 0, 1, 1, 2, 2, 3, 0};
const int token_associativity[] = {NON_ASSOCIATIVE,  NON_ASSOCIATIVE,   NON_ASSOCIATIVE,
                                   LEFT_ASSOCIATIVE, LEFT_ASSOCIATIVE,  LEFT_ASSOCIATIVE,
                                   LEFT_ASSOCIATIVE, RIGHT_ASSOCIATIVE, NON_ASSOCIATIVE};

int token_get_operator_precedence(struct token_t *token) {
  return token_precedence[token->id];
}

int token_get_operator_associativity(struct token_t *token) {
  return token_associativity[token->id];
}

/* Token stack that contains token pointers */
#define DEFAULT_STACK_SIZE 1024
struct token_stack_t *token_stack_init() {
  struct token_stack_t *stack = calloc(1, sizeof(struct token_stack_t));

  assertion(stack);

  stack->stack_size = DEFAULT_STACK_SIZE;
  stack->i = -1;

  stack->stack = calloc(DEFAULT_STACK_SIZE, sizeof(struct token_t *));
  assertion(stack->stack);

  return stack;
}

void token_stack_free(struct token_stack_t *stack) {
  token_stack_assert(stack);

  for (int i = 0; i <= stack->i; i++) {
    free(stack->stack[i]);
  }

  free(stack->stack);
  free(stack);
}

int token_stack_is_empty(struct token_stack_t *stack) {
  token_stack_assert(stack);

  return (stack->i == -1);
}

struct token_t *token_stack_pop(struct token_stack_t *stack) {
  token_stack_assert(stack);

  if (token_stack_is_empty(stack)) {
    return NULL;
  }

  return stack->stack[stack->i--];
}

struct token_t *token_stack_peek(struct token_stack_t *stack) {
  token_stack_assert(stack);

  if (token_stack_is_empty(stack)) {
    return NULL;
  }

  return stack->stack[stack->i];
}

void token_stack_push(struct token_stack_t *stack, struct token_t *token) {
  token_stack_assert(stack);

  assertion(token);

  if (stack->i == stack->stack_size - 1) {
    stack->stack_size = stack->stack_size * 2;
    stack->stack = realloc(stack->stack, stack->stack_size * sizeof(struct token_t *));
  }

  stack->stack[++stack->i] = token;
}

struct lexer_t {
  const char *src;
  size_t src_len;

  char c;
  size_t i;
};

struct lexer_t *lexer_init(const char *src);
struct token_t *lexer_get_next_token(struct lexer_t *lex);

#define assert_lexer(lex)                                                                                              \
  do {                                                                                                                 \
    assertion(lex);                                                                                                    \
    assertion(lex->src);                                                                                               \
  } while (0)

struct lexer_t *lexer_init(const char *src) {
  struct lexer_t *lex = calloc(1, sizeof(struct lexer_t));

  assertion(lex);
  assertion(src);

  lex->src = src;
  lex->src_len = strlen(src);
  lex->c = lex->src[0];

  return lex;
}

void lexer_free(struct lexer_t *lex) {
  assertion(lex);
  assertion(lex->src);

  free(lex->src);
  free(lex);
}

void lexer_next(struct lexer_t *lex) {
  assert_lexer(lex);

  if (lex->i < lex->src_len && lex->c != '\0') {
    lex->c = lex->src[++lex->i];
  }
}

struct token_t *lexer_next_token_current(struct lexer_t *lex, int id) {
  assert_lexer(lex);

  lexer_next(lex);

  return token_init(0, id);
}

char lexer_peek(struct lexer_t *lex, size_t n) {
  assert_lexer(lex);

  return lex->src[min(lex->i + n, lex->src_len)];
}

struct token_t *lexer_get_number(struct lexer_t *lex) {
  char *end, *begin;
  struct token_t *token;
  long num;

  assert_lexer(lex);

  token = token_init(0, TOKEN_INTEGER);
  begin = &lex->src[lex->i];

  if (lex->c == '-') {
    lexer_next(lex);
  }

  while (lex->c != '\0' && isdigit(lex->c)) {
    lexer_next(lex);
  }

  end = &lex->src[lex->i];
  num = strtol(begin, &end, 10);

  token->value = num;

  return token;
}

struct token_t *lexer_get_next_token(struct lexer_t *lex) {
  assert_lexer(lex);

  while (lex->c != '\0') {
    if (isspace(lex->c)) {
      lexer_next(lex);
      continue;
    }

    if (isdigit(lex->c) || (lex->c == '-' && isdigit(lexer_peek(lex, 1)))) {
      return lexer_get_number(lex);
    }

    switch (lex->c) {
    case '(':
      return lexer_next_token_current(lex, TOKEN_LPAREN);
    case ')':
      return lexer_next_token_current(lex, TOKEN_RPAREN);
    case '*':
      return lexer_next_token_current(lex, TOKEN_TIMES);
    case '/':
      return lexer_next_token_current(lex, TOKEN_DIV);
    case '+':
      return lexer_next_token_current(lex, TOKEN_PLUS);
    case '-':
      return lexer_next_token_current(lex, TOKEN_MINUS);
    default:
      return token_init(0, TOKEN_INVALID);
    }
  }

  return token_init(0, TOKEN_EOF);
}

struct token_queue_t *lexer_get_token_queue(struct lexer_t *lexer) {
  struct token_queue_t *queue;
  struct token_t *token;

  queue = token_queue_init();

  while ((token = lexer_get_next_token(lexer))->id != TOKEN_EOF) {
    if (token->id == TOKEN_INVALID) {
      token_queue_free(queue);
      return NULL;
    }

    token_queue_enqueue(queue, token);
  }

  free(token);

  return queue;
}

char *getueof() {
  char *buffer;
  int size = 256, i, c;

  buffer = calloc(size, sizeof(char));

  for (i = 0; (c = getchar()) != EOF; i++) {
    if (i == size) {
      buffer = realloc(buffer, (size *= 2));
    }

    buffer[i] = c;
  }

  return buffer;
}

struct token_queue_t *lexer_convert_to_rpn(struct lexer_t *lex, int *err);
long lexer_evaluate_rpn(struct token_queue_t *rpn, int *err);

/* https://en.wikipedia.org/wiki/Shunting-yard_algorithm */
struct token_queue_t *lexer_convert_to_rpn(struct lexer_t *lex, int *err) {
  struct token_queue_t *output_queue = token_queue_init();
  struct token_stack_t *operator_stack = token_stack_init();
  struct token_t *token;

  assert_lexer(lex);

  while ((token = lexer_get_next_token(lex))->id != TOKEN_EOF) {
    if (token->id == TOKEN_INVALID) {
      *err = 1;
      token_queue_free(output_queue);
      token_stack_free(operator_stack);
      return NULL;
    }

    /* When an operand is found, it is pushed to the output queue */
    if (token->id == TOKEN_INTEGER) {
      token_queue_enqueue(output_queue, token);
    }

    /* When a left parenthesis is found, it is pushed to the operator stack */
    else if (token->id == TOKEN_LPAREN) {
      token_stack_push(operator_stack, token);
    }

    else if (token->id == TOKEN_RPAREN) {
      while (!token_stack_is_empty(operator_stack) && token_stack_peek(operator_stack)->id != TOKEN_LPAREN) {
        token_queue_enqueue(output_queue, token_stack_pop(operator_stack));
      }

      if (token_stack_is_empty(operator_stack)) {
        // warning("message: mismatched parentheses\n");
        *err = 1;

        token_stack_free(operator_stack);
        token_queue_free(output_queue);

        return NULL;
      }

      free(token);
      free(token_stack_pop(operator_stack));
    }

    else {
      if (!token_stack_is_empty(operator_stack)) {
        struct token_t *token_next = token_stack_peek(operator_stack);
        int p1 = token_get_operator_precedence(token);
        int p2 = token_get_operator_precedence(token_next);

        int a1 = token_get_operator_associativity(token);

        while (!token_stack_is_empty(operator_stack) && token_next->id != TOKEN_LPAREN &&
               ((p2 > p1) || ((p2 == p1) && (a1 == LEFT_ASSOCIATIVE)))) {
          token_queue_enqueue(output_queue, token_stack_pop(operator_stack));

          if (!token_stack_is_empty(operator_stack)) {
            token_next = token_stack_peek(operator_stack);
            p2 = token_get_operator_precedence(token_next);
          }
        }
      }

      token_stack_push(operator_stack, token);
    }
  }

  free(token);

  while (!token_stack_is_empty(operator_stack)) {
    if (token_stack_peek(operator_stack)->id == TOKEN_LPAREN) {
      // warning("message: mismatched parentheses\n");
      *err = 1;

      token_stack_free(operator_stack);
      token_queue_free(output_queue);

      return NULL;
    }

    token_queue_enqueue(output_queue, token_stack_pop(operator_stack));
  }

  token_stack_free(operator_stack);

  token_queue_enqueue(output_queue, token_init(0, TOKEN_EOF));
  return output_queue;
}

void stack_execute_operation(struct token_stack_t *stack, long operation(long left, long right), int *err) {
  struct token_t *left, *right, *new;

  token_stack_assert(stack);

  assertion(!(token_stack_is_empty(stack)));

  if (!(right = token_stack_pop(stack))) {
    *err = 1;
    warning("message: invalid expression\n");
    return;
  }
  if (!(left = token_stack_pop(stack))) {
    *err = 1;
    free(left);
    warning("message: invalid expression\n");
    return;
  }

  new = token_init(0, TOKEN_INTEGER);
  new->value = operation(left->value, right->value);

  free(left);
  free(right);

  token_stack_push(stack, new);
}

long operation_plus(long left, long right) {
  return left + right;
}

long operation_minus(long left, long right) {
  return left - right;
}

long operation_times(long left, long right) {
  return left * right;
}

long operation_div(long left, long right) {
  return left / right;
}

long operation_pow(long left, long right) {
  long temp = 1;

  assertion(right >= 0);

  while (right) {
    temp = temp * left;
    right--;
  }

  return temp;
}

long lexer_evaluate_rpn(struct token_queue_t *rpn, int *err) {
  struct token_t *token, *right = NULL, *left = NULL;
  struct token_stack_t *stack = token_stack_init();
  int result;

  token_queue_assert(rpn);

  while ((token = token_queue_dequeue(rpn))->id != TOKEN_EOF) {
    if (token->id == TOKEN_INTEGER) {
      token_stack_push(stack, token);
      continue;
    }

    switch (token->id) {
    case TOKEN_PLUS:
      stack_execute_operation(stack, operation_plus, err);
      break;
    case TOKEN_MINUS:
      stack_execute_operation(stack, operation_minus, err);
      break;
    case TOKEN_TIMES:
      stack_execute_operation(stack, operation_times, err);
      break;
    case TOKEN_DIV:
      stack_execute_operation(stack, operation_div, err);
      break;
    default:
      warning("message: unknown operation\n");
      *err = 1;
      break;

      if (*err) {
        goto eval_cleanup;
      }
    }
  }

  free(token);

  token = token_stack_pop(stack);
  result = token->value;

  if (!token_stack_is_empty(stack)) {
    *err = 1;
  }

  free(token);

eval_cleanup:
  token_stack_free(stack);
  token_queue_free(rpn);

  return result;
}

int main() {
  struct lexer_t *lexer;
  struct token_queue_t *output;
  int err = 0;
  char *test_string;
  long result;

  test_string = getueof();
  lexer = lexer_init(test_string);

  output = lexer_convert_to_rpn(lexer, &err);
  if (err) {
    printf("ERROR\n");
    goto main_cleanup;
  }

  result = lexer_evaluate_rpn(output, &err);

  if (err) {
    printf("ERROR\n");
    goto main_cleanup;
  }

  printf("%ld", result);

main_cleanup:

  lexer_free(lexer);

  return 0;
}