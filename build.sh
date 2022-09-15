#!/bin/sh

source ./config.txt
cmake -S . -B $build_directory $1 && cd $build_directory && make doc && make && cd ..

if [ ! -f compile_commands.json ]; then
  ln -s $build_directory/compile_commands.json compile_commands.json
fi
