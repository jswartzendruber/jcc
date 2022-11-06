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

ExprTree *parseExpr(TokenArray *tokens, char *fileContents) {
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

TType parseTypeSpecifier(TokenArray *tokens, char *fileContents) {
  return advance(tokens, fileContents).type;
}

char *parseIdentifier(TokenArray *tokens, char *fileContents) {
  Token token = advance(tokens, fileContents);
  int len = token.filePosEnd - token.filePosStart;
  char *ident = malloc(len);
  memcpy(ident, &fileContents[token.filePosStart], len);
  ident[len] = '\0';
  return ident;
}

Statement parseStatement(TokenArray *tokens, char *fileContents) {
  // Return statement
  Token tok = advance(tokens, fileContents);
  if (tok.type != K_RETURN) {
    fprintf(stdout, "Error: Expected 'return'");
    exit(1);
  }
  ExprTree *expr = parseExpr(tokens, fileContents);
  if (advance(tokens, fileContents).type != T_SEMICOLON) {
    fprintf(stdout, "Error: Expected ';'");
    exit(1);
  }

  Statement statement;
  statement.ret.val = expr;
  return statement;
}

ExprTree *parseFunctionDeclaration(TokenArray *tokens, char *fileContents) {
  int returnType = parseTypeSpecifier(tokens, fileContents);
  char *functionName = parseIdentifier(tokens, fileContents);
  free(functionName); // TODO: REMOVE THIS TEMPORARY SANITIZER SILENCER

  if (advance(tokens, fileContents).type != T_LPAREN) {
    fprintf(stdout, "Error: Expected '('");
    exit(1);
  }
  if (advance(tokens, fileContents).type != T_RPAREN) {
    fprintf(stdout, "Error: Expected ')'");
    exit(1);
  }
  if (advance(tokens, fileContents).type != T_LCURLY) {
    fprintf(stdout, "Error: Expected '{'");
    exit(1);
  }

  Statement statement = parseStatement(tokens, fileContents);
}

ExprTree *parseFile(TokenArray *tokens, char *fileContents) {
  return parseFunctionDeclaration(tokens, fileContents);
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
