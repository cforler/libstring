CFLAGS += -W -Wall -Wextra -Werror -L. -finline-small-functions
CFLAGS += -Os -fstack-protector-all -fPIC -s -D_FORTIFY_SOURCE=2 -z now
#CFLAGS += -O1 -ggdb3 -fsanitize=address

INCLUDE_DIR = /usr/include
LIB_DIR = /usr/lib

main: shared tst-libstring

tst-libstring: libstring.c

shared: libstring.so
libstring.so: libstring.c
	$(CC) $(CFLAGS) -shared -o $@ $<
	chmod -x $@

install: libstring.so
	install  libstring.so $(LIB_DIR)
	install  libstring.h  $(INCLUDE_DIR)

uninstall:
	 $(RM)  $(LIB_DIR)/libstring.so
	 $(RM)  $(INCLUDE_DIR)/libstring.h

clean:
	$(RM) test-string *~ libstring.so  tst-libstring
