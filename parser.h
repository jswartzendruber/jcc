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

typedef struct Return {
  ExprTree *val;
} Return;

typedef union Statement {
  Return ret;
} Statement;

typedef struct FunctionDeclaration {
  TType type;
  char *name;
  Statement statement;
} FunctionDeclaration;

typedef struct AST {
  struct AST *right;
  struct AST *left;
  FunctionDeclaration node;
} AST;

/*
typedef struct Quadruple {
  int target;
  int op;
  int arg1;
  int arg2;
} Quadruple;

typedef struct TAC {
  Quadruple *data;
  int maxSize;
  int current;
  int len;
} TAC;
*/

ExprTree *makeExprTree(ExprTree *left, Operation op, ExprTree *right);
void printExprTree(ExprTree *tree, int indent);
void printAST(AST *ast, int indent);
void freeExprTree(ExprTree *tree);
void freeAST(AST *ast);
AST *parseFile(TokenArray *tokens, char *fileContents);
ExprTree *parseExpr(TokenArray *tokens, char *fileContents);

#endif
