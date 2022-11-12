#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void newTACList(TACList *list) {
  int startingSize = 32;
  list->data = malloc(sizeof(TAC) * startingSize);
  if (list->data == NULL) {
    fprintf(stdout, "Error: Malloc failure.\n");
    exit(1);
  }

  list->maxSize = startingSize;
  list->current = 0;
  list->len = 0;
}

void freeTACList(TACList *list) { free(list->data); }

void insertTAC(TACList *list, TAC value) {
  if (list->len < list->maxSize) {
    list->data[list->len++] = value;
  } else {
    list->maxSize *= 2;
    list->data = realloc(list->data, list->maxSize * sizeof(TAC));
    list->data[list->len++] = value;
    if (list->data == NULL) {
      fprintf(stdout, "Error: Realloc failure.\n");
      exit(1);
    }
  }
}

TAC getTAC(TACList *list, int idx) {
  if (idx < list->len) {
    return list->data[idx];
  } else {
    fprintf(stderr, "Panic: List out of bounds!");
    abort();
  }
}

Token expect(TokenList *tokens, char *fileContents, TType expected, char* errorMsg) {
  Token tok = advance(tokens, fileContents);
  if (tok.type != expected) {
    fprintf(stdout, "%s", errorMsg);
    exit(1);
  }
  return tok;
}

ExprNode makeValNode(long value) {
  ExprNode node;
  node.exprNode.value = value;
  node.type = Integer;
  return node;
}

ExprNode makeOpNode(Operation op) {
  ExprNode node;
  node.exprNode.op = op;
  node.type = Operator;
  return node;
}

ExprTree *makeTreeNode(ExprNode node) {
  ExprTree *tree = malloc(sizeof(ExprTree));
  if (tree != NULL) {
    tree->node = node;
    tree->right = NULL;
    tree->left = NULL;
  }
  return tree;
}

AST *makeFunctionDeclaration(TType returnType, char *functionName, Statement statement) {
  AST *func = malloc(sizeof(AST));
  if (func != NULL) {
    func->node.type = returnType;
    func->node.name = functionName;
    func->node.statement = statement;
    func->right = NULL;
    func->left = NULL;
  }
  return func;
}

ExprTree *makeExprTree(ExprTree *left, Operation op, ExprTree *right) {
  ExprTree *tree = makeTreeNode(makeOpNode(op));
  tree->right = right;
  tree->left = left;
  return tree;
}

void freeExprTree(ExprTree *tree) {
  if (tree != NULL) {
    freeExprTree(tree->left);
    freeExprTree(tree->right);
    free(tree);
  }
}

void freeAST(AST *ast) {
  // Function declaration
  if (ast != NULL) {
    free(ast->node.name);
    freeExprTree(ast->node.statement.ret.val);
    freeAST(ast->right);
    freeAST(ast->left);
    free(ast);
  }
}

ExprTree *parseFactor(TokenList *tokens, char *fileContents) {
  Token currToken = advance(tokens, fileContents);

  if (currToken.type == T_INTEGER) {
    return makeTreeNode(makeValNode(getTokenIntValue(currToken, fileContents)));
  } else if (currToken.type == T_LPAREN) {
    ExprTree *expr = parseExpr(tokens, fileContents);
    expect(tokens, fileContents, T_RPAREN, "Error: Expected ')'");
    return expr;
  } else {
    fprintf(stdout, "Error: Expected factor.");
    exit(1);
  }
}

ExprTree *parseTerm(TokenList *tokens, char *fileContents) {
  ExprTree *lhs = parseFactor(tokens, fileContents);

  while (current(tokens, fileContents).type == T_SLASH || current(tokens, fileContents).type == T_STAR) {
    Token currToken = advance(tokens, fileContents);
    Operation op;
    if (currToken.type == T_SLASH) {
      op = Div;
    } else {
      op = Mul;
    }
    ExprTree *rhs = parseFactor(tokens, fileContents);

    lhs = makeExprTree(lhs, op, rhs);
  }

  return lhs;
}

ExprTree *parseExpr(TokenList *tokens, char *fileContents) {
  ExprTree *lhs = parseTerm(tokens, fileContents);

  while (current(tokens, fileContents).type == T_PLUS || current(tokens, fileContents).type == T_MINUS) {
    Token currToken = advance(tokens, fileContents);
    Operation op;
    if (currToken.type == T_PLUS) {
      op = Add;
    } else {
      op = Sub;
    }
    ExprTree *rhs = parseTerm(tokens, fileContents);

    lhs = makeExprTree(lhs, op, rhs);
  }

  return lhs;
}

TType parseTypeSpecifier(TokenList *tokens, char *fileContents) {
  return advance(tokens, fileContents).type;
}

char *parseIdentifier(TokenList *tokens, char *fileContents) {
  Token token = advance(tokens, fileContents);
  int len = token.filePosEnd - token.filePosStart;
  char *ident = malloc(len + 1);
  memcpy(ident, &fileContents[token.filePosStart], len);
  ident[len] = '\0';
  return ident;
}

Statement parseStatement(TokenList *tokens, char *fileContents) {
  // Return statement
  Token tok = expect(tokens, fileContents, K_RETURN, "Error: Expected 'return'");
  ExprTree *expr = parseExpr(tokens, fileContents);
  expect(tokens, fileContents, T_SEMICOLON, "Error: Expected ';'");

  Statement statement;
  statement.ret.val = expr;
  return statement;
}

