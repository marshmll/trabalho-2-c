SRC_FOLDER=src
INCLUDE_FOLDER=include
BUILD_FOLDER=build

CFLAGS=-Wall -g

all: main data.bin
	${BUILD_FOLDER}/main data.bin

main: ${SRC_FOLDER}/main.c ${SRC_FOLDER}/graph.c ${SRC_FOLDER}/binutils.c
	gcc $^ ${CFLAGS} -I ${INCLUDE_FOLDER} -lm -o ${BUILD_FOLDER}/$@

data.bin: bingen pontos.txt vizinhos.txt
	./${BUILD_FOLDER}/$< pontos.txt vizinhos.txt

bingen: ${SRC_FOLDER}/input.c ${SRC_FOLDER}/graph.c ${SRC_FOLDER}/bingen.c
	gcc $^ ${CFLAGS} -I ${INCLUDE_FOLDER} -lm -o ${BUILD_FOLDER}/$@

clean:
	find . -type f -executable -exec rm '{}' \;
	find . -type f -iname "*.bin" -exec rm '{}' \;