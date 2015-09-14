


SRCS := src src/foo src/utils

include ../template.Makefile


all: primer

# создание выполняемого файла
primer: $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@

# очистка	
#clean:
#	rm -rf $(OBJS) *.d primer
	
.PHONY: clean

