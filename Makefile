TARGET = surveillance_test

CC=g++

CFLAGS=-I. -std=c++0x -O2 -Wall
LDFLAGS=-ljson 

SRC=settings.cpp socketexception.cpp udpsocket.cpp tcpsocket.cpp main.cpp
DEPS=settings.h socketexception.h udpsocket.h tcpsocket.h
OBJ=$(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS) 

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f *.o *~ core $(TARGET)

