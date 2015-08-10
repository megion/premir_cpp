#.SUFFIXES: .cpp .o
#SOURCES=main.cpp hello.cpp factorial.cpp
#OBJECTS=$(SOURCES:.cpp=.o)


CFLAGS=-c -Wall -std=c++11

#.cpp.o:
#	$(CC) $(CFLAGS) -c -o $@ $<

SRCS = src/main.cpp
OBJS = $(SRCS:.cpp=.o)

primer: $(OBJS)
	$(CC) -o primer $(OBJS) $(CFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

#primer: .cpp.o 
	#$(CC) $(LDFLAGS) $(OBJECTS) -o $@