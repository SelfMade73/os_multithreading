#!/bin/bash
file_name="os3"
echo "Building $file_name.cpp ..."
g++ $file_name.cpp -lpthread -o $file_name
echo "Executable file is: $file_name"