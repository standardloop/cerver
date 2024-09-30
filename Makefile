include Makefile.properties

all: build

clean:
	@rm -f $(EXECUTABLE_NAME)
	@rm -f $(EXECUTABLE_NAME)-debug
	@rm -f $(EXECUTABLE_NAME)-optimize
	@rm -f a.out
	@rm -f $(DYLIB_NAME)

build:
	@$(CC) $(CC_FLAGS) \
	main.c \
	cerver.c \
	thread/pool.c \
	thread/worker.c \
	thread/scheduler.c \
	http/handler.c \
	http/router.c \
	structures/queue/queue.c \
	http/response/codes.c \
	http/response/response.c \
	http/request/request.c \
	http/request/parser/body.c \
	http/request/parser/host.c \
	http/request/parser/path.c \
	http/request/parser/port.c \
	http/request/parser/query.c \
	http/request/parser/method.c \
	http/request/parser/headers.c \
	http/request/parser/version.c \
	$(DYN_LIBS_USED_PATH) \
	$(DYN_LIBS_USED) \
	-o $(EXECUTABLE_NAME)


# 	-fsanitize=address
#	-fno-omit-frame-pointer

test:

test.no_headers:
	curl localhost:8080/ -H 'User-Agent:' -H 'Accept:' -H 'Host:'

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
