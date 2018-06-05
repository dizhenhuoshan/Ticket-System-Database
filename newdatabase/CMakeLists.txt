cmake_minimum_required(VERSION 3.10)
project(newdatabase)

set(CMAKE_CXX_STANDARD 11)

add_executable(newdatabase main.cpp
        interpreter.h
        utility.h
        ./bptree/BPlusTree.h
        ./bptree/BufferManager.h
        ./bptree/constant.h
        ./bptree/vector.h
        ./bptree/TreeNode.h
        train_info_kv.h
        user_kv.h
        constant.h
        train_id_kv.h
        station_kv.h
        const_recorder.h
        processor.h
        ticket_kv.h bill_kv.h)