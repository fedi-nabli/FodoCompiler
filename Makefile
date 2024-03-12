OBJECTS = ./build/compiler.o ./build/cprocess.o ./build/lexer.o ./build/lex_process.o ./build/token.o ./build/parser.o ./build/node.o ./build/expressionable.o ./build/datatype.o ./build/scope.o ./build/symresolver.o ./build/array.o ./build/fixup.o ./build/codegen.o ./build/stackframe.o ./build/resolver.o ./build/rdefault.o ./build/helper.o ./build/helpers/buffer.o ./build/helpers/vector.o
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

./build/node.o: ./node.c
	gcc ./node.c ${INCLUDES} -g -c -o ./build/node.o

./build/expressionable.o: ./expressionable.c
	gcc ./expressionable.c ${INCLUDES} -g -c -o ./build/expressionable.o

./build/datatype.o: ./datatype.c
	gcc ./datatype.c ${INCLUDES} -g -c -o ./build/datatype.o

./build/scope.o: ./scope.c
	gcc ./scope.c ${INCLUDES} -g -c -o ./build/scope.o

./build/symresolver.o: ./symresolver.c
	gcc ./symresolver.c ${INCLUDES} -g -c -o ./build/symresolver.o

./build/array.o: ./array.c
	gcc ./array.c ${INCLUDES} -g -c -o ./build/array.o

./build/fixup.o: ./fixup.h ./fixup.c
	gcc ./fixup.c ${INCLUDES} -g -c -o ./build/fixup.o

./build/codegen.o: ./codegen.c
	gcc ./codegen.c ${INCLUDES} -g -c -o ./build/codegen.o

./build/stackframe.o: ./stackframe.c
	gcc ./stackframe.c ${INCLUDES} -g -c -o ./build/stackframe.o

./build/resolver.o: ./resolver.c
	gcc ./resolver.c ${INCLUDES} -g -c -o ./build/resolver.o

./build/rdefault.o: ./rdefault.c
	gcc ./rdefault.c ${INCLUDES} -g -c -o ./build/rdefault.o

./build/helper.o: ./helper.c
	gcc ./helper.c ${INCLUDES} -g -c -o ./build/helper.o

./build/helpers/buffer.o: ./helpers/buffer.c
	gcc ./helpers/buffer.c -o ./build/helpers/buffer.o -g -c

./build/helpers/vector.o: ./helpers/vector.c
	gcc ./helpers/vector.c -o ./build/helpers/vector.o -g -c

clean:
	if [ -f ./main ] ; \
	then \
		rm ./main ; \
	fi;
	
	if [ -f ./test ] ; \
	then \
		rm ./test ; \
	fi;

	if [ -d ./main.dSYM ] ; \
	then \
		rm -rf ./main.dSYM ; \
	fi;

	rm -rf ${OBJECTS}