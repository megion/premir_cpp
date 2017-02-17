

all: runtest

runtest:
	cd mybuild; make test

clean:
	cd mybuild; make clean

	
.PHONY: clean

