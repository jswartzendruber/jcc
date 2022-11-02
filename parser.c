#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ExprNode makeValNode(long long int value) {
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

// maybe just use the primitives functions?
ExprTree *makeExprLeaf(Operation op, long long int lhs, long long int rhs) {
  ExprTree *tree = makeTreeNode(makeOpNode(op));
  tree->right = makeTreeNode(makeValNode(rhs));
  tree->left = makeTreeNode(makeValNode(lhs));
  return tree;
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

ExprTree *parseFactor(TokenArray *tokens, char *fileContents) {
  Token currToken = advance(tokens, fileContents);

  if (currToken.type == T_INTEGER) {
    return makeTreeNode(makeValNode(getTokenIntValue(currToken, fileContents)));
  } else if (currToken.type == T_LPAREN) {
    ExprTree *expr = parseExpr(tokens, fileContents);
    Token rParen = advance(tokens, fileContents);
    if (rParen.type == T_RPAREN) {
      return expr;
    } else {
      fprintf(stdout, "Error: Expected '('");
      exit(1);
    }
  } else {
    fprintf(stdout, "Error: Expected factor.");
    exit(1);
  }
}

ExprTree *parseTerm(TokenArray *tokens, char *fileContents) {
  ExprTree *lhs = parseFactor(tokens, fileContents);

  while (current(tokens, fileContents).type == T_SLASH || current(tokens, fileContents).type == T_STAR) {
    Token currToken = current(tokens, fileContents);
    Operation op;
    if (currToken.type == T_SLASH) {
      op = Div;
    } else {
      op = Mul;
    }
    advance(tokens, fileContents); // Operation
    ExprTree *rhs = parseFactor(tokens, fileContents);

    lhs = makeExprTree(lhs, op, rhs);
  }

  return lhs;
}

ExprTree *parseExpr(TokenArray *tokens, char *fileContents) {
  ExprTree *lhs = parseTerm(tokens, fileContents);

  while (current(tokens, fileContents).type == T_PLUS || current(tokens, fileContents).type == T_MINUS) {
    Token currToken = current(tokens, fileContents);
    Operation op;
    if (currToken.type == T_PLUS) {
      op = Add;
    } else {
      op = Sub;
    }
    advance(tokens, fileContents); // Operation
    ExprTree *rhs = parseTerm(tokens, fileContents);

    lhs = makeExprTree(lhs, op, rhs);
  }

  return lhs;
}

ExprTree *parseFile(TokenArray *tokens) {
  TokenArray expr;
  newTokenArrayList(&expr);
  char *backing = "3 * (4 + 1) / 6";
  tokenizeFile(&expr, backing, strlen(backing));
  ExprTree *ex = parseExpr(&expr, backing);
  printExprTree(ex, 0);

  deleteTokenArrayList(&expr);

  return ex;
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
    printf("%*s%i\n", indent, "", tree->node.exprNode.value);
  }
}
