CFLAGS = -Wall -Wextra

generate: generate_array.c
	rm -f generate_array
	gcc $(CFLAGS) -O3 -o generate_array generate_array.c ../helpers/helpers.c -lm -Wno-unknown-pragmas
	./generate_array 1000 U ARR_U00001000.bin
	./generate_array 1000 N ARR_N00001000.bin
	./generate_array 1000 E ARR_E00001000.bin
	# ./generate_array 100000 U ARR_U00100000.bin
	# ./generate_array 100000 N ARR_N00100000.bin
	# ./generate_array 100000 E ARR_E00100000.bin
	# ./generate_array 1000000 U ARR_U01000000.bin
	# ./generate_array 1000000 N ARR_N01000000.bin
	# ./generate_array 1000000 E ARR_E01000000.bin
	# ./generate_array 10000000 U ARR_U10000000.bin
	# ./generate_array 10000000 N ARR_N10000000.bin
	# ./generate_array 10000000 E ARR_E10000000.bin
	# ./generate_array 50000000 U ARR_U50000000.bin
	# ./generate_array 50000000 N ARR_N50000000.bin
	# ./generate_array 50000000 E ARR_E50000000.bin

	# ./generate_array 10000 U ARR_U00010000.bin
	# ./generate_array 100000 U ARR_U00100000.bin
	# ./generate_array 1000000 U ARR_U01000000.bin
	# ./generate_array 10000000 U ARR_U10000000.bin
	# ./generate_array 20000000 U ARR_U20000000.bin
	# ./generate_array 30000000 U ARR_U30000000.bin
	# ./generate_array 40000000 U ARR_U40000000.bin

	mv -f ARR* ../input_data/
	rm -f generate_array

clean:
	rm -f generate_array ../input_data/ARR*