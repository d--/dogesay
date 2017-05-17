.PHONY: all install clean

CFLAGS = -Wall

all: bin/dogesay

build:
	mkdir -p build

bin:
	mkdir -p bin

install: all
	cp bin/dogesay /usr/local/bin/dogesay

bin/dogesay: src/dogesay.c build/doge_say_t.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ \
		$(shell pkg-config --cflags --libs lcm)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ \
		$(shell pkg-config --cflags lcm)

clean:
	$(RM) -r build bin

