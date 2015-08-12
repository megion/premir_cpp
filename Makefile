CC = g++
CCFLAGS = -std=c++11

# получить список всех файлов *.cpp
SRCS := $(wildcard src/*.cpp src/*/*.cpp)

SRC_DIRS := src src/foo
search_wildcards := $(addsuffix /*.cpp, $(SRC_DIRS))

# список каталогов, где шаблонные правила будут искать зависимости
VPATH := $(SRC_DIRS)

# укороченный вызов функции patsubst $(имя_переменной:.старый_суффикс=.новый_суффикс)
# полный вызов выглядит так: OBJS := $(patsubst   %.cpp, %.o, $(SRCS))
# получится список файлов *.o разделенный пробелами
#OBJS := $(SRCS:.cpp=.o)
OBJS := $(notdir $(patsubst %.cpp, %.o, $(wildcard $(search_wildcards))))

primer: $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@

# .cpp.o:
#	$(CC) $(CCFLAGS) -c -o $@ $<

var1 := $(addprefix -I, $(SRC_DIRS))

%.o: %.cpp
	echo $<
	$(CC) $(CCFLAGS) -c -MD $(var1) $<
# TextLine.o: TextLine.h - необходимо доработать зависисмость от *.h файлов
	
clean:
	rm -rf $(OBJS) primer
	

src_files := $(wildcard src/*.cpp src/*/*.cpp)	

print:
	echo $(var1)
	
.PHONY: clean

