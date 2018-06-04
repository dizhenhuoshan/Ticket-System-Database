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
#include "ticket_kv.h"
#include "bill_kv.h"


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

        BPlusTree<ticket_key, ticket_val> *ticket_db;
        BPlusTree<bill_key, bill_val> *bill_db;

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

            ticket_db = new BPlusTree<ticket_key, ticket_val>();
            ticket_db->set_filename("zticket_db.txt");

            bill_db = new BPlusTree<bill_key, bill_val>();
            bill_db->set_filename("zbill_db.txt");

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
            delete bill_db;
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
                /**
                 * User command: */
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

                /**
                 * Train command: */
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

                /**
                 * Query ticket command: */

                else if (cmd == QUERY_TICKET) {
                    if (!task_query_ticket())
                        printf("%d\n", 0);
                }
                else if (cmd == QUERY_TRANSFER) {
                    if(!task_query_transfer())
                        printf("%d\n", -1);
                }
                /**
                 * Buy ticket command: */
                else if (cmd == BUY_TICKET) {
                    if (task_buy_ticket())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }

                else if (cmd == REFUND_TICKET) {
                    if (task_refund_ticket())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }
                else if (cmd == QUERY_ORDER) {
                    if (!task_query_bill())
                        printf("%d\n", -1);
                }
                /**
                 * Other command: */
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

        bool task_buy_ticket();

        /**
        * create a order_key and zip it, with which to search in bill_db->
        * if it doesn't exist or there isn't enough tickets to refund, return false.
        * otherwise, we need to change it, and also change in ticket_db->
        * delete bill if all tickets are gone. */
        bool task_refund_ticket();

        /**
         * query different catalog respectively, and store output information in bill_val struct.
         * put them in array, and count their total number.
         * finally print it. */
        bool task_query_bill();

        bool task_query_ticket();

        bool task_query_transfer();

        bool task_clean();
        /**
         * create file. */
        void init_open_file() {
            user_db->open_file();
            train_id_db->open_file();
            train_info_db->open_file();
            station_db->open_file();
            ticket_db->open_file();
            bill_db->open_file();
            CR.open_file();
        }
        void close_file() {
            user_db->close_file();
            train_id_db->close_file();
            train_info_db->close_file();
            station_db->close_file();
            ticket_db->close_file();
            bill_db->close_file();
            CR.close_file();
        }

    private:
        bool get_train_info(const char *train_id, train_info_val &train_info);
        bool get_train_station_indices(const train_info_val &train_info, const char *station1, const char *station2, char &station_idx1, char &station_idx2);
        bool get_train_seat_catalog_index(const train_info_val &train_info, const char *seat_kind, char &seat_idx);

        /**
         * get the ticket sold for a particular train on a particular day along all stations passed by this train. */
        void get_ticket_sold(const train_info_val &train_info, ticket_key *tickets_key, ticket_val *tickets_val, const int &len, short ticket_sold[][5]);
        bool check_sufficient(const char &station_idx1, const char &station_idx2, const char &seat_kind_idx, short ticket_sold[][5], const short &bought_num);

        /**
         * 你只要把那个数组容器给我，把当前长度给我，我把我要到的东西都append在后面，然后更新数组长度变量。*/
        void query_bill_per_catalog(const int &user_id, const short &num_date, const char &train_cat, bill_val *bills_val, int &len, bill_key *bills_key);
        /**
         * sub-function used by task_query_bill, which structures bill information and print it out. */
        void print_bill(const bill_key &b_key, const bill_val &b_val);

        /**
         * sub-function used by task_query_ticket. */
        void query_ticket_per_catalog(const char *station1, const char *station2, const char *date, const char &train_cat, ticket_wrap wrap[], int &len);

        /**
         * get all the trains passing one station with a certain train_catalog. */
        void get_trains_pass_loc(const char *loc, const char &cat, station_val *station_vals, int &len);

        /**
         * fill data into return data_wrapper.
         * Note that we need to change date_head in order to match different dates for different stations. */
        void fill_query_ticket(const train_info_val &train_info, const int &station_idx1, const int &station_idx2, const char *date_head,
                               const short *ticket_left, ticket_wrap &wrapper);

        void get_ticket_left(const train_info_val &train_info, const char &station_idx1, const char &station_idx2, const char *date_head, short *ticket_left);
    };
    #include "processor.cpp"
};
#endif //MYYMT_PROCESSOR_H
