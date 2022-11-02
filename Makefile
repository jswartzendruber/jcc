PROG := jcc
CC := gcc
WARNINGS := -fsanitize=address -Wall -Wextra -pedantic -Wshadow

OBJS = main.o lexer.o parser.o

$(PROG): $(OBJS)
	$(CC) $(WARNINGS) $(OBJS) -o $(PROG)

main.o : lexer.h parser.h

test : $(PROG)
	./runTests.py

.PHONY : clean
clean :
	rm $(PROG) $(OBJS)
