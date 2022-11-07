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
  long value;
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

typedef struct VirtualRegister {
  int id;
} VirtualRegister;

typedef union TACUnion {
  VirtualRegister reg;
  int integerConstant;
} TACUnion;

typedef enum TACType {
  TACRegister,
  TACInteger,
} TACType;

typedef struct TACValue {
  TACUnion val;
  TACType type;
} TACValue;

typedef struct Quadruple {
  VirtualRegister target;
  Operation op;
  TACValue arg1;
  TACValue arg2;
} Quadruple;

typedef struct TACList {
  Quadruple *data;
  int maxSize;
  int current;
  int len;
} TACList;

void generateTAC(TACList *list, AST *ast);
void freeTACList(TACList *list);
ExprTree *makeExprTree(ExprTree *left, Operation op, ExprTree *right);
void printExprTree(ExprTree *tree, int indent);
void printAST(AST *ast, int indent);
void freeExprTree(ExprTree *tree);
void freeAST(AST *ast);
AST *parseFile(TokenList *tokens, char *fileContents);
ExprTree *parseExpr(TokenList *tokens, char *fileContents);

#endif
