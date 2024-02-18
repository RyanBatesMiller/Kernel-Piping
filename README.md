## UID: 905757984

## Pipe Up

C program that simulates the behavior of shell pipes, allowing users to pass 
executable names as command line arguments to be executed in a pipeline.

## Building

In directory:
> make

## Running
To run:
./pipe arg1 arg2 ... argn

Examples:
./pipe ls
> equivalent to
> ls

./pipe ls cat wc
> equivalent to
> ls | cat | wc

## Cleaning up

> make clean
