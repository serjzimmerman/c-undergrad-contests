#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
  union {
    unsigned char regs[4];
    struct {
      unsigned char a, b, c, d;
    };
  };
} state = {0};

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

enum reg_e {
  REG_A = 0x0,
  REG_B = 0x1,
  REG_C = 0x2,
  REG_D = 0x3,
};

void execute_add(enum reg_e op1, enum reg_e op2) {
  unsigned char result;
  result          = state.regs[op1] + state.regs[op2];
  state.regs[op1] = result;
}

void execute_sub(enum reg_e op1, enum reg_e op2) {
  unsigned char result;
  result          = state.regs[op1] - state.regs[op2];
  state.regs[op1] = result;
}

void execute_mul(enum reg_e op1, enum reg_e op2) {
  unsigned char result;
  result          = state.regs[op1] * state.regs[op2];
  state.regs[op1] = result;
}

void execute_div(enum reg_e op1, enum reg_e op2) {
  unsigned char result;
  result          = state.regs[op1] / state.regs[op2];
  state.regs[op1] = result;
}

void execute_movi(unsigned char imm) {
  state.d = imm;
}

void execute_in(enum reg_e op) {
  unsigned char result;
  scanf("%hhu", &result);
  state.regs[op] = result;
}

void execute_out(enum reg_e op) {
  printf("%hhu\n", state.regs[op]);
}

enum operation_op2_e {
  OP_ADD = 0x0,
  OP_SUB = 0x1,
  OP_MUL = 0x2,
  OP_DIV = 0x3,
};

enum operation_op1_e {
  OP_IN  = 0x0,
  OP_OUT = 0x1,
};

void error_callback();

void execute(unsigned char code) {
  command_t command = {0};

  command.raw = code;

  if (!command.imm_bitfield.isnimm) {
    execute_movi(command.imm_bitfield.imm);
    return;
  }

  if (!command.op2_bitfield.is1op) {
    switch (command.op2_bitfield.opcode) {
    case OP_ADD:
      execute_add(command.op2_bitfield.op1, command.op2_bitfield.op2);
      break;
    case OP_SUB:
      execute_sub(command.op2_bitfield.op1, command.op2_bitfield.op2);
      break;
    case OP_MUL:
      execute_mul(command.op2_bitfield.op1, command.op2_bitfield.op2);
      break;
    case OP_DIV:
      execute_div(command.op2_bitfield.op1, command.op2_bitfield.op2);
      break;
    default:
      error_callback();
    }
  } else {
    switch (command.op1_bitfield.opcode) {
    case OP_IN:
      execute_in(command.op1_bitfield.op);
      break;
    case OP_OUT:
      execute_out(command.op1_bitfield.op);
      break;
    default:
      error_callback();
    }
  }
}

void error_callback() {
  printf("ERROR\n");
  exit(EXIT_SUCCESS);
}

unsigned char *getueof(FILE *fp) {
  unsigned char *buffer;
  int            size = 256, i, c;

  buffer = calloc(size, sizeof(char));

  for (i = 0; (c = getc(fp)) != EOF; i++) {
    if (i == size) {
      buffer = realloc(buffer, (size *= 2));
    }

    buffer[i] = c;
  }

  return buffer;
}

int main(int argc, char *argv[]) {
  unsigned char code, *program, *tok;
  FILE         *fp;

  fp      = fopen(argv[1], "r");
  program = getueof(fp);

  tok = strtok(program, " ");
  while (tok) {
    sscanf(tok, "%hhx", &code);
    execute(code);
    tok = strtok(NULL, " ");
  }

  return 0;
}