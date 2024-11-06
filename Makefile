SRC_FOLDER=src
INCLUDE_FOLDER=include
BUILD_FOLDER=build

data.bin: binutil pontos.txt vizinhos.txt
	./${BUILD_FOLDER}/$< pontos.txt vizinhos.txt

binutil: ${SRC_FOLDER}/input.c ${SRC_FOLDER}/binutil.c
	gcc $^ -I ${INCLUDE_FOLDER} -lm -o ${BUILD_FOLDER}/$@

clean:
	find . -type f -executable -exec rm '{}' \;
	find . -type f -iname "*.bin" -exec rm '{}' \;