CC = gcc
CFLAGS += -std=c11 -Wall -Wextra
CLIBS = 
FILES = example.c

ifeq ($(OS),Windows_NT) 
APPLICATION_NAME = example.exe
RM = del /Q /F
else
APPLICATION_NAME = example
RM = rm -rf
endif

all: clean build

build:
	$(CC) $(CFLAGS) -o $(APPLICATION_NAME) $(FILES) $(CLIBS)

clean:
	$(RM) $(APPLICATION_NAME)
