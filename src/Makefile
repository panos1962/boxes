NAME = boxes
EXEC = ./$(NAME).exe
OBJS = global.o main.o $(NAME).tab.o lex.yy.o box.o item.o title.o \
	contents.o string.o html.o text.o error.o

CFLAGS = -O
LIBS = -ll

TEST1 = test1.boxes
TEST2 = test2.boxes
TEST3 = test3.boxes
TEST4 = test4.boxes

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

test: $(EXEC) $(TEST1) $(TEST2) $(TEST3)
	#$(EXEC) -h $(TEST1) >test1.html && google-chrome --new-window test1.html
	#$(EXEC) -h $(TEST2) >test2.html && google-chrome --new-window test2.html
	#$(EXEC) -h $(TEST3) >test3.html && google-chrome --new-window test3.html
	#$(EXEC) -h $(TEST1) $(TEST3) >test.html && google-chrome --new-window test.html
	$(EXEC) -h $(TEST4) >test4.html && google-chrome --new-window test4.html

cleanup:
	@rm -f *.exe *.o *.html $(NAME).tab.c lex.yy.c $(NAME).tab.h x.tab.h
