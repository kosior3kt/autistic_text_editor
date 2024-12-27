#!/bin/bash
eval cc main.c $(pkg-config --libs --cflags raylib) -g -o prog
