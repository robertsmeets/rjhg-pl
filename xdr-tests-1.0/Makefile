# $Id: Makefile,v 1.3 2007/04/11 09:35:25 rjones Exp $

PACKAGE	:= xdr-tests
VERSION	:= 1.0

CC	:= gcc
CFLAGS	:= -g -Wall -Wno-unused -Werror -Iinclude -D__BSD_VISIBLE  -D___int64_t_defined
LFLAGS	:= -static -L/usr/lib/debug/usr/lib64

all: test.c test.h test1 test2

test1: test1.o test.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

test2: test2.o test.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

test.c: test.x
	rm -f $@
	rpcgen -5 -c -o $@ $<

test.h: test.x
	rm -f $@
	rpcgen -5 -h -o $@ $<

clean:
	rm -f test1 test2 *.out *.o core *~ test.c test.h

# Distribution.
dist:
	$(MAKE) check-manifest
	rm -rf $(PACKAGE)-$(VERSION)
	mkdir $(PACKAGE)-$(VERSION)
	tar -cf - -T MANIFEST | tar -C $(PACKAGE)-$(VERSION) -xf -
	tar zcf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION)
	rm -rf $(PACKAGE)-$(VERSION)
	ls -l $(PACKAGE)-$(VERSION).tar.gz

check-manifest:
	@for d in `find -type d -name CVS | grep -v '^\./debian/'`; \
	do \
	b=`dirname $$d`/; \
	awk -F/ '$$1 != "D" {print $$2}' $$d/Entries | \
	sed -e "s|^|$$b|" -e "s|^\./||"; \
	done | sort > .check-manifest; \
	sort MANIFEST > .orig-manifest; \
	diff -u .orig-manifest .check-manifest; rv=$$?; \
	rm -f .orig-manifest .check-manifest; \
	exit $$rv
