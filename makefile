CFLAGS = -O3 -Wall
INCLUDE = -I libcli-1.10.7/include
LIB = -L libcli-1.10.7/lib/.libs/

all: example.o
	gcc ${CFLAGS} ${INCLUDE} ${LIB} example.c -o exampl -lcli
	

	
