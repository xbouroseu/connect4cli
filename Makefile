all:
	gcc -o connect4 main.c

.PHONY: clean

clean:
	rm connect4
