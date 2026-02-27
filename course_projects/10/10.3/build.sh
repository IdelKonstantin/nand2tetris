#!/bin/bash

# Компилируем все объектные файлы
g++ -std=c++14 -c *.cpp

# Линкуем все вместе
g++ -std=c++14 JackTokenizer.o JackCompiler.o main.o -o jackCompiler -lstdc++fs -O0 -g -lpthread

rm -rf *.o

echo "Build complete. Run ./jackCompiler <file.jack | directory>"
