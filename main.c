#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
  printf("jcc <OPTIONS>\n");
  printf("    -o <FILENAME>\n");
  printf("        Compiles code to executable named <FILENAME>\n");
  printf("    -h\n");
  printf("        Shows this menu\n");
}

int isDigit(char c) {
  return c >= '0' && c <= '9';
}

int isAlphanumeric(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void tokenizeFile(TokenArray *array, char *file, long fileLen) {
  long lineNumber = 0;
  long idx = 0;

  while (idx < fileLen) {
    if (file[idx] == ' ' || file[idx] == '\n' || file[idx] == '\t') {
      if (file[idx] == '\n') {
	lineNumber++;
      }
      idx++;
    } else if (file[idx] == '+') {
      insert(array, createToken(T_PLUS, idx, idx));
      idx++;
    } else if (file[idx] == '-') {
      insert(array, createToken(T_MINUS, idx, idx));
      idx++;
    } else if (file[idx] == '*') {
      insert(array, createToken(T_STAR, idx, idx));
      idx++;
    } else if (file[idx] == '/') {
      insert(array, createToken(T_SLASH, idx, idx));
      idx++;
    } else if (file[idx] == '%') {
      insert(array, createToken(T_PERCENT, idx, idx));
      idx++;
    } else if (file[idx] == '\\') {
      insert(array, createToken(T_BACKSLASH, idx, idx));
      idx++;
    } else if (file[idx] == '<') {
      if (idx + 1 < fileLen && file[idx + 1] == '=') {
	insert(array, createToken(T_LESSEQUALTHAN, idx, idx + 1));
	idx += 2;
      } else {
	insert(array, createToken(T_LESSTHAN, idx, idx));
	idx++;
      }
    } else if (file[idx] == '>') {
      if (idx + 1 < fileLen && file[idx + 1] == '=') {
	insert(array, createToken(T_GREATEREQUALTHAN, idx, idx + 1));
	idx += 2;
      } else {
	insert(array, createToken(T_GREATERTHAN, idx, idx));
	idx++;
      }
    } else if (file[idx] == '=') {
      if (idx + 1 < fileLen && file[idx + 1] == '=') {
	insert(array, createToken(T_EQUALEQUAL, idx, idx + 1));
	idx += 2;
      } else {
	insert(array, createToken(T_EQUAL, idx, idx));
	idx++;
      }
    } else if (file[idx] == '!') {
      if (idx + 1 < fileLen && file[idx + 1] == '=') {
	insert(array, createToken(T_BANGEQUAL, idx, idx + 1));
	idx += 2;
      } else {
	insert(array, createToken(T_BANG, idx, idx));
	idx++;
      }
    } else if (file[idx] == '&') {
      if (idx + 1 < fileLen && file[idx + 1] == '&') {
	insert(array, createToken(T_AMPERAMPER, idx, idx + 1));
	idx += 2;
      } else {
	fprintf(stderr, "Lexer Error on line %i: Unexpected character '%c'.\n", file[idx]);
	idx++; // TODO: Continue looking for errors? Set some kind of flag?
      }
    } else if (file[idx] == '|') {
      if (idx + 1 < fileLen && file[idx + 1] == '|') {
	insert(array, createToken(T_PIPEPIPE, idx, idx + 1));
	idx += 2;
      } else {
	fprintf(stderr, "Lexer Error on line %i: Unexpected character '%c'.\n", file[idx]);
	idx++; // TODO: Continue looking for errors? Set some kind of flag?
      }
    } else if (file[idx] == ';') {
      insert(array, createToken(T_SEMICOLON, idx, idx));
      idx++;
    } else if (file[idx] == ',') {
      insert(array, createToken(T_COMMA, idx, idx));
      idx++;
    } else if (file[idx] == '.') {
      insert(array, createToken(T_DOT, idx, idx));
      idx++;
    } else if (file[idx] == '[') {
      insert(array, createToken(T_LBRACKET, idx, idx));
      idx++;
    } else if (file[idx] == ']') {
      insert(array, createToken(T_RBRACKET, idx, idx));
      idx++;
    } else if (file[idx] == '(') {
      insert(array, createToken(T_LPAREN, idx, idx));
      idx++;
    } else if (file[idx] == ')') {
      insert(array, createToken(T_RPAREN, idx, idx));
      idx++;
    } else if (file[idx] == '{') {
      insert(array, createToken(T_LCURLY, idx, idx));
      idx++;
    } else if (file[idx] == '}') {
      insert(array, createToken(T_RCURLY, idx, idx));
      idx++;
    } else if (isDigit(file[idx]) == 1) {
      long start = idx;
      int floating = 0;
      while (idx < fileLen && (isDigit(file[idx]) == 1 || file[idx] == '.')) {
	if (file[idx] == '.') {
	  if (floating == 0) {
	    floating = 1;
	  } else {
	    fprintf(stderr, "Lexer Error on line %i: Multiple periods found in number.\n", file[idx]);
	    idx++; // TODO: Continue looking for errors? Set some kind of flag?
	  }
	} else {
	  idx++;
	}
      }
      if (floating) {
	insert(array, createToken(T_FLOAT, start, idx - 1));
      } else {
	insert(array, createToken(T_INTEGER, start, idx - 1));
      }
    } else if (file[idx] == '"') {
      long start = idx;
      idx++;
      while (idx < fileLen && file[idx] != '"') {
	idx++;
      }
      insert(array, createToken(T_STRING, start, idx));
      idx++;
    } else if (isAlphanumeric(file[idx])) {
      long start = idx;
      while (idx < fileLen && (isAlphanumeric(file[idx]) || file[idx] == '_')) {
	idx++;
      }
      insert(array, createToken(T_IDENTIFIER, start, idx - 1));
    } else {
      fprintf(stderr, "Lexer Error on line %i: Unexpected character '%c'.\n", file[idx]);
      idx++; // TODO: Continue looking for errors? Set some kind of flag?
    }
  }
}

void lex(char *path) {
  printf("lex: %s\n", path);

  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stdout, "Error: File '%s' not found.\n", path);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *contents = malloc(fileSize + 1);
  if (contents == NULL) {
    fprintf(stderr, "Error: Malloc failure.\n");
    exit(1);
  }

  fread(contents, fileSize, 1, file);
  contents[fileSize] = '\0';
  fclose(file);

  TokenArray array;
  newTokenArrayList(&array);

  tokenizeFile(&array, contents, fileSize);

  for (int i = 0; i < array.len; i++) {
    printf("%s\n", enumToString(get(&array, i).type));
  }

  deleteTokenArrayList(&array);
  free(contents);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    print_usage();
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp("-h", argv[i]) == 0) {
      print_usage();
      return 0;
    } else if (strcmp("-o", argv[i]) == 0) {
      if (i + 1 < argc) {
        printf("outfile: %s\n", argv[i + 1]);
        i++;
      } else {
        fprintf(stderr, "Error: -o must be followed by a file name.\n");
        return 1;
      }
    } else {
      lex(argv[i]);
    }
  }

  return 0;
}
