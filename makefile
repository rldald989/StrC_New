
all: main

main: main.o tokenizer.o lexer.o
	g++ -std=c++20 bin/main.o bin/tokenizer.o bin/lexer.o -o bin/main

main.o: src/main.cpp
	g++ -c -std=c++20 src/main.cpp -o bin/main.o

tokenizer.o: classes/Tokenizer/Tokenizer.cpp
	g++ -c -std=c++20 classes/Tokenizer/Tokenizer.cpp -o bin/tokenizer.o

lexer.o: classes/Lexer/Lexer.cpp
	g++ -c -std=c++20 classes/Lexer/Lexer.cpp -o bin/lexer.o

.PHONY clean:
	del bin\*.exe
	del bin\*.o