CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = main.o

SOURCES = oshw2.c

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET) resources.txt allocations.txt requests.txt

.PHONY: all clean run