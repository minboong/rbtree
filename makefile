all: compile

compile: rbtree.c
	gcc rbtree.c -o assignment2

run: assignment2
	./assignment2 input.txt output.txt

clean: assignment2
	rm assignment2
