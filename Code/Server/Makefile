.PHONY: all main clean
CC=gcc
CFLAGS=
LIBS = -lm
LDFLAGS = -lm


EXECUTABLE=main
SRCS := $(wildcard *.c)

OBJS = $(SRCS:.c=.o)
#OBJS := $(patsubst %.c,%.o,$(wildcard *.c))

all: $(EXECUTABLE)

#$(EXECUTABLE): $(SOURCES)
#$(CC) -o $@ $(SOURCES) $(CFLAGS) $(LIBS)


#clean:
#rm -f $(EXECUTABLE) *.o


# Link the object files into the final executable
$(EXECUTABLE) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	


