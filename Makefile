SRC_FOLDER=src
INCLUDE_FOLDER=include
BIN_FOLDER=bin

CFLAGS=-Wall -g

all: bingen data.bin main
	${BIN_FOLDER}/main ${BIN_FOLDER}/data.bin

main: ${SRC_FOLDER}/main.c ${SRC_FOLDER}/graph.c ${SRC_FOLDER}/binparser.c ${SRC_FOLDER}/input.c
	gcc $^ ${CFLAGS} -I ${INCLUDE_FOLDER} -lm -o ${BIN_FOLDER}/$@

data.bin: bingen pontos.txt vizinhos.txt
	./${BIN_FOLDER}/$< pontos.txt vizinhos.txt ${BIN_FOLDER}/$@

bingen: ${SRC_FOLDER}/graph.c ${SRC_FOLDER}/input.c ${SRC_FOLDER}/bingen.c
	mkdir -p bin
	gcc $^ ${CFLAGS} -I ${INCLUDE_FOLDER} -lm -o ${BIN_FOLDER}/$@

clean:
	find . -type f -executable -exec rm '{}' \;
	find . -type f -iname "*.bin" -exec rm '{}' \;