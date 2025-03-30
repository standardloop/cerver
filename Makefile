include Makefile.properties

all: build

clean:
	@rm -f $(EXECUTABLE_NAME)
	@rm -f $(EXECUTABLE_NAME)-debug
	@rm -f $(EXECUTABLE_NAME)-optimize
	@rm -f $(EXECUTABLE_NAME)-sanitize
	@rm -f a.out
	@rm -f $(DYLIB_NAME)

build:
	@$(CC) $(CC_FLAGS) \
	main.c \
	$(SOURCE_FILES) \
	$(DYN_LIBS_USED_PATH) \
	$(DYN_LIBS_USED) \
	-o $(EXECUTABLE_NAME)


build_sanitize:
	@$(CC) $(CC_FLAGS) \
	main.c \
	-fsanitize=address \
	-fno-omit-frame-pointer \
	$(SOURCE_FILES) \
	$(DYN_LIBS_USED_PATH) \
	$(DYN_LIBS_USED) \
	-o $(EXECUTABLE_NAME)-sanitize

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
