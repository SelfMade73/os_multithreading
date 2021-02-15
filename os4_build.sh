#!/bin/bash
client="os4_1"
parent="os4_2"
echo "Building $client"
g++ $client.cpp -o $client
echo "Excutable $client"

echo "Building $parent"
g++ $parent.cpp -o $parent
echo "Excutable $parent"