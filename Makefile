CC = gcc -fopenmp

CC_DEBUG = @$(CC) -g
CC_RELEASE = @$(CC) -O3


all:
	$(CC_RELEASE) quicksort-seq.c -o quicksort-seq

debug:
	$(CC_DEBUG) quicksort-seq.c -o quicksort-seq

clean:
	@rm -r quicksort-seq
