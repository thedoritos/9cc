#include "9cc.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool starts_with(char *str, char *op) {
  return strncmp(str, op, strlen(op)) == 0;
}

bool is_num(char c) { return ('0' <= c && c <= '9'); }

bool is_alnum(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_') ||
         is_num(c);
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *t = calloc(1, sizeof(Token));
  t->kind = kind;
  t->str = str;
  t->len = len;
  cur->next = t;
  return t;
}

Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (starts_with(p, "return") && !is_alnum(p[6])) {
      cur = new_token(TK_RETURN, cur, p, 6);
      p += 6;
      continue;
    }

    if (starts_with(p, "if") && !is_alnum(p[2])) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p += 2;
      continue;
    }

    if (starts_with(p, "else") && !is_alnum(p[4])) {
      cur = new_token(TK_RESERVED, cur, p, 4);
      p += 4;
      continue;
    }

    if (starts_with(p, "while") && !is_alnum(p[5])) {
      cur = new_token(TK_RESERVED, cur, p, 5);
      p += 5;
      continue;
    }

    if (starts_with(p, "for") && !is_alnum(p[3])) {
      cur = new_token(TK_RESERVED, cur, p, 3);
      p += 3;
      continue;
    }

    if (is_alnum(*p) && !is_num(*p)) {
      char *q = p;
      while (is_alnum(*p)) {
        p++;
      }
      cur = new_token(TK_IDENT, cur, q, p - q);
      continue;
    }

    if (starts_with(p, "==") || starts_with(p, "!=") || starts_with(p, "<=") ||
        starts_with(p, ">=")) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p += 2;
      continue;
    }

    if (strchr("+-*/()<>=;", *p)) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      char *q = p;
      cur = new_token(TK_NUM, cur, p, 0);
      cur->val = strtol(p, &p, 10);
      cur->len = p - q;
      continue;
    }

    error("Failed to tokenize");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}
