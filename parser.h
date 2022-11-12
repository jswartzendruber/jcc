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

typedef union TACUnion {
  int regID;
  int integerConstant;
} TACUnion;

typedef enum TACQuadrupleType {
  TACRegister,
  TACInteger,
} TACQuadrupleType;

typedef struct TACOperandValue {
  TACUnion val;
  TACQuadrupleType type;
} TACOperandValue;

typedef struct TACDouble {
  int target;
  TACOperandValue arg;
} TACDouble;

typedef struct TACQuadruple {
  int target;
  Operation op;
  TACOperandValue arg1;
  TACOperandValue arg2;
} TACQuadruple;

typedef union TACValue {
  TACQuadruple quad;
  TACDouble doub;
  char* label;
} TACValue;

typedef enum TACType {
  Quadruple,
  Double,
  Label
} TACType;

typedef struct TAC {
  TACValue val;
  TACType type;
} TAC;

typedef struct TACList {
  TAC *data;
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
