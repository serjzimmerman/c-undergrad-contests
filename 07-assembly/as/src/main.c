/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -E -G -L ANSI-C -t src/keywords.gperf  */
/* Computed positions: -k'1' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) &&             \
      (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) &&              \
      ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) &&              \
      ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) &&              \
      ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) &&              \
      ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) &&              \
      ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) &&              \
      ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) &&             \
      ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) &&       \
      ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) &&      \
      ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) &&      \
      ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#include <stdlib.h>
#include <string.h>

enum keyword_e {
  OP_TWO_IMM_MOVI = 0x0,
  OP_TWO_ADD      = 0x1,
  OP_TWO_SUB      = 0x2,
  OP_TWO_MUL      = 0x3,
  OP_TWO_DIV      = 0x4,
  OP_ONE_IN       = 0x5,
  OP_ONE_OUT      = 0x6,
  REG_A           = 0x7,
  REG_B           = 0x8,
  REG_C           = 0x9,
  REG_D           = 0xa,
};

struct keyword_s {
  char          *name;
  enum keyword_e keyword;
};
enum { TOTAL_KEYWORDS = 11, MIN_WORD_LENGTH = 1, MAX_WORD_LENGTH = 4, MIN_HASH_VALUE = 1, MAX_HASH_VALUE = 18 };

/* maximum key range = 18, duplicates = 0 */
static unsigned int hash(register const char *str, register size_t len) {
  static unsigned char asso_values[] = {
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 10, 15, 0,  5,  19, 19, 19, 19, 0,  19, 19, 19, 0,  19, 4,  19, 19, 19, 15, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19};
  return len + asso_values[(unsigned char)str[0]];
}

static struct keyword_s wordlist[] = {{""},
                                      {"C", REG_C},
                                      {"IN", OP_ONE_IN},
                                      {"MUL", OP_TWO_MUL},
                                      {"MOVI", OP_TWO_IMM_MOVI},
                                      {""},
                                      {"D", REG_D},
                                      {"OUT", OP_ONE_OUT},
                                      {"DIV", OP_TWO_DIV},
                                      {""},
                                      {""},
                                      {"A", REG_A},
                                      {""},
                                      {"ADD", OP_TWO_ADD},
                                      {""},
                                      {""},
                                      {"B", REG_B},
                                      {""},
                                      {"SUB", OP_TWO_SUB}};

struct keyword_s *in_word_set(register const char *str, register size_t len) {
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH) {
    register unsigned int key = hash(str, len);

    if (key <= MAX_HASH_VALUE) {
      register const char *s = wordlist[key].name;

      if (*str == *s && !strcmp(str + 1, s + 1))
        return &wordlist[key];
    }
  }
  return 0;
}
/* End of auto-generated code */

/* Debug messages */
#if defined(DEBUG)

