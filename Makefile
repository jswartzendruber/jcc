.PHONY : clean
.PHONY : tests
.PHONY : jcc
.PHONY : run

WARNINGS := -fsanitize=address -Wall -Wextra -pedantic -Wshadow
OBJS = main.o lexer.o parser.o
PROG = jcc

$(PROG): $(OBJS)
	gcc $(WARNINGS) $(OBJS) -o $(PROG)

main.o : lexer.h parser.h

run : $(PROG)
	./$(PROG) examples/return.c

tests : $(PROG)
	./runTests.py

clean :
	rm $(PROG) $(OBJS)

all: $(PROG) tests
