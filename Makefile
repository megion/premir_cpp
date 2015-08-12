CC = g++
CCFLAGS = -std=c++11
SRCS = src/main.cpp

# список каталогов, где шаблонные правила будут искать зависимости
VPATH := src

# укороченный вызов функции patsubst $(имя_переменной:.старый_суффикс=.новый_суффикс)
# полный вызов выглядит так: OBJS := $(patsubst   %.cpp, %.o, $(SRCS))
# получится список файлов *.o разделенный пробелами
OBJS := $(SRCS:.cpp=.o)

primer: $(OBJS)
	$(CC) -o primer $(OBJS) $(CCFLAGS)

# .cpp.o:
#	$(CC) $(CCFLAGS) -c -o $@ $<
%.o: %.cpp
	$(CC) $(CCFLAGS) $^ -o $@
# TextLine.o: TextLine.h - необходимо доработать зависисмость от *.h файлов
	
clean:
	rm -rf $(OBJS) primer
	
print:
	echo $(OBJS)
	
.PHONY: clean

