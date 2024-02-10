OBJECTS = ./build/helpers/buffer.o ./build/helpers/vector.o
INCLUDES = -I./

all: ${OBJECTS}
	gcc main.c ${INCLUDES} ${OBJECTS} -g -o ./main

./build/helpers/buffer.o: ./helpers/buffer.c
	gcc ./helpers/buffer.c -o ./build/helpers/buffer.o -g -c

./build/helpers/vector.o: ./helpers/vector.c
	gcc ./helpers/vector.c -o ./build/helpers/vector.o -g -c

clean:
	rm ./main
	rm -rf ${OBJECTS}