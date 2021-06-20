LIB = libcli.so
LIB_STATIC = libcli.a

CC = gcc
AR = ar
ARFLAGS = rcs
DEBUG = -g
OPTIM = -O3
override CFLAGS += $(DEBUG) $(OPTIM) -Wall -std=c99 -pedantic -Wformat-security -Wno-format-zero-length -Werror -Wwrite-strings -Wformat -fdiagnostics-show-option -Wextra -Wsign-compare -Wcast-align -Wno-unused-parameter
override LDFLAGS += -shared
override LIBPATH += -L.

ifeq ($(UNAME),Darwin)
override LDFLAGS += -Wl,-install_name,$(LIB).$(MAJOR).$(MINOR)
else
override LDFLAGS += -Wl,-soname,$(LIB).$(MAJOR).$(MINOR)
LIBS = -lcrypt
endif

ifeq (1,$(DYNAMIC_LIB))
TARGET_LIBS += $(LIB)
endif
ifeq (1,$(STATIC_LIB))
TARGET_LIBS += $(LIB_STATIC)
endif
all: $(TARGET_LIBS) $(if $(filter 1,$(TESTS)),clitest) example.o

$(LIB): libcli.o
	$(CC) -o $(LIB).$(MAJOR).$(MINOR).$(REVISION) $^ $(LDFLAGS) $(LIBS)
	-rm -f $(LIB) $(LIB).$(MAJOR).$(MINOR)
	ln -s $(LIB).$(MAJOR).$(MINOR).$(REVISION) $(LIB).$(MAJOR).$(MINOR)
	ln -s $(LIB).$(MAJOR).$(MINOR) $(LIB)

$(LIB_STATIC): libcli.o
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -o $@ -c $<

clitest: clitest.o $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -L. -lcli

example:
	$(CC) $(CPPFLAGS) $(CFLAGS) example.c -o exampl $@ $< -L. -lcli
	

	
