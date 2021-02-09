#!/bin/bash

files=( "os2_mutex" "os2_trylock_mutex" "os2_timedlockmutex" )

for item in ${files[*]}
do
    echo "Building $item.cpp ..."
    g++ $item.cpp -lpthread -o $item
    echo "Executable file is: $item"
    echo
done