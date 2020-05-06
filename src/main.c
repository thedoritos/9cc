#include "9cc.h"
#include <stdio.h>

char *user_input;
Token *token;
Node *code[100];

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Wrong number of args\n");
    return 1;
  }

  user_input = argv[1];
  token = tokenize(user_input);
  program();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  printf("    mov rbp, rsp\n");
  printf("    sub rsp, 208\n"); // 8 bytes x 26 variables

  for (int i = 0; code[i]; i++) {
    gen(code[i]);
    printf("    pop rax\n");
  }

  printf("    mov rsp, rbp\n");
  printf("    ret\n");
  return 0;
}
