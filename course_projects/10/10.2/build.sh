#!/bin/bash

# Компилируем все объектные файлы
g++ -std=c++14 -c *.cpp

# Линкуем все вместе
g++ -std=c++14 JackTokenizer.o JackAnalyzer.o main.o -o analyserSimple -lstdc++fs -O0 -g -lpthread

rm -rf *.o

echo "Build complete. Run ./analyserSimple <file.jack | directory>"
