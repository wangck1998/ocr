g++			= g++.exe
edit		= bitmap.o
rm			= rm.exe
ext			= -g

.PHONY: all clean

all: main

clean: 
	$(rm) *.o *.exe

bitmap.o: bitmap.cpp bitmap.h
	$(g++) bitmap.cpp -c $(ext)

main: $(edit)
	$(g++) main.cpp $(edit) -o main.exe $(ext)
