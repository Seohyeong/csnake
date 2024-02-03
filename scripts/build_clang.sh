#!/bin/bash

cd $(dirname $0)
set -e

pushd ..

BUILD_DIR=./build
rm -r -f $BUILD_DIR
mkdir -p $BUILD_DIR

EXTRA_FLAGS="-Werror -Wall -g -O0"

clang++ main.cpp $EXTRA_FLAGS -std=c++17 -Ilibs/raylib/Debug/include -L./libs/raylib/Debug/lib -lraylib -Wl,-framework,cocoa -Wl,-framework,iokit -o $BUILD_DIR/test

popd


# -Werror => treat warnings as errors
# -Wall => enable all warnings
# -g => build debug information (so debugger can work)
# -O0 => disable optimization
# -std=c++<number> => use c++ version <number>
# -I<path> => add this path to include directories, when you do #include <…>, it will look there also
# -L<path> => add this path to lib directories, when you link a library, it will look there also for the lib file
# -l<name> => link this library
# -Wl => comma seperated list linker arguments (using it to add frameworks, whatever that is, some mac thing)
# -o <path> => output path

# ./build/zero2nn/Debug/zero2nn