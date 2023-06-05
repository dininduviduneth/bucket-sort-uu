CFLAGS = -Wall -Wextra

generate: bucket.c
	rm -f bucket
	gcc $(CFLAGS) -O3 -o bucket bucket.c ../helpers/helpers.c

clean:
	rm -f bucket result.bin