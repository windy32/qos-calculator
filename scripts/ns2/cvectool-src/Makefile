cvectool: cvectool.o parser.o
	g++ -o cvectool cvectool.o parser.o

cvectool.o: epoch.h cvector.h cvectool.cpp parser.h
	g++ -c cvectool.cpp

parser.o: parser.h parser.cpp
	g++ -c parser.cpp

clean:
	rm cvectool *.o
	
