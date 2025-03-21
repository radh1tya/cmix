cc = cc
cflags = -g
ldflags = -lpulse -lncurses

source = main.c
target = cmix

install:
	$(cc) $(source) -o $(target) $(cflags) $(ldflags)

rm:
	rm -f $(target)
