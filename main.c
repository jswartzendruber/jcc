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

void lex(char* path) {
  printf("lex: %s\n", path);

  FILE *file = fopen(path, "rb");
  if (file != NULL) {
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *contents = malloc(fileSize + 1);
    if (contents == NULL) {
      fprintf(stderr, "Error: Malloc failed when lexing file '%s'.\n", path);
      exit(1);
    }
    fread(contents, fileSize, 1, file);
    contents[fileSize] = '\0';

    fclose(file);

    printf("%s\n", contents);

    free(contents);
  } else {
    fprintf(stdout, "Error: File '%s' not found.\n", path);
  }
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
