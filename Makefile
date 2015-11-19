CC = icc

CC_DEBUG = @$(CC) -g
CC_RELEASE = @$(CC) -O3

all:
	$(CC_RELEASE) quicksort-seq.c -o seq -lm
	$(CC_RELEASE) quicksort-par.cpp -o par -lm

debug:
	$(CC_DEBUG) quicksort-seq.c -o seq -lm
	$(CC_DEBUG) quicksort-par.cpp -o par -lm

clean:
	@rm -r seq par
