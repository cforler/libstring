CFLAGS += -W -Wall -Wextra -Werror -L. -finline-small-functions
CFLAGS += -Os -fstack-protector-all -fPIC -s -D_FORTIFY_SOURCE=2 -z now
#CFLAGS += -O1 -ggdb3 -fsanitize=address

main: shared tst-libstring

tst-libstring: libstring.c

shared: libstring.so
libstring.so: libstring.c
	$(CC) $(CFLAGS) -shared -o $@ $<
	chmod -x $@

clean:
	$(RM) test-string *~ libstring.so  tst-libstring
