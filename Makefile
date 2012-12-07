CFLAGS=-g -std=c99
DBMS:	sql.tab.o lex.yy.o parser.o print.o Catalog.o Block.o Node.o Bucket.o Header.o SManager.o BManager.o Quary.o MBlock.o Index.o Main.o
	$(CXX) $(CXXFLAGS) $^ -o dbms

sql.tab.c sql.tab.h: sql.y
	bison --defines sql.y
lex.yy.c: sql.lex
	flex --header-file=sql.lex.h sql.lex
lex.yy.o: lex.yy.c sql.tab.h
	$(CC) $(CFLAGS) -D_POSIX_SOURCE -c -o lex.yy.o lex.yy.c
sql.tab.o: sql.tab.c sql.tab.h
parser.o: parser.c parser.h
print.o: print.c print.h
Catalog.o: Catalog.cpp Catalog.h
Block.o: Block.cpp Block.h
Node.o: Node.cpp Node.h
Bucket.o: Bucket.cpp Bucket.h
Header.o: Header.cpp Header.h
SManager.o: SManager.cpp SManager.h
BManager.o: BManager.cpp BManager.h
Quary.o: Quary.cpp Quary.h
MBlock.o: MBlock.cpp MBlock.h
Index.o: Index.cpp Index.h
Main.o: Main.cpp
