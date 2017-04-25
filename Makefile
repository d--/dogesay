.PHONY: all clean

CFLAGS = -Wall

all: bin/pub bin/sub

build:
	mkdir -p build

bin:
	mkdir -p bin

bin/pub: src/pub.c build/doge_say_t.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ \
		$(shell pkg-config --cflags --libs lcm)

bin/sub: src/sub.c build/doge_say_t.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ \
		$(shell pkg-config --cflags --libs lcm)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ \
		$(shell pkg-config --cflags lcm)

clean:
	$(RM) -r build bin

