include Makefile.properties


all: main

clean:
	rm main

main: main.c
	$(CC) $(CC_FLAGS) main.c lib/cerver/cerver.c -o main

run:
	./main

