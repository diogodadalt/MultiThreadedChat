#!/bin/bash
# Dependencies:
# sudo apt-get install libcdk5-dev
#
sudo rm client.o client
gcc -o client client.c user_interface.c ../common/socket_wrapper.c -lpthread -lcdk -lncurses; 
clear; 
./client localhost
