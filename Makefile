include Makefile.properties


all: build run

clean:
	rm main

build: main.c
	$(CC) $(CC_FLAGS) main.c \
	lib/cerver/cerver.c \
	lib/cerver/util/util.c \
	lib/cerver/http/request/methods/methods.c \
	lib/cerver/http/request/version/version.c \
	lib/cerver/http/request/host/host.c \
	lib/cerver/http/request/port/port.c \
	lib/cerver/http/request/request.c \
	lib/cerver/thread/scheduler.c \
	lib/cerver/thread/pool.c \
	lib/cerver/thread/queue.c \
	lib/cerver/thread/worker.c \
	-o main

run:
	./main

test:

test.simple:
	curl localhost:8080/ -I

test.post:
	curl -X POST -H 'Accept: application/json' \
    -H 'Content-type: application/json' http://localhost:8080/ \
    --data '{"title":"dart", "artist":"foo"}'

test.post2:
	curl --http2 -X POST -H 'Accept: application/json' \
    -H 'Content-type: application/json' http://localhost:8080/ \
    --data '{"title":"dart", "artist":"foo"}'

# curl -X DELETE -H 'Accept: application/json' \
#     -H 'Content-type: application/json' http://localhost:3000/songs/1

# curl -X PUT -H 'Accept: application/json' \
#     -H 'Content-type: application/json' http://localhost:3000/songs/2/ \
#     --data '{"title":"dart", "artist":"bar"}'

# curl -X GET -H 'Accept: application/json' -H 'Content-type: application/json' http://localhost:3000/api/v1/songs/