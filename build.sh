#! /bin/sh -e
g++  -pedantic -std=c++17 -Wall -Wno-unused-parameter -Wno-reorder -Wno-sign-compare -Wno-address -Wno-noexcept-type -c src/main.cpp -o build/main.o
g++  -pedantic -std=c++17 -Wall -Wno-unused-parameter -Wno-reorder -Wno-sign-compare -Wno-address -Wno-noexcept-type -c src/register.cpp -o build/register.o
g++ build/main.o build/register.o  -lgmpxx -lgmp -o Emulator
