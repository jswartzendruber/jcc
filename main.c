#include "lexer.h"
#include "parser.h"
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

void lexFile(char *path) {
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

  TokenArray tokens;
  newTokenArrayList(&tokens);

  int errors = tokenizeFile(&tokens, contents, fileSize);
  if (errors > 0) {
    exit(1);
  }

  // Display tokens
  /* for (int i = 0; i < array.len; i++) { */
  /*   printToken(tokens.data[i], contents); */
  /* } */

  ExprTree *tree = parseFile(&tokens);
  //printExprTree(tree, 0);
  freeExprTree(tree);

  deleteTokenArrayList(&tokens);
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
	// save outfile
        i++;
      } else {
        fprintf(stderr, "Error: -o must be followed by a file name.\n");
        return 1;
      }
    } else {
      lexFile(argv[i]);
    }
  }

  return 0;
}
