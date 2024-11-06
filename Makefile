INCLUDE_FOLDER=include/

data.bin: binutil pontos.txt vizinhos.txt
	./$< pontos.txt vizinhos.txt

binutil: input.c binutil.c
	gcc $^ -I ${INCLUDE_FOLDER} -lm -o $@

clean:
	find . -type f -executable -exec rm '{}' \;
	find . -type f -iname "*.bin" -exec rm '{}' \;