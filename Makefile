CC = gcc -fopenmp

CC_DEBUG = @$(CC) -g
CC_RELEASE = @$(CC) -O3


all:
	$(CC_RELEASE) quicksort-seq.c -o quicksort-seq -lm
	$(CC_RELEASE) quicksort-par.c -o quicksort-par -lm

debug:
	$(CC_DEBUG) quicksort-seq.c -o quicksort-seq -lm
	$(CC_DEBUG) quicksort-par.c -o quicksort-par -lm

clean:
	@rm -r quicksort-seq quicksort-par
