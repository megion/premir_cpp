# Справка
# $@ имя цели правила
# $< имя первой зависимости правила
# $^ список всех зависимостей правила
# 
# <цели> : <зависмости>
# 	<команда1>
# 	<команда2>

CC := g++
CCFLAGS := -std=c++11

# получить список всех файлов *.cpp
#SRCS := $(wildcard src/*.cpp src/*/*.cpp)

SRC_DIRS := src src/foo src/utils

# если файлы сборки будут помещаться например в каталог release.
# Запуск командой: make --directory=release --makefile=../Makefile
SRC_DIRS := $(addprefix ../, $(SRC_DIRS))

SRC_SEARCHES := $(addsuffix /*.cpp, $(SRC_DIRS))

# список каталогов, где шаблонные правила будут искать зависимости
VPATH := $(SRC_DIRS)

# Укороченный вызов функции patsubst
# $(имя_переменной:.старый_суффикс=.новый_суффикс)
# полный вызов выглядит так: OBJS := $(patsubst   %.cpp, %.o, $(SRCS))
# получится список всех объектных файлов *.o разделенный пробелами
#OBJS := $(SRCS:.cpp=.o)
OBJS := $(notdir $(patsubst %.cpp, %.o, $(wildcard $(SRC_SEARCHES))))

all: primer

# создание выполняемого файла
primer: $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@

# правило создания объектных файлов
# флаг -I для того чтобы разрешить компилятору искать заголовочные файлы
# в указанных директориях
# флаг -MMD для создания файлов *.d со списком зависимостей: сам исходный файл,
# и файлы, включаемые с помощью директивы #include "имя_файла"  
%.o: %.cpp
	$(CC) $(CCFLAGS) -c -MMD $(addprefix -I, $(SRC_DIRS)) $<
include $(wildcard *.d)

# очистка	
clean:
	rm -rf $(OBJS) *.d primer

print:
	echo $(OBJS)
	
.PHONY: clean

