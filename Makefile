include Makefile.properties

all: build run

clean:
	rm main

build: main
	$(CC) $(CC_FLAGS) \
	main.c \
	src/lib/cerver.c \
	src/lib/logger.c \
	src/lib/util/util.c \
	src/lib/util/regex.c \
	src/lib/util/env.c \
	src/lib/util/explode.c \
	src/lib/thread/pool.c \
	src/lib/thread/worker.c \
	src/lib/thread/scheduler.c \
	src/lib/http/handler.c \
	src/lib/http/router.c \
	src/lib/structures/map/map.c \
	src/lib/structures/queue/queue.c \
	src/lib/http/response/codes.c \
	src/lib/http/response/response.c \
	src/lib/http/request/request.c \
	src/lib/http/request/parser/host.c \
	src/lib/http/request/parser/path.c \
	src/lib/http/request/parser/port.c \
	src/lib/http/request/parser/query.c \
	src/lib/http/request/parser/method.c \
	src/lib/http/request/parser/headers.c \
	src/lib/http/request/parser/version.c \
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
