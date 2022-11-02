#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
  Add,
  Sub,
  Mul,
  Div,
  Mod,
} Operation;

typedef enum {
  Integer,
  Operator,
} ExprNodeUnionType;

typedef union {
  long long int value;
  Operation op;
} ExprNodeUnion;

typedef struct {
  ExprNodeUnion exprNode;
  ExprNodeUnionType type;
} ExprNode;

typedef struct ExprTree {
  struct ExprTree *right;
  struct ExprTree *left;
  ExprNode node;
} ExprTree;

ExprTree *makeExprLeaf(Operation op, long long int lhs, long long int rhs);
ExprTree *makeExprTree(ExprTree *left, Operation op, ExprTree *right);
void printExprTree(ExprTree *tree, int indent);
void freeExprTree(ExprTree *tree);
ExprTree *parseFile(TokenArray *tokens);
ExprTree *parseExpr(TokenArray *tokens, char *fileContents);

#endif
