CFLAGS = -Wall -Wextra

generate: generate_array.c
	rm -f generate_array
	gcc $(CFLAGS) -O3 -o generate_array generate_array.c ../helpers/helpers.c -lm
	./generate_array 1000 U ARR_U00001000.bin
	./generate_array 1000 N ARR_N00001000.bin
	./generate_array 1000 E ARR_E00001000.bin
	./generate_array 100000 U ARR_U00100000.bin
	./generate_array 100000 N ARR_N00100000.bin
	./generate_array 100000 E ARR_E00100000.bin
	./generate_array 50000000 U ARR_U50000000.bin

	mv -f ARR* ../input_data/
	rm -f generate_array

clean:
	rm -f generate_array ../input_data/ARR*