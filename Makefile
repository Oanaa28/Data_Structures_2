build: main.c
	gcc -Wall -g -o tema3 main.c graf1.c graf2.c
run:
	./tema3
clean:
	rm tema3
valgrind:
	valgrind --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tema3