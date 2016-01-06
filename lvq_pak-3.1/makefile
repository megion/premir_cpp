
# 
CC=cc
CFLAGS=-O
LDFLAGS=-s
LDLIBS=-lm
LD=$(CC)

## NetBSD-1.0 (tested on Amiga)
## 
#CC=gcc
#CFLAGS=-O2
#LDFLAGS=-s
#LDLIBS=-lm
#LD=$(CC)

# DEC Alpha/OSF
#
#CC=cc
#CFLAGS=-O2 
#LDFLAGS=-s
#LDLIBS=-lm
#LD=$(CC)

## Amiga/AmigaDOS
##  Tested on an Amiga 2000 w/ 68030+68882 with gcc and dmake, OS 3.1
#CC=gcc
#CFLAGS=-O2 -m68020 -m68881
#LDFLAGS=-s
#LDLIBS=-lm
#LD=$(CC)

## HPUX
##
#CC=c89
#CFLAGS=-O
#LDFLAGS=-s
#LDLIBS=-lm
#LD=$(CC)

## Linux
##
#CC=gcc
#CFLAGS=-O2
#LDFLAGS=-s
#LDLIBS=-lm
#LD=$(CC)

## SGI
##
#CC=cc
#CFLAGS=-O2 -mips1
#LDFLAGS=-s
#LDLIBS=-lm
#LD=$(CC)

TESTFILES=ex1.dat ex2.dat
OBJS=lvq_rout.o lvq_pak.o fileio.o labels.o datafile.o version.o
OTHERFILES=
PAKDIR=lvq_pak-3.0

all:	accuracy knntest pick setlabel lvqtrain lvq1 lvq2 lvq3 olvq1 eveninit \
	propinit showlabs mindist mcnemar sammon cmatr elimin balance \
	stddev classify extract lvq_run

accuracy:	accuracy.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ accuracy.o $(OBJS) $(LDLIBS)

knntest:	knntest.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

pick:	pick.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

setlabel:	setlabel.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

classify:	classify.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

showlabs:	showlabs.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

extract:	extract.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

mindist:	mindist.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

stddev:	stddev.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

balance:	balance.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

mcnemar:	mcnemar.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

sammon:	sammon.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

cmatr:	cmatr.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

elimin:	elimin.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

lvqtrain:	lvqtrain.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

lvq1:	lvqtrain
	rm -f $@
	ln lvqtrain $@

olvq1:	lvqtrain
	rm -f $@
	ln lvqtrain $@

lvq2:	lvqtrain
	rm -f $@
	ln lvqtrain $@

lvq3:	lvqtrain
	rm -f $@
	ln lvqtrain $@

eveninit:	eveninit.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

propinit:	eveninit
	rm -f $@
	ln eveninit $@

lvq_run:	lvq_run.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $@.o $(OBJS) $(LDLIBS)

dist:
	rm -f lvq_pak-3.0.tar.gz lvq_pak.tar 
	tar -cvf lvq_pak-3.0.tar *.c *.h Makefile.* $(TESTFILES) $(OTHERFILES)
	gzip -v -9 lvq_pak.tar

version.o: lvq_pak.o fileio.o labels.o datafile.o version.h 

example :
	./eveninit -din ex1.dat  -cout ex1e.cod -noc 200
	./mindist  -cin ex1e.cod
	./balance  -din ex1.dat  -cin ex1e.cod  -cout ex1b.cod
	./olvq1    -din ex1.dat  -cin ex1b.cod  -cout ex1o.cod -rlen 5000
	./accuracy -din ex2.dat  -cin ex1o.cod

fileio.o:	fileio.h
datafile.o:	lvq_pak.h datafile.h fileio.h
labels.o:	labels.h lvq_pak.h
lvq_pak.o:	lvq_pak.h datafile.h fileio.h
lvq_rout.o:     lvq_rout.h lvq_pak.h datafile.h fileio.h

accuracy.o knntest.o pick.o setlabel.o lvqtrain.o eveninit.o \
propinit.o showlabs.o mindist.o mcnemar.o sammon.o cmatr.o \
elimin.o balance.o stddev.o classify.o lvq_run.o:	lvq_pak.h fileio.h datafile.h labels.h lvq_rout.h


