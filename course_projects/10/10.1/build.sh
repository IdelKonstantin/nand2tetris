#!/bin/bash
g++ -std=c++14 main.cpp JackTokenizer.cpp -o tokenizer -lstdc++fs -O0 -g -lpthread
echo "Build complete. Run ./tokenizer <file.jack>"
