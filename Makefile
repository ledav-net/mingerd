
DESTBIN:=	mingerd

build:          $(DESTBIN)

include         .depends

OBJS:=		common.o main.o init.o sigevents.o config.o log.o emails.o digest.o

LIBRARIES:=	-lbeecrypt
INCLUDES:=	-I$(HOME)/include

CFLAGS:=	-O2 -Wall -pipe
#CFLAGS:=	-Wall -pipe -ggdb

PREFIX:=	$(HOME)
INSTALLBIN:=	$(PREFIX)/bin

.SUFFIXES:	.c .o

$(DESTBIN):	$(OBJS)
		cc $(CFLAGS) -o $(DESTBIN) $(OBJS) $(LIBRARIES)

install:        build
		@install -D -s -m 0755 --verbose $(DESTBIN) $(INSTALLBIN)/$(DESTBIN)

uninstall:
		@rm -vf $(INSTALLBIN)/$(DESTBIN)

dep\
.depends:
		@echo "Rebuilding dependencies from sources ..."
		@rm -f .depends
		@if ls *.c  &> /dev/null; then gcc -M $(INCLUDES) *.c > .depends; else exit 0; fi

clean:
		@rm -vf *~ $(OBJS)

distclean:	clean
		@rm -vf $(DESTBIN) .depends
