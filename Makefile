CFLAGS += -W -Wall -Wextra -Werror -L. -finline-small-functions

INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib

main: shared tst-libstring

tst-libstring: CFLAGS += -ggdb3 -fsanitize=address
tst-libstring: libstring.c

shared: CFLAGS += -O3 -fstack-protector-all -fPIC -s -D_FORTIFY_SOURCE=2 -z now
shared: libstring.so
libstring.so: libstring.c
	$(CC) $(CFLAGS) -shared -o $@ $<
	chmod -x $@

install: libstring.so
	install -m 644  libstring.so $(LIB_DIR)
	install -m 644 libstring.h  $(INCLUDE_DIR)

uninstall:
	 $(RM)  $(LIB_DIR)/libstring.so
	 $(RM)  $(INCLUDE_DIR)/libstring.h

html:
	doxygen doxygen.conf

clean:
	$(RM) test-string *~ libstring.so tst-libstring
	$(RM) -r html/
