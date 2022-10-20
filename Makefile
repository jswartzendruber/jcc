PROG := jcc
CC := gcc

OBJS = main.o

$(PROG): $(OBJS)
	$(CC) -fsanitize=address $(OBJS) -o $(PROG)

main.o : lexer.h

.PHONY : clean
clean :
	rm $(PROG) $(OBJS)
