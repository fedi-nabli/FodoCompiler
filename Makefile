OBJECTS = ./build/compiler.o ./build/cprocess.o ./build/lexer.o ./build/lex_process.o ./build/token.o ./build/parser.o ./build/helpers/buffer.o ./build/helpers/vector.o
INCLUDES = -I./

all: ${OBJECTS}
	gcc main.c ${INCLUDES} ${OBJECTS} -g -o ./main

./build/compiler.o: ./compiler.c
	gcc ./compiler.c ${INCLUDES} -o ./build/compiler.o -g -c

./build/cprocess.o: ./cprocess.c
	gcc ./cprocess.c ${INCLUDES} -o ./build/cprocess.o -g -c

./build/lexer.o: ./lexer.c
	gcc ./lexer.c ${INCLUDES} -g -c -o ./build/lexer.o

./build/lex_process.o: ./lex_process.c
	gcc ./lex_process.c ${INCLUDES} -g -c -o ./build/lex_process.o

./build/token.o: ./token.c
	gcc ./token.c ${INCLUDES} -g -c -o ./build/token.o

./build/parser.o: ./parser.c
	gcc ./parser.c ${INCLUDES} -g -c -o ./build/parser.o

./build/helpers/buffer.o: ./helpers/buffer.c
	gcc ./helpers/buffer.c -o ./build/helpers/buffer.o -g -c

./build/helpers/vector.o: ./helpers/vector.c
	gcc ./helpers/vector.c -o ./build/helpers/vector.o -g -c

clean:
	if [ -f ./main ] ; \
	then \
		rm ./main ; \
	fi;
	
	rm -rf ${OBJECTS}