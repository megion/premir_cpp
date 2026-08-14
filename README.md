
# Clusters analysis

Build only Linux OS because for visualization has used xcb lib.

## Install dependensies
  `$ dnf install libpng-devel libxcb-devel`

## Build

Build use make or cmake 

1. Use Makefile
  `$ make`

2. Use CMake
  `$ cd build/`
  `$ cmake ..`
  `$ make`


## Build and run

```sh
# build run
$ make

# generate `compile_commands.json` and build and run 
$ bear -- make

```
