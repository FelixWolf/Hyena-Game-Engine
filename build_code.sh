#!/usr/bin/env bash
clear
echo Building code...
g++ -std=c++11 -c ./src/main.cpp -o ./bin/main.out

RESULT=$?
if [ $RESULT -ne 0 ]; then
	exit 1
fi

echo Linking binary...
g++ -o ./bin/a ./bin/main.out -lGL -lglfw -lGLEW -lSDL2

RESULT=$?
if [ $RESULT -ne 0 ]; then
	exit 1
fi
echo Cleaning up...
rm ./bin/main.out

if [[ -n "$1" ]] && [[ $1 == "run" ]]; then
    ./bin/a
fi
