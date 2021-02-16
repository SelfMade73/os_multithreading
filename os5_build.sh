#!/bin/bash
first="os5"
second="os5_1"
echo "Building $first.cpp"
g++ $first.cpp -lpthread -o $first
echo "Excutable $first"

echo "Building $second.cpp"
g++ $second.cpp -lpthread -o $second
echo "Excutable $second"