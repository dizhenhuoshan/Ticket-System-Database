#ifndef MYDATABASE_DEBUG_H
#define MYDATABASE_DEBUG_H

#include <iostream>
#include "BPlusTree.h"
#include "BufferManager.h"
#include "TreeNode.h"
#include "time.h"
#include <cstdlib>
#include <algorithm>
#include <map>


namespace sjtu {
    class debugger {
    public:
        debugger() = default;


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

            bp2.view();

            bp2.close_file();
            std::cout << "test2 complete" << std::endl;
        }

        void insertDeleteTest() {
            UniqueBPlusTree<int, double> bp("test3");
            bp.open_file();

            int bound = 5;
            for(int i = 0; i < bound; ++i)
                bp.insert(i, i + 0.5);
            std::cout << "B+ tree after insertion." << std::endl;
            bp.view();

            std::cout << std::endl << "delete 0" << std::endl;
            bp.erase(0);
            bp.view();

            std::cout << std::endl << "delete 1" << std::endl;
            bp.erase(1);
            bp.view();

            std::cout << std::endl << "delete 2" << std::endl;
            bp.erase(2);
            bp.view();

            std::cout << std::endl << "delete 3" << std::endl;
            bp.erase(3);
            bp.view();

            std::cout << std::endl << "delete 4" << std::endl;
            bp.erase(4);
            bp.view();

            bp.close_file();
            std::cout << "test3 complete" << std::endl;
        }

        void traverseTest() {
            UniqueBPlusTree<int, double> bp("test4");
            bp.open_file();

            for(int i = 0; i < 5; ++i)
                bp.insert(i, i + 0.5);
            bp.bm.traverse();

            bp.close_file();
        }

        void test_insert() {
            srand(time(0));
            timer tm;
            UniqueBPlusTree<int, double> bp("test5");
            std::map<int, double> m;
            double st, et;

            bp.open_file();
            int keyArr[10000];
            for(int i = 0; i < 10000; ++i)
                keyArr[i] = i;

            int perTime = rand() % 10000;
            while(perTime--)
                std::next_permutation(keyArr, keyArr + 10000);

            st = tm.get_time();
            for(int i = 0; i < 10000; ++i) {
                int key = keyArr[i];
                double val = rand() * 1.7;
                bp.insert(key, val);
                m.emplace(key, val);
            }

            for(int i = 0; i < 10000; ++i) {
                double val1, val2;
                val1 = bp.find(i);
                val2 = m.at(i);
                if(val1 != val2) {
                    fprintf(stderr, "mismatch! key: %d; bp: %f; map: %f\n", i, val1, val2);
                }
            }
            et = tm.get_time();

            printf("%fs has passed\n", et - st);

            bp.close_file();
            printf("test5 complete!\n");

            st = tm.get_time();
            m.clear();
            for(int i = 0; i < 10000; ++i) {
                int key = keyArr[i];
                double val = rand() * 1.7;
                m.emplace(key, val);
            }
            for(int i = 0; i < 10000; ++i) {
                double val1;
                val1 = bp.find(i);
                if(val1 == -1) {
                    fprintf(stderr, "mismatch! key: %d; bp: %f; map: %f\n", i, val1, -1.0);
                }
            }
            et = tm.get_time();
            printf("for std::map alone, %fs has passed\n", et - st);
        }

        void test_erase() {
            srand(time(0));
            UniqueBPlusTree<int, double> bp("test5");
            int keyArr[20];
            int perTime = rand() % 20;

            for(int i = 0; i < 20; ++i)
                keyArr[i] = 2 * i;


            //while(perTime--)
            //    std::next_permutation(keyArr, keyArr + 20);

            bp.open_file();

            for(int i = 0; i < 20; ++i)
                bp.insert(keyArr[i], keyArr[i]);

            bp.bm.traverse();

            for(int i = 19; i >= 0; --i) {
                bp.erase(keyArr[i]);
                bp.bm.traverse();
                bp.view();
            }

            bp.close_file();
        }

        void debug_erase() {
            UniqueBPlusTree<int, double> bp("test6");
            bp.open_file();
            for(int i = 0; i < 8; ++i)
                bp.insert(i, i + 0.5);
            bp.view();


            for(int i = 7; i >= 0; --i) {
                if(i == 4)
                    int a = 1;
                bp.erase(i);
                bp.view();
            }

            bp.close_file();
        }

        void info_test() {
            UniqueBPlusTree<int, double> bp("test6");
            bp.open_file();
            for(int i = 0; i < 10; ++i) {
                bp.insert(i, i + 0.5);
                printf("insert %d\n", i);
                bp.bm.traverse();
            }
            bp.close_file();
        }
    };
};
#endif //MYDATABASE_DEBUG_H
