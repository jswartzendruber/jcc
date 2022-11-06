#ifndef LEXER_H
#define LEXER_H

typedef enum {
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_EQUAL,
  T_SEMICOLON,
  T_LPAREN,
  T_RPAREN,
  T_RCURLY,
  T_LCURLY,
  T_INTEGER,
  T_RETURN,
  T_IDENTIFIER,
  K_INT,
  K_RETURN,
} TType;

typedef struct {
  TType type;
  int filePosStart;
  int filePosEnd;
} Token;

typedef struct {
  Token *data;
  int maxSize;
  int current;
  int len;
} TokenArray;

void newTokenArrayList(TokenArray *array);
int tokenizeFile(TokenArray *array, char *file, long fileLen);
Token get(TokenArray *array, int idx);
void deleteTokenArrayList(TokenArray *array);
void printToken(Token token, char *fileContents);
Token createToken(TType type, int filePosStart, int filePosEnd);
char *enumToString(TType t);
char *tokenStringValue(char *buff, Token token, char *fileContents);
void insert(TokenArray *array, Token token);
int getTokenIntValue(Token token, char *fileContents);
Token advance(TokenArray *array, char* fileContents);
Token current(TokenArray *array, char* fileContents);

#endif
