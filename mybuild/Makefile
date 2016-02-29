# Справка
# $@ имя цели правила
# $< имя первой зависимости правила
# $^ список всех зависимостей правила
# 
# <цели> : <зависмости>
# 	<команда1>
# 	<команда2>

CC := g++
# -lxcb for xcb lib
# -lX11 for X11 lib
# -g for include debugging symbols
# -pthread enable multithreading
CCFLAGS := -std=c++11 -lxcb -g -pthread -lpng

# список каталогов с исходыми кодами
SRC_DIRS := $(shell find ../src -type d)
TEST_DIRS := $(shell find ../test -type d) $(SRC_DIRS)

# список всех файлов *.cpp
SRC := $(shell find ../src -name *.cpp)
TEST_SRC := $(shell find {../test,../src} -name *.cpp | sed '/primer/d')

# Укороченный вызов функции patsubst
# $(имя_переменной:.старый_суффикс=.новый_суффикс)
# полный вызов выглядит так: OBJS := $(patsubst   %.cpp, %.o, $(SRCS))
# получится список всех объектных файлов *.o разделенный пробелами
#OBJS := $(SRCS:.cpp=.o)
SRC_OBJS := $(notdir $(patsubst %.cpp, %.o, $(SRC)))
TEST_OBJS := $(notdir $(patsubst %.cpp, %.o, $(TEST_SRC)))

all: primer test_runner

# создание выполняемого файла primer
primer: $(SRC_OBJS)
	$(CC) $(CCFLAGS) $^ -o $@
	
# создание выполняемого файла test
test_runner: $(TEST_OBJS)
	$(CC) $(CCFLAGS) $^ -o $@
	
test: test_runner
	@./$<
	
# список каталогов, где шаблонные правила будут искать зависимости
VPATH := $(TEST_DIRS)

# шаблонное правило создания объектных файлов
# флаг -I для того чтобы разрешить компилятору искать заголовочные файлы
# в указанных директориях
# флаг -MMD для создания файлов *.d со списком зависимостей: сам исходный файл,
# и файлы, включаемые с помощью директивы #include "имя_файла"  
%.o: %.cpp
	$(CC) $(CCFLAGS) -c -MMD $(addprefix -I, $(TEST_DIRS)) $<
	@printf "\e[36m$(CC)\e[90m %s\e[0m\n" $@
include $(wildcard *.d)

# очистка	
clean:
	rm -rf *.o *.d primer test_runner

print:
	echo $(TEST_DIRS)
	
.PHONY: clean

