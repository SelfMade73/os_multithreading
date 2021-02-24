#!/bin/bash
first="os6_1"
second="os6_2"
echo "Building $first.cpp"
g++ $first.cpp -lpthread -lrt -o $first
echo "Excutable $first"

echo "Building $second.cpp"
g++ $second.cpp -lpthread -lrt -o $second
echo "Excutable $second"