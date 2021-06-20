CFLAGS = -O3 -g
INCLUDE = -I libcli-1.10.7/include
LIB = -L libcli-1.10.7/lib/.libs/

all: 
	gcc ${CFLAGS} ${INCLUDE} ${LIB} example.c -o example -lcli
	

	
