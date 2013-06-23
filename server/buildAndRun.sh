#!/bin/bash
sudo rm server.o server
gcc -o server server.c -lpthread; 
clear; 
./server
