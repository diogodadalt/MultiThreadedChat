#!/bin/bash
# Dependencies:
# sudo apt-get install libglib2.0-dev
# sudo apt-get install libcdk5-dev
#
sudo rm client.o client
gcc -o client client.c user_interface.c `pkg-config --cflags --libs glib-2.0` -lpthread -lcdk -lncurses; 
clear; 
./client localhost
