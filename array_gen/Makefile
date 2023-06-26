CFLAGS = -Wall -Wextra

generate: generate_array.c
	rm -f generate_array
	gcc $(CFLAGS) -O3 -o generate_array generate_array.c ../helpers/helpers.c -lm
	./generate_array 1000 U ARR_U0001000.bin
	./generate_array 1000 N ARR_N0001000.bin
	./generate_array 1000 E ARR_E0001000.bin
	./generate_array 100000 U ARR_U0100000.bin
	./generate_array 100000 N ARR_N0100000.bin
	./generate_array 100000 E ARR_E0100000.bin
	./generate_array 1000000 U ARR_U1000000.bin
	./generate_array 1000000 N ARR_N1000000.bin
	./generate_array 1000000 E ARR_E1000000.bin

	mv -f ARR* ../input_data/
	rm -f generate_array

clean:
	rm -f generate_array ../input_data/ARR*