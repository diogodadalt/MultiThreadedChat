#!/bin/bash
sudo rm server.o server
gcc -o server server.c ../common/socket_wrapper.c client_info.c -lpthread; 
clear; 
./server
