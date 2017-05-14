NAME = boxes
EXEC = ./$(NAME).exe
OBJS = global.o main.o $(NAME).tab.o lex.yy.o box.o item.o title.o string.o html.o text.o error.o

CFLAGS = -O
LIBS = -ll

TEST1 = test1.boxes
TEST2 = test2.boxes

$(EXEC): $(OBJS)
	cc -o $(EXEC) $(OBJS) $(LIBS)

$(NAME).tab.c: $(NAME).y
	bison -d $(NAME).y

lex.yy.o: lex.yy.c x.tab.h

lex.yy.c: $(NAME).l
	flex $(NAME).l

x.tab.h: $(NAME).tab.h
	@-cmp -s x.tab.h $(NAME).tab.h || cp $(NAME).tab.h x.tab.h

$(NAME).tab.h: $(NAME).y

test: $(EXEC) $(TEST1) $(TEST2)
	 # $(EXEC) -h $(TEST1) >test1.html && google-chrome --new-window test1.html
	 $(EXEC) -h $(TEST2) >test2.html && google-chrome --new-window test2.html

cleanup:
	@rm -f *.exe *.o *.html $(NAME).tab.c lex.yy.c $(NAME).tab.h x.tab.h
