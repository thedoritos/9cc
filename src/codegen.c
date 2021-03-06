#include "9cc.h"
#include <stdio.h>

void gen_lvar(Node *node) {
  if (node->kind != ND_LVAR)
    error("Left value is not an identifier");

  printf("    mov rax, rbp\n");
  printf("    sub rax, %d\n", node->offset);
  printf("    push rax\n");
}

void gen(Node *node) {
  switch (node->kind) {
  case ND_NUM:
    printf("    push %d\n", node->val);
    return;
  case ND_LVAR:
    gen_lvar(node);
    printf("    pop rax\n");
    printf("    mov rax, [rax]\n");
    printf("    push rax\n");
    return;
  case ND_ASSIGN:
    gen_lvar(node->lhs);
    gen(node->rhs);
    printf("    pop rdi\n");
    printf("    pop rax\n");
    printf("    mov [rax], rdi\n");
    printf("    push rdi\n");
    return;
  case ND_IF:
    gen(node->cond);
    printf("    pop rax\n");
    printf("    cmp rax, 0\n");
    printf("    je .Lelse\n");
    gen(node->lhs);
    printf("    jmp .Lend\n");
    printf(".Lelse:\n");
    if (node->rhs)
      gen(node->rhs);
    printf(".Lend:\n");
    return;
  case ND_WHILE:
    printf(".Lwhile:\n");
    gen(node->cond);
    printf("    pop rax\n");
    printf("    cmp rax, 0\n");
    printf("    je .Lend\n");
    gen(node->lhs);
    printf("    jmp .Lwhile\n");
    printf(".Lend:\n");
    return;
  case ND_FOR:
    if (node->init)
      gen(node->init);
    printf(".Lfor:\n");
    if (node->cond) {
      gen(node->cond);
      printf("    pop rax\n");
      printf("    cmp rax, 0\n");
      printf("    je .Lend\n");
    }
    gen(node->then);
    if (node->incr)
      gen(node->incr);
    printf("    jmp .Lfor\n");
    printf(".Lend:\n");
    return;
  case ND_BLOCK:
    node = node->body;
    printf("    \n");
    while (node != NULL) {
      gen(node);
      node = node->next;
      printf("    pop rax\n");
      printf("    \n");
    }
    return;
  case ND_RETURN:
    gen(node->lhs);
    printf("    pop rax\n");
    printf("    mov rsp, rbp\n");
    printf("    ret\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("    pop rdi\n");
  printf("    pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf("    add rax, rdi\n");
    break;
  case ND_SUB:
    printf("    sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("    imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("    cqo\n");
    printf("    idiv rdi\n");
    break;
  case ND_EQ:
    printf("    cmp rax, rdi\n");
    printf("    sete al\n");
    printf("    movzb rax, al\n");
    break;
  case ND_NE:
    printf("    cmp rax, rdi\n");
    printf("    setne al\n");
    printf("    movzb rax, al\n");
    break;
  case ND_LT:
    printf("    cmp rax, rdi\n");
    printf("    setl al\n");
    printf("    movzb rax, al\n");
    break;
  case ND_LE:
    printf("    cmp rax, rdi\n");
    printf("    setle al\n");
    printf("    movzb rax, al\n");
    break;
  }

  printf("    push rax\n");
}
