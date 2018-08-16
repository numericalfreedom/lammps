#!/bin/bash


# Eigen implemented vector operation:

g++ -I/usr/include/eigen3 -c function.cpp


# Create static library:

ar ruv libfunction.a function.o
ranlib libfunction.a


# Fortran function interface to C++ implemented Eigen vector operation function:

gfortran -c ffunction.f


# Fortran program calls C++ function with Eigen implemented vector operation:

gfortran -c program.f


# Link with C++ linker:

g++ -L./ program.o ffunction.o -lfunction -lgfortran -o eigentest.out


