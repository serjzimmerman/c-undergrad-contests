#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum reg_e {
  A = 0x0,
  B = 0x1,
  C = 0x2,
  D = 0x3,
};

const char *const reg_name[] = {"A", "B", "C", "D"};

const char *const two_op_name[] = {"ADD", "SUB", "MUL", "DIV"};

const char *const one_op_name[] = {"IN", "OUT"};

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

void decode(unsigned char code, char *dest) {
  command_t command;

  command.raw = code;

  if (!command.imm_bitfield.isnimm) {
    sprintf(dest, "MOVI %hhd", command.imm_bitfield.imm);
    return;
  }

  if (!command.op2_bitfield.is1op) {
    sprintf(dest, "%s %s, %s", two_op_name[command.op2_bitfield.opcode], reg_name[command.op2_bitfield.op1],
            reg_name[command.op2_bitfield.op2]);
  } else {
    sprintf(dest, "%s %s", one_op_name[command.op1_bitfield.opcode], reg_name[command.op1_bitfield.op]);
  }

  return;
};

int main() {
  int           res;
  unsigned char data;
  char          buf[128] = {0};

  for (;;) {
    res = scanf("%hhx", &data);

    if (res != 1) {
      break;
    }

    decode(data, buf);
    printf("%s\n", buf);
  }
}