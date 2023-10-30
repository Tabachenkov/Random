CC = gcc
LD = gcc
CFLAGS = -g -c -O2 -std=gnu2x -Wall -Werror -Wno-pointer-sign -Werror=vla
LDFLAGS = -lm
.PHONY: all, clean
all: printrandom
printrandom: random.o linear.o main.o
	$(LD) $(LDFLAGS) -rdynamic -fPIC $^ -o $@
random.o: random.c
	$(CC) $(CFLAGS) -fPIC -DPIC $< -o $@
linear.o: linear.c
	$(CC) $(CFLAGS) -fPIC -DPIC $< -o $@
main.o: main.c
	$(CC) $(CFLAGS) -fPIC -DPIC $< -o $@
clean:
	rm -rf random.o linear.o main.o printrandom
