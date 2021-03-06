#include <stdbool.h>

typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_RETURN,
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

typedef struct LVar LVar;

struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_ASSIGN,
  ND_LVAR,
  ND_NUM,
  ND_EQ,
  ND_NE,
  ND_LT,
  ND_LE,
  ND_RETURN,
  ND_IF,
  ND_WHILE,
  ND_FOR,
  ND_BLOCK,
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *cond;
  Node *lhs;
  Node *rhs;

  // "for" statement
  Node *init;
  Node *incr;
  Node *then;

  // block
  Node *body;
  Node *next;

  int val;
  int offset;
};

extern char *user_input;
extern Token *token;
extern Node *code[];

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

Token *tokenize(char *p);

Node *expr();
void program();

void gen(Node *node);
