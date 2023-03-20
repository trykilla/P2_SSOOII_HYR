DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

#g++ archivo.cpp -o archivo -pthread -std=c++11

CFLAGS := -I$(DIRHEA) -c -Wall -ansi -std=c++11
LDLIBS := -lpthread -lrt -pthread 
CC := g++

all : dirs manager #pa, pb

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(CC) $(CFLAGS) $^ -o $@

test:
	./$(DIREXE)manager 3 2 5

solution:
	./$(DIREXE)manager 2 3 4

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~
