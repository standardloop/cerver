include Makefile.properties


all: compile run

clean:
	rm main

compile: main.c
	$(CC) $(CC_FLAGS) main.c lib/cerver/cerver.c lib/cerver/spec/http/request/request.c -o main

run:
	./main