AST *parseFunctionDeclaration(TokenList *tokens, char *fileContents) {
  int returnType = parseTypeSpecifier(tokens, fileContents);
  char *functionName = parseIdentifier(tokens, fileContents);

  expect(tokens, fileContents, T_LPAREN, "Error: Expected '('");
  // TODO: function arguments
  expect(tokens, fileContents, T_RPAREN, "Error: Expected ')'");
  expect(tokens, fileContents, T_LCURLY, "Error: Expected '{'");

  Statement statement = parseStatement(tokens, fileContents);
  return makeFunctionDeclaration(returnType, functionName, statement);
}

AST *parseFile(TokenList *tokens, char *fileContents) {
  return parseFunctionDeclaration(tokens, fileContents);
}

TAC newTACQuad(int targetRegister, Operation op, int reg1, int reg2) {
  TAC value;
  TACQuadruple quad;
  quad.target = targetRegister;
  quad.op = op;
  TACOperandValue val1;
  val1.val.regID = reg1;
  val1.type = TACRegister;
  quad.arg1 = val1;
  TACOperandValue val2;
  val2.val.regID = reg2;
  val2.type = TACRegister;
  quad.arg2 = val2;
  value.val.quad = quad;
  value.type = Quadruple;

  return value;
}

TAC newTACDouble(int targetRegister, int arg) {
  TAC value;
  TACDouble d;
  d.target = targetRegister;
  TACOperandValue val1;
  val1.val.integerConstant = arg;
  val1.type = TACInteger;
  d.arg = val1;
  value.type = Double;
  value.val.doub = d;

  return value;
}

TAC newTACLabel(char *label) {
  TAC value;
  value.type = Label;
  value.val.label = label;
  return value;
}

void printTAC(TAC t) {
  if (t.type == Label) {
    printf("%s:\n", t.val.label);
  } else if (t.type == Quadruple) {
    char* opString;
    if (t.val.quad.op == Add) {
      opString = "+";
    } else if (t.val.quad.op == Sub) {
      opString = "-";
    } else if (t.val.quad.op == Mul) {
      opString = "*";
    } else if (t.val.quad.op == Div) {
      opString = "/";
    } else {
      opString = "?";
    }

    char arg1[32];
    char arg2[32];
    if (t.val.quad.arg1.type == TACRegister) {
      sprintf(arg1, "_t%i", t.val.quad.arg1.val.regID);
    } else if (t.val.quad.arg1.type == TACInteger) {
      sprintf(arg1, "%i", t.val.quad.arg1.val.integerConstant);
    }
    if (t.val.quad.arg2.type == TACRegister) {
      sprintf(arg2, "_t%i", t.val.quad.arg2.val.regID);
    } else if (t.val.quad.arg1.type == TACInteger) {
      sprintf(arg2, "%i", t.val.quad.arg2.val.integerConstant);
    }

    printf("_t%i = %s %s, %s\n", t.val.quad.target, opString, arg1, arg2);
  } else if (t.type == Double) {
    printf("_t%i = %i\n", t.val.doub.target, t.val.doub.arg.val.integerConstant);
  }
}

void generateExprTAC(TACList *list, ExprTree *expr) {
  /* TAC label = newTACLabel("Main"); */
  /* TAC inter = newTACQuad(2, Div, 3, 4); */
  /* printTAC(label); */
  /* printTAC(inter); */
  printExprTree(expr, 0);
  /* if (tree->node.type == Operator) { */
  /*   if (tree->node.exprNode.op == Add) */
  /*     printf("%*s+\n", indent, ""); */
  /*   else if (tree->node.exprNode.op == Sub) */
  /*     printf("%*s-\n", indent, ""); */
  /*   else if (tree->node.exprNode.op == Mul) */
  /*     printf("%*s*\n", indent, ""); */
  /*   else if (tree->node.exprNode.op == Div) */
  /*     printf("%*s/\n", indent, ""); */
  /*   indent++; */
  /*   printExprTree(tree->left, indent); */
  /*   printExprTree(tree->right, indent); */
  /* } else { */
  /*   printf("%*s%li\n", indent, "", tree->node.exprNode.value); */
  /* } */

  // Create TACList, add to it.

  /* TAC t = newTACDouble(0, 69); */
  /* printTAC(t); */
}

void generateStatementTAC(TACList *list, Statement statement) {
  // Do we need a TAC for BeginFunc n (stack space), EndFunc?
  // I believe n includes space for callee saved registers.
  generateExprTAC(list, statement.ret.val);
}

void generateTAC(TACList *list, AST *ast) {
  newTACList(list);

  // Generate main
  if (strcmp(ast->node.name, "main") == 0) {
    insertTAC(list, newTACLabel(ast->node.name));
    generateStatementTAC(list, ast->node.statement);
  }
}

void printAST(AST *ast, int indent) {
  // Function declaration
  printf("%s %s:\n", enumToString(ast->node.type), ast->node.name);
  indent++;
  // Statement
  printf("%*sreturn\n", indent++, "");
  printExprTree(ast->node.statement.ret.val, indent);
}

void printExprTree(ExprTree *tree, int indent) {
  if (tree->node.type == Operator) {
    if (tree->node.exprNode.op == Add)
      printf("%*s+\n", indent, "");
    else if (tree->node.exprNode.op == Sub)
      printf("%*s-\n", indent, "");
    else if (tree->node.exprNode.op == Mul)
      printf("%*s*\n", indent, "");
    else if (tree->node.exprNode.op == Div)
      printf("%*s/\n", indent, "");
    indent++;
    printExprTree(tree->left, indent);
    printExprTree(tree->right, indent);
  } else {
    printf("%*s%li\n", indent, "", tree->node.exprNode.value);
  }
}
