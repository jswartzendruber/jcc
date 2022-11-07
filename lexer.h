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
} TokenList;

void newTokenList(TokenList *list);
int tokenizeFile(TokenList *list, char *file, long fileLen);
Token getTokenList(TokenList *list, int idx);
void freeTokenList(TokenList *list);
void printToken(Token token, char *fileContents);
Token createToken(TType type, int filePosStart, int filePosEnd);
char *enumToString(TType t);
char *tokenStringValue(char *buff, Token token, char *fileContents);
void insertTokenList(TokenList *list, Token token);
int getTokenIntValue(Token token, char *fileContents);
Token advance(TokenList *list, char* fileContents);
Token current(TokenList *list, char* fileContents);

#endif
