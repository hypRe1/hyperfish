all:
	gcc -oFast hyperfish.c -o hyperfish
	clear
	./hyperfish
debug:
	gcc hyperfish.c -o hyperfish