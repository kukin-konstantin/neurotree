#!/bin/bash

GCC=g++
BINARY=neuro

$GCC -c ./*.cpp
$GCC -o $BINARY ./*.o -lboost_thread -lboost_system

