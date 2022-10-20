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
  T_LBRACE,
  T_RBRACE,
  T_INTEGER,
  T_FLOAT,
  T_STRING,
  T_CHAR,
  T_TRUE,
  T_FALSE,
} TType;

typedef struct {
  TType type;
  int filePosStart;
  int filePosEnd;
} Token;

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

void insert(TokenArray *array, Token *token) {
  if (array->len < array->maxSize) {
    array->data[array->len++] = *token;
  } else {
    printf("new size: %i\n", array->maxSize * 3);
    array->maxSize *= 3;
    array->data = realloc(array->data, array->maxSize * sizeof(Token));
    array->data[array->len++] = *token;
    if (array->data == NULL) {
      fprintf(stdout, "Error: Realloc failure.\n");
      exit(1);
    }
  }
}

Token *get(TokenArray *array, int idx) {
  if (idx < array->len) {
    return &array->data[idx];
  } else {
    fprintf(stderr, "Panic: Array out of bounds!");
    abort();
  }
}

#endif
