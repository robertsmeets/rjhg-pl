.PHONY: clean
.PHONY: test

myparser.exe: myparser.lex.o myparser.y.o classes.o
	g++ -g -o myparser.exe myparser.lex.o myparser.y.o classes.o

y.tab.h: myparser.y
	byacc -d myparser.y

y.tab.c: myparser.y
	byacc -d myparser.y
	
myparser.lex.o: myparser.lex.c y.tab.h
	g++ -g -c myparser.lex.c -o myparser.lex.o

myparser.y.o: y.tab.c
	g++ -Wno-write-strings -std=c++11 -g -c y.tab.c -o myparser.y.o

myparser.lex.c: myparser.lex
	flex myparser.lex
	cat lex.yy.c | sed s/malloc\(strlen/\(char*\)malloc\(strlen/g >myparser.lex.c

classes.o: classes.h classes.cpp
	g++ -g -o classes.o -c classes.cpp

clean:
	rm -f myparser.lex.o
	rm -f myparser.y.o
	rm -f yywrap.o
	rm -f y.tab.h
	rm -f y.tab.c
	rm -f myparser.tab.h
	rm -f myparser.tab.c
	rm -f myparser.h
	rm -f myparser.y.c
	rm -f myparser.exe
	rm -f myparser.lex.c
	rm -f myparser.c
	rm -f classes.o
	rm -f y.output
	rm -f lex.yy.c

test: myparser.exe
	./myparser.exe < example.src	
