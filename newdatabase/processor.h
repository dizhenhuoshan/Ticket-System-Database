#ifndef MYYMT_PROCESSOR_H
#define MYYMT_PROCESSOR_H

#include <iostream>
#include <unistd.h>
#include "limits.h"

#include "./bptree/BPlusTree.h"
#include "interpreter.h"

#include "user_kv.h"
#include "train_info_kv.h"
#include "train_id_kv.h"
#include "station_kv.h"
#include "const_recorder.h"


namespace sjtu {

    /**
     * processor is a class that process the information.
     * It has a private member interpreter which assigns tasks.
     * it reads in parameters from stdin, and gives out corresponding feedback. */
    class processor {
    private:
        interpreter itpr;
        BPlusTree<user_key, user_val> *user_db;

        BPlusTree<train_id_key, train_id_val> *train_id_db;
        pile_train_storer *train_info_db;

        BPlusTree<station_key, station_val> *station_db;

        const_recorder CR;
        bool clean;
        /** processor utility is a bunch of necessary variable that records things like id. */
    public:
        processor() {
            user_db = new BPlusTree<user_key, user_val>();
            user_db->set_filename("zuser_db.txt");

            train_id_db = new BPlusTree<train_id_key, train_id_val>();
            train_info_db = new pile_train_storer();
            train_id_db->set_filename("ztrain_id_db.txt");
            train_info_db->set_filename("ztrain_pile.txt");

            station_db = new BPlusTree<station_key, station_val>();
            station_db->set_filename("zstation_db.txt");

            CR.set_filename("zconst.txt");

            init_open_file();
            clean = false;
        }
        ~processor() {
            if (!clean)
                close_file();
            delete user_db;
            delete train_id_db;
            delete train_info_db;
            delete station_db;
        }

        /**
         * master waits, organizes, assigns queries. */
        void master() {
            while (true) {
                // printf("debug: %d\n", bill_db->size());
                command cmd = itpr.parser();

                if(clean && cmd != CLEAN && cmd != EXIT && cmd != NULLCOMMAND) {
                    init_open_file();
                    clean = false;
                }

                if (cmd == REGISTER) {
                    printf("%d\n", task_register());
                }
                else if (cmd == QUERY_PROFILE) {
                    if(!task_query_profile())
                        printf("0\n");
                }
                else if (cmd == LOGIN) {
                    if (task_login())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }
                else if (cmd == MODIFY_PROFILE) {
                    if (task_modify_profile())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }
                else if (cmd == MODIFY_PRIVILEGE) {
                    if (task_modify_privilege())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }

                else if (cmd == ADD_TRAIN) {
                    if (task_add_train())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }

                else if (cmd == SALE_TRAIN) {
                    if (task_sale_train())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }

                else if (cmd == QUERY_TRAIN) {
                    train_info_val ret;
                    if (task_query_train(ret))
                        ret.view();
                    else
                        printf("%d\n", 0);
                }

                else if (cmd == DELETE_TRAIN) {
                    if (task_delete_train())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }
                else if (cmd == MODIFY_TRAIN) {
                    if (task_modify_train())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }

                else if (cmd == CLEAN) {
                    if(task_clean())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }

                else if (cmd == EXIT) {
                    printf("BYE\n");
                    break;
                }
                else if (cmd == NULLCOMMAND) {
                    printf("invalid command\n");
                }
            }
        }

        int task_register();
        bool task_query_profile();
        bool task_login();
        bool task_modify_profile();
        bool task_modify_privilege();
        /**
         * add a train, if train_id has existed, return false.
         * otherwise return true to indicate successful operation.
         *
         * when judge, turn off all robust check. */
        bool task_add_train();
        bool task_sale_train();
        bool task_delete_train();
        bool task_modify_train();
        /**
         * query information on a train with specific train_id.
         * if train_id doesn't exist, return false.
         * Otherwise return true, and fill ret with data.
         * Note that we !don't! care whether this train is on sale. */
        bool task_query_train(train_info_val &ret);


        bool task_clean();
        /**
         * create file. */
        void init_open_file() {
            user_db->open_file();
            train_id_db->open_file();
            train_info_db->open_file();
            station_db->open_file();
            CR.open_file();
        }
        void close_file() {
            user_db->close_file();
            train_id_db->close_file();
            train_info_db->close_file();
            station_db->close_file();
            CR.close_file();
        }
    };
    #include "processor.cpp"
};
#endif //MYYMT_PROCESSOR_H
