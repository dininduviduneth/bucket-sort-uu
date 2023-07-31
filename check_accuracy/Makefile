CFLAGS = -Wall -Wextra

check_accuracy: check_accuracy.c
	rm -f check_accuracy
	gcc $(CFLAGS) -O3 -o check_accuracy check_accuracy.c ../helpers/helpers.c -lm -Wno-unknown-pragmas

clean:
	rm -f check_accuracy