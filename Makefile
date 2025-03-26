main: main.c
	gcc -std=c23 -Wall -ggdb3 -Wextra -g main.c -o main

run: main
	./main

grind: main
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./main

clean: main
	rm ./main

# vim: noet
