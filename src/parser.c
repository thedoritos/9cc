#include "9cc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool consume(char *op) {
  if (token->kind != TK_RESERVED || token->len != strlen(op) ||
      memcmp(token->str, op, token->len)) {
    return false;
  }
  token = token->next;
  return true;
}

Token *consume_ident() {
  if (token->kind != TK_IDENT) {
    return NULL;
  }
  Token *ident = token;
  token = token->next;
  return ident;
}

Token *consume_return() {
  if (token->kind != TK_RETURN) {
    return NULL;
  }
  Token *ret = token;
  token = token->next;
  return ret;
}

void expect(char *op) {
  if (token->kind != TK_RESERVED || memcmp(token->str, op, token->len)) {
    error_at(token->str, "Character is not a symbol of '%c'", op);
  }
  token = token->next;
}

int expect_number() {
  if (token->kind != TK_NUM) {
    error_at(token->str, "Character is not a number");
  }
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() { return token->kind == TK_EOF; }

LVar *locals;

LVar *find_lvar(Token *t) {
  for (LVar *var = locals; var; var = var->next)
    if (var->len == t->len && !memcmp(t->str, var->name, var->len))
      return var;
  return NULL;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_ident(Token *t) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_LVAR;

  LVar *lvar = find_lvar(t);
  if (lvar) {
    node->offset = lvar->offset;
    return node;
  }

  lvar = calloc(1, sizeof(LVar));
  lvar->next = locals;
  lvar->name = t->str;
  lvar->len = t->len;
  lvar->offset = locals ? locals->offset + 8 : 8;
  locals = lvar;

  node->offset = lvar->offset;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *primary() {
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  Token *ident = consume_ident();
  if (ident)
    return new_node_ident(ident);

  return new_node_num(expect_number());
}

Node *unary() {
  if (consume("+"))
    return primary();
  else if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  else
    return primary();
}

Node *mul() {
  Node *node = unary();
  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();
  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();
  for (;;) {
    if (consume("<"))
      node = new_node(ND_LT, node, add());
    else if (consume(">"))
      node = new_node(ND_LT, add(), node);
    else if (consume("<="))
      node = new_node(ND_LE, node, add());
    else if (consume(">="))
      node = new_node(ND_LE, add(), node);
    else
      return node;
  }
}

Node *equality() {
  Node *node = relational();
  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_node(ND_NE, node, relational());
    else
      return node;
  }
}

Node *assign() {
  Node *node = equality();
  if (consume("="))
    node = new_node(ND_ASSIGN, node, assign());
  return node;
}

Node *expr() { return assign(); }

Node *stmt() {
  if (consume_return()) {
    Node *node = new_node(ND_RETURN, expr(), NULL);
    expect(";");
    return node;
  }

  if (consume("if")) {
    expect("(");
    Node *cond = expr();
    expect(")");
    Node *node = new_node(ND_IF, expr(), NULL);
    expect(";");
    node->cond = cond;
    if (consume("else")) {
      node->rhs = expr();
      expect(";");
    }
    return node;
  }

  Node *node = expr();
  expect(";");
  return node;
}

void program() {
  int i = 0;
  while (!at_eof())
    code[i++] = stmt();
  code[i] = NULL;
}
