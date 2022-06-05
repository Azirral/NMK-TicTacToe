cmake_minimum_required(VERSION 3.22)
project(MNKTEST)

set(CMAKE_CXX_STANDARD 14)

add_executable(MNKTEST main.cpp SingleLinkedList.cpp SingleLinkedList.h Node.cpp Node.h)
