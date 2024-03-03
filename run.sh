#!/bin/bash
clear

echo Compiling all scripts
gcc time_pipe.c -o time_pipe
gcc time_shm.c -o time_shm

echo Running all scripts in terminal
echo
./time_shm ls -l
echo
./time_pipe ls -l
echo

echo Running all scripts into text files
echo
./time_shm ls -l > time_shm_output.txt
./time_pipe ls -l > time_pipe_output.txt

echo Cleaning up files
rm time_shm
rm time_pipe