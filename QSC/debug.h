#ifndef MYDATABASE_DEBUG_H
#define MYDATABASE_DEBUG_H

#include <iostream>
#include "BPlusTree.h"
#include "BufferManager.h"

namespace sjtu {
    class debugger {
    public:
        debugger() = default;

        void debug_msg() {
            insertFindTest();
        }

        void insertFindTest() {
            UniqueBPlusTree<int, double> bp("test1");
            bp.open_file();

            bp.insert(2, 2.5);
            std::cout << bp.find(2) << std::endl;

            bp.insert(3, 3.5);
            std::cout << bp.find(3) << std::endl;

            bp.insert(4, 4.5);
            std::cout << bp.find(4) << std::endl;

            bp.insert(5, 5.5);
            std::cout << bp.find(5) << std::endl;

            // bp.view();       // up to here is correct.

            int bound = 15;
            for(int i = 10; i < bound; ++i)
                bp.insert(i, i + 0.5);
            std::cout << "the answer should be " << bound - 1 + 0.5 << "  find: ";
            std::cout << bp.find(bound - 1) << std::endl;

            std::cout << "test1 complete" << std::endl;
        }

        void staticStorageTest() {

            UniqueBPlusTree<int, double> bp("test2");
            bp.open_file();

            int bound = 15;
            for(int i = 0; i < bound; ++i)
                bp.insert(i, i + 0.5);

            bp.close_file();

            UniqueBPlusTree<int, double> bp2("test2");

            bp2.open_file();
            int key = 10;
            std::cout << "the answer should be " << key + 0.5 << "  find: ";
            std::cout << bp2.find(key) << std::endl;

            bp2.close_file();
            std::cout << "test2 complete" << std::endl;
        }
    };
};
#endif //MYDATABASE_DEBUG_H
