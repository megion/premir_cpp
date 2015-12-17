

#all:
#	cd build; make
	
all:
	cd mybuild; make test

clean:
	cd mybuild; make clean
	
	
.PHONY: clean

