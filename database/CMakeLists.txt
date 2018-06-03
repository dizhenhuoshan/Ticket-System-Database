cmake_minimum_required(VERSION 3.10)
project(myYMT)

set(CMAKE_CXX_STANDARD 11)

add_executable(myYMT main.cpp user.h constant.h utility.h interpreter.h processor.h
        ./bptree/BPlusTree.h
        ./bptree/BufferManager.h
        ./bptree/constant.h
        ./bptree/vector.h
        ./bptree/TreeNode.h train.h keys.h)