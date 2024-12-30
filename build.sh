#!/bin/bash
eval cc main.c \
	$(pkg-config --libs --cflags raylib)\
	-Wall \
	-Wextra \
	-g \
	-Wno-unused-variable \
	-o prog
