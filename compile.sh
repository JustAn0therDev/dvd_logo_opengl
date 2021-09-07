#!/bin/bash
# If this one file does not work, change the name of the file.
gcc main.c -o dvd_logo_opengl glad.o -lglfw -ldl
./dvd_logo_opengl