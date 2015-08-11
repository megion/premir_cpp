CC = g++
CCFLAGS = -std=c++11
SRCS = src/main.cpp
OBJS = $(SRCS:.cpp=.o)

primer: $(OBJS)
	$(CC) -o primer $(OBJS) $(CCFLAGS)

.cpp.o:
	$(CC) $(CCFLAGS) -c -o $@ $<
	
clean:
	rm -rf $(OBJS) primer
	
print:
	echo $(OBJS)

