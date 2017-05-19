NAME = boxes
EXEC = ./$(NAME).exe
OBJS = global.o main.o $(NAME).tab.o lex.yy.o box.o item.o \
	string.o html.o text.o error.o

CFLAGS = -O
LIBS = -ll

BROWSER = google-chrome --new-window

TEST1 = test1.boxes
TEST3 = test3.boxes
TEST5 = test5.boxes

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

test: $(EXEC) $(TEST1) $(TEST2) $(TEST3) $(TEST4) $(TEST5)
	$(EXEC) -h $(TEST1) >test1.html && $(BROWSER) test1.html
	$(EXEC) -h $(TEST3) >test3.html && $(BROWSER) test3.html
	#$(EXEC) -h $(TEST1) $(TEST3) >test.html && $(BROWSER) test.html
	$(EXEC) -h $(TEST5) >test5.html && $(BROWSER) test5.html

cleanup:
	@rm -f *.exe *.o *.html $(NAME).tab.c lex.yy.c $(NAME).tab.h x.tab.h
