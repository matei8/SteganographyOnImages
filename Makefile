build:
	gcc -g main.c comandsFunctions.c codeDecode.c -o steg

run:
	./steg

clean:
	rm steg