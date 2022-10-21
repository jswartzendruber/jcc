#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_PERCENT,
  T_BACKSLASH,
  T_LESSTHAN,
  T_LESSEQUALTHAN,
  T_GREATERTHAN,
  T_GREATEREQUALTHAN,
  T_EQUAL,
  T_EQUALEQUAL,
  T_BANGEQUAL,
  T_AMPERAMPER,
  T_PIPEPIPE,
  T_BANG,
  T_SEMICOLON,
  T_COMMA,
  T_DOT,
  T_LBRACKET,
  T_RBRACKET,
  T_LPAREN,
  T_RPAREN,
  T_LCURLY,
  T_RCURLY,
  T_INTEGER,
  T_FLOAT,
  T_STRING,
  T_IDENTIFIER,
} TType;

char *enumToString(TType t) {
  if (t == T_PLUS) return "T_PLUS";
  if (t == T_MINUS) return "T_MINUS";
  if (t == T_STAR) return "T_STAR";
  if (t == T_SLASH) return "T_SLASH";
  if (t == T_PERCENT) return "T_PERCENT";
  if (t == T_BACKSLASH) return "T_BACKSLASH";
  if (t == T_LESSTHAN) return "T_LESSTHAN";
  if (t == T_LESSEQUALTHAN) return "T_LESSEQUALTHAN";
  if (t == T_GREATERTHAN) return "T_GREATERTHAN";
  if (t == T_GREATEREQUALTHAN) return "T_GREATEREQUALTHAN";
  if (t == T_EQUAL) return "T_EQUAL";
  if (t == T_EQUALEQUAL) return "T_EQUALEQUAL";
  if (t == T_BANGEQUAL) return "T_BANGEQUAL";
  if (t == T_AMPERAMPER) return "T_AMPERAMPER";
  if (t == T_PIPEPIPE) return "T_PIPEPIPE";
  if (t == T_BANG) return "T_BANG";
  if (t == T_SEMICOLON) return "T_SEMICOLON";
  if (t == T_COMMA) return "T_COMMA";
  if (t == T_DOT) return "T_DOT";
  if (t == T_LBRACKET) return "T_LBRACKET";
  if (t == T_RBRACKET) return "T_RBRACKET";
  if (t == T_LPAREN) return "T_LPAREN";
  if (t == T_RPAREN) return "T_RPAREN";
  if (t == T_LCURLY) return "T_LCURLY";
  if (t == T_RCURLY) return "T_RCURLY";
  if (t == T_INTEGER) return "T_INTEGER";
  if (t == T_FLOAT) return "T_FLOAT";
  if (t == T_STRING) return "T_STRING";
  if (t == T_IDENTIFIER) return "T_IDENTIFIER";
  return "";
}

typedef struct {
  TType type;
  int filePosStart;
  int filePosEnd;
} Token;

Token createToken(TType type, int filePosStart, int filePosEnd) {
  Token token;
  token.type = type;
  token.filePosStart = filePosStart;
  token.filePosEnd = filePosEnd;
  return token;
}

typedef struct {
  Token *data;
  int maxSize;
  int len;
} TokenArray;

void newTokenArrayList(TokenArray *array) {
  int startingSize = 2;
  array->data = malloc(sizeof(Token) * startingSize);
  if (array->data == NULL) {
    fprintf(stdout, "Error: Malloc failure.\n");
    exit(1);
  }

  array->maxSize = startingSize;
  array->len = 0;
}

void deleteTokenArrayList(TokenArray *array) { free(array->data); }

void insert(TokenArray *array, Token token) {
  if (array->len < array->maxSize) {
    array->data[array->len++] = token;
  } else {
    array->maxSize *= 3;
    array->data = realloc(array->data, array->maxSize * sizeof(Token));
    array->data[array->len++] = token;
    if (array->data == NULL) {
      fprintf(stdout, "Error: Realloc failure.\n");
      exit(1);
    }
  }
}

Token get(TokenArray *array, int idx) {
  if (idx < array->len) {
    return array->data[idx];
  } else {
    fprintf(stderr, "Panic: Array out of bounds!");
    abort();
  }
}

#endif
