#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* tokenStringValue(char *buff, Token token, char *fileContents) {
  int j = 0;
  for (int i = token.filePosStart; i < token.filePosEnd; i++) {
    buff[j++] = fileContents[i];
  }
  buff[j] = '\0';
  return buff;
}

char *enumToString(TType t) {
  if (t == T_PLUS) return "T_PLUS";
  if (t == T_MINUS) return "T_MINUS";
  if (t == T_STAR) return "T_STAR";
  if (t == T_SLASH) return "T_SLASH";
  if (t == T_EQUAL) return "T_EQUAL";
  if (t == T_SEMICOLON) return "T_SEMICOLON";
  if (t == T_LPAREN) return "T_LPAREN";
  if (t == T_RPAREN) return "T_RPAREN";
  if (t == T_LCURLY) return "T_LCURLY";
  if (t == T_RCURLY) return "T_RCURLY";
  if (t == T_INTEGER) return "T_INTEGER";
  if (t == T_RETURN) return "T_RETURN";
  if (t == T_IDENTIFIER) return "T_IDENTIFIER";
  return "UNKNOWN";
}

void printToken(Token token, char *fileContents) {
  long len = token.filePosEnd - token.filePosStart;
  char buff[len + 1];
  printf("%s: %s\n", enumToString(token.type), tokenStringValue(buff, token, fileContents));
}

Token createToken(TType type, int filePosStart, int filePosEnd) {
  Token token;
  token.type = type;
  token.filePosStart = filePosStart;
  token.filePosEnd = filePosEnd;
  return token;
}

void newTokenArrayList(TokenArray *array) {
  int startingSize = 32;
  array->data = malloc(sizeof(Token) * startingSize);
  if (array->data == NULL) {
    fprintf(stdout, "Error: Malloc failure.\n");
    exit(1);
  }

  array->maxSize = startingSize;
  array->current = 0;
  array->len = 0;
}

void deleteTokenArrayList(TokenArray *array) { free(array->data); }

void insert(TokenArray *array, Token token) {
  if (array->len < array->maxSize) {
    array->data[array->len++] = token;
  } else {
    array->maxSize *= 2;
    array->data = realloc(array->data, array->maxSize * sizeof(Token));
    array->data[array->len++] = token;
    if (array->data == NULL) {
      fprintf(stdout, "Error: Realloc failure.\n");
      exit(1);
    }
  }
}

int getTokenIntValue(Token token, char *fileContents) {
  int len = token.filePosEnd - token.filePosStart;
  char buff[len + 1];
  memcpy(buff, fileContents + token.filePosStart, len);
  buff[len] = '\0';
  return strtol(buff, NULL, 10);
}

Token current(TokenArray *array, char *fileContents) {
  /* printf("at: "); */
  /* printToken(array->data[array->current], fileContents); */
  return get(array, array->current);
}

Token advance(TokenArray *array, char *fileContents) {
  /* printf("eat: "); */
  /* printToken(array->data[array->current], fileContents); */
  Token currToken = get(array, array->current);
  if (array->current < array->len) {
    array->current++;
  }
  return currToken;
}

Token get(TokenArray *array, int idx) {
  if (idx < array->len) {
    return array->data[idx];
  } else {
    return array->data[idx - 1];
    /* fprintf(stderr, "Panic: Array out of bounds!"); */
    /* abort(); */
  }
}

int isDigit(char c) {
  return c >= '0' && c <= '9';
}

int isAlphanumeric(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int tokenizeFile(TokenArray *array, char *file, long fileLen) {
  long line = 1;
  long idx = 0;
  int errors = 0;

  while (idx < fileLen) {
    if (file[idx] == ' ' || file[idx] == '\n' || file[idx] == '\t') {
      if (file[idx] == '\n') {
	line++;
      }
      idx++;
    } else if (file[idx] == '+') {
      insert(array, createToken(T_PLUS, idx, idx + 1));
      idx++;
    } else if (file[idx] == '-') {
      insert(array, createToken(T_MINUS, idx, idx + 1));
      idx++;
    } else if (file[idx] == '*') {
      insert(array, createToken(T_STAR, idx, idx + 1));
      idx++;
    } else if (file[idx] == '/') {
      insert(array, createToken(T_SLASH, idx, idx + 1));
      idx++;
    } else if (file[idx] == '=') {
      insert(array, createToken(T_EQUAL, idx, idx + 1));
      idx++;
    } else if (file[idx] == ';') {
      insert(array, createToken(T_SEMICOLON, idx, idx + 1));
      idx++;
    } else if (file[idx] == '(') {
      insert(array, createToken(T_LPAREN, idx, idx + 1));
      idx++;
    } else if (file[idx] == ')') {
      insert(array, createToken(T_RPAREN, idx, idx + 1));
      idx++;
    } else if (file[idx] == '{') {
      insert(array, createToken(T_LCURLY, idx, idx + 1));
      idx++;
    } else if (file[idx] == '}') {
      insert(array, createToken(T_RCURLY, idx, idx + 1));
      idx++;
    } else if (isDigit(file[idx]) == 1) {
      long start = idx;
      while (idx < fileLen && isDigit(file[idx]) == 1) {
	idx++;
      }
      insert(array, createToken(T_INTEGER, start, idx));
    } else if (isAlphanumeric(file[idx])) {
      long start = idx;
      while (idx < fileLen && (isAlphanumeric(file[idx]) || isDigit(file[idx]))) {
	idx++;
      }
      insert(array, createToken(T_IDENTIFIER, start, idx));
    } else {
      fprintf(stdout, "Lexer Error on line %ld: Unexpected character '%c'.\n", line, file[idx]);
      errors++;
      idx++; // TODO: Continue looking for errors? Set some kind of flag?
    }
  }

  return errors;
}