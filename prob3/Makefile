CC = gcc
CFLAGS = -Wall -ggdb3 -std=c99
LFLAGS = -Wall
LIBS = -lm -lpthread

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = prob3

all:	$(TARGET)

$(TARGET):	$(OBJS)
	gcc $(LFLAGS) -o $@ $^ $(LIBS)

%.o:	%.c
	gcc $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
