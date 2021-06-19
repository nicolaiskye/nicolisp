.PHONY: build

build:
	cc -std=c99 -Wall src/main.c mpc/mpc.c -ledit -lm -o nicolisp

clean:
	rm nicolisp
