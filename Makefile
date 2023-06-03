# Flags for the compiler and linker.
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -pedantic
LIBS = -lm
PFLAGS = -pthread
HFILES = ActiveObject.h Queue.h
RM = rm -f

# Phony targets - targets that are not files but commands to be executed by make.
.PHONY: all default clean

# Default target - compile everything and create the executables and libraries.
all: st_pipeline

# Alias for the default target.
default: all


############
# Programs #
############
st_pipeline: ActiveObject.o Queue.o Main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(PFLAGS)

################
# Object files #
################
%.o: %.c $(HFILES)
	$(CC) $(CFLAGS) -c $<
	
#################
# Cleanup files #
#################
clean:
	$(RM) *.o *.so st_pipeline