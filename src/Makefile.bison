Parser: Parser.lex.o Parser.y.o yywrap.o
	gcc -g -o Parser Parser.lex.o Parser.y.o yywrap.o

Parser.h: Parser.y
	bison -d Parser.y
	mv Parser.tab.h Parser.h

Parser.y.c: Parser.y
	bison -d Parser.y
	mv Parser.tab.c Parser.y.c
	
Parser.lex.o: Parser.lex.c Parser.h
	gcc -g -c Parser.lex.c -o Parser.lex.o

Parser.y.o: Parser.y.c
	gcc -g -c Parser.y.c -o Parser.y.o

yywrap.o: yywrap.c
	gcc -g -o yywrap.o -c yywrap.c

Parser.lex.c: Parser.lex
	flex Parser.lex
	mv lex.yy.c Parser.lex.c

clean:
	rm -f Parser.lex.o
	rm -f Parser.y.o
	rm -f yywrap.o
	rm -f Parser.tab.h
	rm -f Parser.tab.c
	rm -f Parser.h
	rm -f Parser.y.c
	rm -f Parser.exe
	rm -f Parser.lex.c
