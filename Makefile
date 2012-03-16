
DESTBIN:=	mingerd

build:          $(DESTBIN)

include         .depends

OBJS:=		common.o main.o init.o sigevents.o config.o log.o emails.o digest.o

LIBRARIES:=	-lbeecrypt
INCLUDES:=

CFLAGS:=	-O2 -Wall -pipe
#CFLAGS:=	-Wall -pipe -ggdb

VERSION:=	1.x

PREFIX:=	$(HOME)
INSTALLBIN:=	$(PREFIX)/bin
INSTALLDOC:=	$(PREFIX)/doc
INSTALLETC:=	$(PREFIX)/etc

.SUFFIXES:	.c .o

$(DESTBIN):	$(OBJS)
		cc $(CFLAGS) -o $(DESTBIN) $(OBJS) $(LIBRARIES)

install:        build
		@install -D -m 0755 --verbose -s $(DESTBIN) $(INSTALLBIN)/$(DESTBIN)
		@install -D -m 0640 --verbose $(DESTBIN).conf $(INSTALLETC)/$(DESTBIN).conf
		@install -D -m 0644 --verbose README $(INSTALLDOC)/$(DESTBIN)-$(VERSION)/README
		@install -D -m 0644 --verbose COPYING $(INSTALLDOC)/$(DESTBIN)-$(VERSION)/COPYING

uninstall:
		@rm -vf $(INSTALLBIN)/$(DESTBIN)
		@mv -vf $(INSTALLETC)/$(DESTBIN).conf $(INSTALLETC)/$(DESTBIN).conf.save
		@rm -vf $(INSTALLDOC)/$(DESTBIN)-$(VERSION)/README
		@rm -vf $(INSTALLDOC)/$(DESTBIN)-$(VERSION)/COPYING
		@rmdir -v $(INSTALLDOC)/$(DESTBIN)-$(VERSION)

dep\
.depends:
		@echo "Rebuilding dependencies from sources ..."
		@rm -f .depends
		@if ls *.c  &> /dev/null; then gcc -M $(INCLUDES) *.c > .depends; else exit 0; fi

clean:
		@rm -vf *~ $(OBJS)

distclean:	clean
		@rm -vf $(DESTBIN) .depends