#define warning(format, ...) message("warning", format, ##__VA_ARGS__);
#define error(format, ...)                                                                                                                 \
  do {                                                                                                                                     \
    message("fatal error", format, ##__VA_ARGS__);                                                                                         \
    exit(EXIT_FAILURE);                                                                                                                    \
  } while (0)

#else
#define warning(format, ...)                                                                                                               \
  do {                                                                                                                                     \
  } while (0)
#define error(format, ...)                                                                                                                 \
  do {                                                                                                                                     \
    exit(EXIT_FAILURE);                                                                                                                    \
  } while (0)
#endif

#define message(errtype, format, ...)                                                                                                      \
  do {                                                                                                                                     \
    fprintf(stderr, "%s: at line %d of %s in function %s\n" format, errtype, __LINE__, __FILE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);      \
  } while (0)

#define assertion(condition)                                                                                                               \
  if (!(condition)) {                                                                                                                      \
    do {                                                                                                                                   \
      message("Assertion (" #condition ") failed", "");                                                                                    \
      exit(EXIT_FAILURE);                                                                                                                  \
    } while (0);                                                                                                                           \
  }

/* Sync token enum values to keyword value */
enum token_id_e {
  TOKEN_OP_TWO_IMM_MOVI = 0x0,
  TOKEN_OP_TWO_ADD      = 0x1,
  TOKEN_OP_TWO_SUB      = 0x2,
  TOKEN_OP_TWO_MUL      = 0x3,
  TOKEN_OP_TWO_DIV      = 0x4,
  TOKEN_OP_ONE_IN       = 0x5,
  TOKEN_OP_ONE_OUT      = 0x6,
  TOKEN_REG             = 0x7,
  TOKEN_IMM             = 0x8,
  TOKEN_COMMA           = 0x9,
  TOKEN_EOF             = -1,
  TOKEN_INVALID         = -2,
};

enum reg_e {
  EREG_A = 0x0,
  EREG_B = 0x1,
  EREG_C = 0x2,
  EREG_D = 0x3,
};

typedef struct {
  enum token_id_e id;
  union {
    enum reg_e reg;
    unsigned   value;
  };
} token_t;

token_t token_inits_reg(enum reg_e reg) {
  token_t token = {0};
  token.id      = TOKEN_REG;
  token.reg     = reg;
  return token;
}

token_t token_inits_imm(enum token_id_e id, unsigned value) {
  token_t token = {0};
  token.id      = id;
  token.value   = value;
  return token;
}

token_t token_inits_keyword(enum token_id_e id) {
  token_t token = {0};
  token.id      = id;
  return token;
}

#define TOKI(value) token_inits_imm(TOKEN_IMM, value)
#define TOKT(id)    token_inits_keyword(id)
#define TOKR(reg)   token_inits_reg(reg)

#include <ctype.h>
#include <stdio.h>

struct {
  const char *src;
  size_t      len, i;

  char    c;
  token_t curr;
} lexer;

void next_char() {
  if (lexer.i < lexer.len && lexer.c != '\0') {
    lexer.c = lexer.src[++lexer.i];
  }
}

char curr_char() {
  return lexer.c;
}

token_t curr() {
  return lexer.curr;
}

token_t get_number() {
  const char *end, *begin;
  long        num;

  begin = &lexer.src[lexer.i];

  while (curr_char() != '\0' && isdigit(curr_char())) {
    next_char();
  }

  end = &lexer.src[lexer.i];
  num = strtol(begin, &end, 10);

  return TOKI(num);
}

#define MAX_KEYWORD_LEN 128
token_t get_keyword() {
  char   str[MAX_KEYWORD_LEN] = {0};
  size_t i                    = 0;

  struct keyword_s *word;

  while (curr_char() != '\0' && isalpha(curr_char())) {
    if (i == MAX_KEYWORD_LEN - 1) {
      return TOKT(TOKEN_INVALID);
    }
    str[i++] = curr_char();
    next_char();
  }

  word = in_word_set(str, i);
  if (!word) {
    return TOKT(TOKEN_INVALID);
  }

  if (word->keyword < REG_A) {
    return TOKT(word->keyword);
  } else {
    return TOKR(word->keyword - REG_A);
  }
}

token_t next() {
  token_t tok = TOKT(TOKEN_EOF);

  while (curr_char() != '\0') {
    if (isspace(curr_char())) {
      next_char();
      continue;
    }

    if (isdigit(curr_char())) {
      tok = get_number();
      goto next_exit;
    }

    if (isalpha(curr_char())) {
      tok = get_keyword();
      goto next_exit;
    }

    switch (curr_char()) {
    case ',':
      tok = TOKT(TOKEN_COMMA);
      break;
    default:
      tok = TOKT(TOKEN_INVALID);
      break;
    }

    next_char();
    goto next_exit;
  }

next_exit:
  lexer.curr = tok;
  return tok;
}

void set_source(const char *src) {
  memset(&lexer, 0, sizeof(lexer));

  lexer.src = src;
  lexer.len = strlen(src);
  lexer.c   = src[0];

  lexer.curr = next();
}

/* Type punning with bit fields is actually undefined behaviour, but most commonly bits are placed in LSB first order.
 * Thus, we can get away with it */
typedef struct {
  union {
    unsigned char raw;

    struct {
      unsigned char imm    : 7;
      unsigned char isnimm : 1;
    } imm_bitfield;

    struct {
      unsigned char op2    : 2;
      unsigned char op1    : 2;
      unsigned char opcode : 2;
      unsigned char is1op  : 1;
      unsigned char isnimm : 1;
    } op2_bitfield;

    struct {
      unsigned char op       : 2;
      unsigned char opcode   : 1;
      unsigned char reserved : 3;
      unsigned char is1op    : 1;
      unsigned char isnimm   : 1;
    } op1_bitfield;
  };
} command_t;

void parse_error_callback() {
  printf("ERROR\n");
  exit(0);
}

command_t parse_movi() {
  command_t result = {0};

  if (curr().id != TOKEN_OP_TWO_IMM_MOVI) {
    parse_error_callback();
  }

  result.imm_bitfield.isnimm = 0;
  if (next().id != TOKEN_IMM) {
    parse_error_callback();
  }

  if (curr().value > 127) {
    parse_error_callback();
  }

  result.imm_bitfield.imm = curr().value;
  next();

  return result;
}

command_t parse_oneop() {
  command_t result = {0};

  if (curr().id != TOKEN_OP_ONE_IN && curr().id != TOKEN_OP_ONE_OUT) {
    parse_error_callback();
  }

  result.op1_bitfield.isnimm = 1;
  result.op1_bitfield.is1op  = 1;

  result.op1_bitfield.opcode = curr().id - TOKEN_OP_ONE_IN;

  if (next().id != TOKEN_REG) {
    parse_error_callback();
  }

  result.op1_bitfield.op = curr().reg;
  next();

  return result;
}

command_t parse_twoop() {
  command_t result = {0};

  if (curr().id > TOKEN_OP_TWO_DIV || curr().id < TOKEN_OP_TWO_ADD) {
    parse_error_callback();
  }

  result.op2_bitfield.isnimm = 1;
  result.op2_bitfield.is1op  = 0;
  result.op2_bitfield.opcode = curr().id - TOKEN_OP_TWO_ADD;

  if (next().id != TOKEN_REG) {
    parse_error_callback();
  }

  result.op2_bitfield.op1 = curr().reg;

  if (next().id != TOKEN_COMMA) {
    parse_error_callback();
  }

  if (next().id != TOKEN_REG) {
    parse_error_callback();
  }

  result.op2_bitfield.op2 = curr().reg;
  next();

  return result;
}

command_t parse_instruction(int *complete) {
  command_t result;
  result.raw = 0xff;

  switch (curr().id) {
  case TOKEN_OP_TWO_IMM_MOVI:
    return parse_movi();
  case TOKEN_OP_TWO_ADD:
    return parse_twoop();
  case TOKEN_OP_TWO_SUB:
    return parse_twoop();
  case TOKEN_OP_TWO_MUL:
    return parse_twoop();
  case TOKEN_OP_TWO_DIV:
    return parse_twoop();
  case TOKEN_OP_ONE_IN:
    return parse_oneop();
  case TOKEN_OP_ONE_OUT:
    return parse_oneop();
  case TOKEN_EOF:
    *complete = 1;
    break;
  default:
    parse_error_callback();
    break;
  }

  return result;
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
  command_t com;
  char     *input;

  int complete = 0;

  input = getueof();
  set_source(input);

  while (!complete) {
    com = parse_instruction(&complete);
    if (!complete) {
      printf("0x%x ", com.raw);
    }
  }
}