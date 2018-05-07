cmake_minimum_required(VERSION 3.10)
project(myDatabaseThirdVersion)

set(CMAKE_CXX_STANDARD 11)

add_executable(myDatabaseThirdVersion main.cpp BPlusTree.h TreeNode.h)