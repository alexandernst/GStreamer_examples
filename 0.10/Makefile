CC		=	@gcc
CFLAGS	=	-g -Wall -w -Wextra -Werror
CFLAGS	+=	$(shell pkg-config --cflags gstreamer-0.10)

LDLIBS	=	libs/helpers.o
LDLIBS	+=	$(shell pkg-config --libs gstreamer-0.10)

SOURCES	=	$(shell ls *.c)
DESTS	=	$(patsubst %.c,%,$(SOURCES))

new: clean all

clean:
	@rm -rf *.o */*.o $(DESTS)

all: libs/helpers.o $(DESTS)