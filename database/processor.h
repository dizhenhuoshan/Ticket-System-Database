#ifndef MYYMT_PROCESSOR_H
#define MYYMT_PROCESSOR_H

#include "interpreter.h"
#include "./bptree/BPlusTree.h"
#include "user.h"
#include "train.h"
#include "keys.h"
#include <iostream>
#include <unistd.h>
#include "limits.h"

namespace sjtu {

    /**
     * processor is a class that process the information.
     * It has a private member interpreter which assigns tasks.
     * it reads in parameters from stdin, and gives out corresponding feedback. */
    class processor {
    private:
        interpreter itpr;
        BPlusTree<user_key, user> *user_db;
        pile_train_storer *train_info_db;
        BPlusTree<train_id_key, train_id_val> *train_id_db;
        BPlusTree<loc_key, loc_val> *loc_db;
        BPlusTree<ticket_key, ticket_val> *ticket_db;
        BPlusTree<order_key, order_val> *bill_db;

        bool clean;
        /** processor utility is a bunch of necessary variable that records things like id. */
    public:
        processor() {
            user_db = new BPlusTree<user_key, user>();
            train_info_db = new pile_train_storer();
            train_id_db = new BPlusTree<train_id_key, train_id_val>();
            loc_db = new BPlusTree<loc_key, loc_val>();
            ticket_db = new BPlusTree<ticket_key, ticket_val>();
            bill_db = new BPlusTree<order_key, order_val>();

            user_db->set_filename("zuser_db.txt");
            train_id_db->set_filename("ztrain_id_db.txt");
            train_info_db->set_filename("ztrain_pile.txt");
            loc_db->set_filename("zlocation_db.txt");
            ticket_db->set_filename("zticket_db.txt");
            bill_db->set_filename("zbill_db.txt");

            init_open_file();
            clean = false;
        }

        ~processor() {
            if(!clean)
                close_file();
            delete user_db;
            delete train_info_db;
            delete train_id_db;
            delete loc_db;
            delete ticket_db;
            delete bill_db;
        }

        // void debug() {
        //     loc_db->bm.traverse();
        //    ticket_db->bm.traverse();
        // } 
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
                    char msg[NAME_SIZE + EMAIL_SIZE + PHONE_SIZE + PRIVILEGE_SIZE + 1];
                    task_query_profile(msg);
                    printf("%s\n", msg);
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
                else if (cmd == QUERY_TRAIN) {
                    train ret;
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
                else if (cmd == SALE_TRAIN) {
                    if (task_sale_train())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }
                else if (cmd == BUY_TICKET) {
                    if (task_buy_ticket())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }
                else if (cmd == QUERY_ORDER) {
                    if (!task_query_order())
                        printf("%d\n", -1);
                }
                else if (cmd == QUERY_TICKET) {
                    if (!task_query_ticket())
                        printf("%d\n", 0);
                }
                else if (cmd == REFUND_TICKET) {
                    if(task_refund_ticket())
                        printf("%d\n", 1);
                    else
                        printf("%d\n", 0);
                }
                else if (cmd == QUERY_TRANSFER) {
                    if(!task_query_transfer())
                        printf("%d\n", -1);
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

        int task_register() {
            user newUser;

            // find user_id and privilege first.
            if (user_db->size() == 0) {
                newUser.user_id = START_USER_ID;
                newUser.privilege = 2;
            }
            else {
                newUser.user_id = user_db->back().user_id + 1;
                newUser.privilege = 1;
            }

            // read in user information.
            newUser.read();

            // 插入名册元素。
            user_db->insert(newUser.user_id, newUser);
            return newUser.user_id;
        }

        void task_query_profile(char *msg) {
            user_key id;
            user ret;

            scanf("%d", &id);
            ret = user_db->find(id);

            // check whether found.
            if (ret.null_obj()) {
                msg[0] = '0';
                msg[1] = '\0';
            }
            else {
                strcpy(msg, ret.name);
                strcat(msg, " ");
                strcat(msg, ret.email);
                strcat(msg, " ");
                strcat(msg, ret.phone);
                strcat(msg, " ");
                // since privilege is integer, it's a little bit messier here.
                int len = strlen(msg);
                msg[len++] = (char) (ret.privilege + '0');
                msg[len] = '\0';
            }
        }

        bool task_login() {
            user_key id;
            char pswd[PASSWORD_SIZE];
            user user_found;

            scanf("%d %s", &id, pswd);
            user_found = user_db->find(id);
            // printf("%s\n", user_found.password);
            // user not found or password not match.
            if (user_found.null_obj() || strcmp(pswd, user_found.password) != 0)
                return false;
            else
                return true;
        }

        bool task_modify_profile() {
            user_key id;
            user ret;

            scanf("%d", &id);
            ret = user_db->find(id);
            // even if this query is invalid, we have to read a user obj to clear stdin stream.
            if (ret.null_obj()) {
                ret.read();
                return false;
            }
            else {
                ret.read();
                user_db->modify(id, ret);
                return true;
            }
        }

        bool task_modify_privilege() {
            int id1, id2;
            int privilege;
            user user1, user2;

            scanf("%d %d %d", &id1, &id2, &privilege);
            user1 = user_db->find(id1);
            user2 = user_db->find(id2);
		if(id1 == id2) 
			return false; 
            // if users exist.
            if (user1.null_obj() || user2.null_obj())
                return false;

            // simple user can't use this command.
            if (user1.privilege == 1)
                return false;
            // admin can't degenerate other admin to simple user.
            if (user2.privilege > 1 && privilege == 1)
                return false;
            // do things.
            user2.privilege = privilege;
            user_db->modify(id2, user2);
            return true;
        }

        /**
         * add a train, if train_id has existed, return false.
         * otherwise return true to indicate successful operation.
         *
         * when judge, turn off all robust check. */
        bool task_add_train() {
            train newTrain;
            train_id_val tmp;

            newTrain.read();
            // check whether train_id has existed.
            tmp = train_id_db->find(train_id_key(newTrain.train_id));
            if (!tmp.null_obj())
                return false;

            // insert both in pile and B+.
            // B+:
            train_id_db->insert(newTrain.train_id, train_info_db->get_tail_offset());   // implicitly call constructor.
            // pile:
            train_info_db->append_train(newTrain);
            return true;
        }

        /**
         * query information on a train with specific train_id.
         * if train_id doesn't exist, return false.
         * Otherwise return true, and fill ret with data.
         * Note that we !don't! care whether this train is on sale. */
        bool task_query_train(train &ret) {
            char id[ID_SIZE];
            train_id_val val;

            scanf("%s", id);
            val = train_id_db->find(id);

            // check whether id exists or whether it's on sale.
            if (val.null_obj() || !val.sale)
                return false;
            // query for train information.
            train_info_db->get_train_by_offset(val.address, ret);
            return true;
        }

        bool task_delete_train() {
            char id[ID_SIZE];
            train_id_val val;

            scanf("%s", id);
            val = train_id_db->find(id);

            // if id doesn't exist or the train has been on sale, return false.
            // otherwise erase it.
            if (val.null_obj() || val.sale)
                return false;
            else {
                train_id_db->erase(id); // erase train_id.
                return true;
            }
        }

        bool task_modify_train() {
            train modified_train;
            train_id_val val;

            modified_train.read();
            val = train_id_db->find(modified_train.train_id);

            // if train doesn't exist, or the train has been on sale, return false.
            if (val.null_obj() || val.sale)
                return false;
            else {
                train_info_db->write_train_by_offset(val.address, modified_train);
                return true;
            }
        }

        bool task_sale_train() {
            char id[ID_SIZE];
            train_id_val id_val;

            scanf("%s", id);
            id_val = train_id_db->find(id);

            // if the train doesn't exist, or the train has been on sale, return false.
            // otherwise sale it, and put its information into loc database.
            if (id_val.null_obj() || id_val.sale == true)
                return false;
            else {
                train ret;

                train_info_db->get_train_by_offset(id_val.address, ret);

                // record all ret's stations into loc_db->
                for (int i = 0; i < ret.station_num; ++i) {
                    loc_db->insert(loc_key{ret.stations[i].station_name, ret.train_catalog, ret.train_id},
                                   loc_val{ret.train_id, i, ((i + 1) == ret.station_num)});
                }
                // change id_val.sale to be true, and store it.
                id_val.sale = true;
                train_id_db->modify(id, id_val);
                return true;
            }
        }

        bool task_buy_ticket() {
            int user_id;
            int num;
            char train_id[ID_SIZE];
            char loc1[LOCATION_SIZE];
            char loc2[LOCATION_SIZE];
            char date[DATE_SIZE];
            char ticket_kind[TICKET_KIND_SIZE]; // correspond to seat catalog.
            int num_date;

            // read in data from stdin.
            scanf("%d %d %s %s %s %s %s", &user_id, &num, train_id, loc1, loc2, date, ticket_kind);
            num_date = date_to_int(date);
            train_id_val id_val;

            id_val = train_id_db->find(train_id);
            // if the train doesn't exist or it hasn't been on sale, return false.
            // otherwise continue.
            if (id_val.null_obj() || !id_val.sale)
                return false;
            else {
                train train_info;
                int loc1_idx;
                int loc2_idx;
                int cat_idx;
                int len;

                train_info_db->get_train_by_offset(id_val.address, train_info);
                ticket_val t_val[train_info.station_num];

                // find all indices.
                loc1_idx = loc2_idx = -1;
                for (int i = 0; i < train_info.station_num; ++i) {
                    if (loc1_idx == -1) {
                        if (strcmp(loc1, train_info.stations[i].station_name) == 0)
                            loc1_idx = i;
                    } else if (loc2_idx == -1) {
                        if (strcmp(loc2, train_info.stations[i].station_name) == 0)
                            loc2_idx = i;
                    } else
                        break;
                }
                for (int i = 0; i < train_info.seat_catalog_num; ++i) {
                    if (strcmp(ticket_kind, train_info.seat_catalog[i]) == 0)
                        cat_idx = i;
                }
                // if invalid loc name, return false.
                if (loc1_idx == -1 || loc2_idx == -1)
                    return false;

                // check ticket number left to determine whether train tickets are sufficient.
                ticket_db->get_range(get_low_ticket_key(train_id, num_date), get_high_ticket_key(train_id, num_date),
                                     t_val, len);
                // if this train today hasn't been bought, just create, check, and insert it.
                if (len == 0) {
                    // 如果订购多于供应，返回false.
                    if (num > MAXTICKET)
                        return false;

                    ticket_key tk(train_id, num_date);
                    len = train_info.station_num;
                    for (int i = 0; i < len; ++i) {
                        // 在每一站都创建已售车票，每一站的车票都初始化为0。
                        t_val[i].seat_cat_num = train_info.seat_catalog_num;
                        memset(t_val[i].ticket_sold, 0, sizeof(t_val[i].ticket_sold));
                        // 购买相应车票。
                        if (loc1_idx <= i && i < loc2_idx) {
                            t_val[i].ticket_sold[cat_idx] += num;
                        }
                        // 插入。
                        tk.loc_idx = i;
                        ticket_db->insert(tk, t_val[i]);
                    }
                } else {
                    /// debug code.
                    if (len != train_info.station_num)
                        std::cerr << "ticket_db error!!!" << std::endl;

                    // 检查订购是否多余供应，返回false。
                    // 对每个站点的特定座位种类。
                    for (int i = loc1_idx; i < loc2_idx; ++i) {
                        if (t_val[i].ticket_sold[cat_idx] + num > MAXTICKET)
                            return false;
                    }

                    ticket_key tk(train_id, num_date);
                    // 增加每个站点的购票数量。
                    for (int i = loc1_idx; i < loc2_idx; ++i) {
                        t_val[i].ticket_sold[cat_idx] += num;
                        // 修改。
                        tk.loc_idx = i;
                        ticket_db->modify(tk, t_val[i]);
                    }
                }

                // 买完票之后，要在当前用户的买票记录里面增加一栏购票信息。
                order_key transaction(user_id, num_date, train_info.train_catalog, train_id, loc1_idx, loc2_idx);
                order_val transaction_info(train_id, loc1_idx, loc2_idx, cat_idx, num);
                if (bill_db->insert(transaction, transaction_info) == false) {
                    // if have existed.
                    order_val existed = bill_db->find(transaction);
                    existed.tickets[cat_idx] += num;
                    bill_db->modify(transaction, existed);
                }
                return true;
            }
        }



        /**
         * query different catalog respectively, and store output information in order_val struct.
         * put them in array, and count their total number.
         * finally print it. */
        bool task_query_order() {
            int user_id;
            char date[DATE_SIZE];
            char train_cat[CATALOG_SIZE];
            size_t cat_num;
            order_val orders[10];
            int len = 0;
            bool noEmpty = false;

            scanf("%d %s %s", &user_id, date, train_cat);
            cat_num = strlen(train_cat);

            // query different catalog.
            for(int i = 0; i < cat_num; ++i) {
                query_order_per_catalog(user_id, date, train_cat[i], orders, len);
            }

            // if data doesn't exist.
            if (len == 0)
                return false;
            else {
                // output.
                printf("%d\n", len);
                // 对每一个购票信息进行输出。
                for (int i = 0; i < len; ++i) {
                    // use order_ret struct to structure data.
                    order_ret data_wrapper;
                    order_val &ov = orders[i];
                    int train_address = train_id_db->find(ov.train_id).address;
                    train train_info;
                    train_info_db->get_train_by_offset(train_address, train_info);

                    strcpy(data_wrapper.train_id, ov.train_id);

                    strcpy(data_wrapper.beg, train_info.stations[ov.loc1_idx].station_name);
                    strcpy(data_wrapper.beg_date, date);
                    time_to_str(train_info.stations[ov.loc1_idx].start, data_wrapper.beg_time);

                    strcpy(data_wrapper.tail, train_info.stations[ov.loc2_idx].station_name);
                    strcpy(data_wrapper.tail_date, date);
                    time_to_str(train_info.stations[ov.loc2_idx].arrive, data_wrapper.tail_time);

                    data_wrapper.num_catalog = train_info.seat_catalog_num;

                    // 对第j种座位统计价格。
                    for (int j = 0; j < data_wrapper.num_catalog; ++j) {
                        // 复制座位名称。
                        strcpy(data_wrapper.seat_cat[j], train_info.seat_catalog[j]);
                        // 对价格求和。
                        double price = 0;
                        for (int k = ov.loc1_idx + 1; k <= ov.loc2_idx; ++k)
                            price += train_info.stations[k].price[j];
                        // 输入wrapper。
                        data_wrapper.seat_price[j] = price;
                        data_wrapper.seat_num[j] = ov.tickets[j];
                    }
                    data_wrapper.view();
                }
                return true;
            }
        }


        bool task_query_ticket() {
            char loc1[NAME_SIZE];
            char loc2[NAME_SIZE];
            char date[DATE_SIZE];
            char train_cat[CATALOG_SIZE];
            ticket_ret ret[1000];
            int len_ret;
            size_t cat_len;

            scanf("%s %s %s %s", loc1, loc2, date, train_cat);
            cat_len = strlen(train_cat);
            len_ret = 0;

            for(int i = 0; i < cat_len; ++i) {
                query_ticket_per_catalog(loc1, loc2, date, train_cat[i], ret, len_ret);
            }

            // if fail to find suitable train.
            if (len_ret == 0)
                return false;
            else {
                // 输出所有query到的ticket。
                printf("%d\n", len_ret);
                for (int i = 0; i < len_ret; ++i)
                    ret[i].view();
                return true;
            }
        }

        /**
         * create a order_key and zip it, with which to search in bill_db->
         * if it doesn't exist or there isn't enough tickets to refund, return false.
         * otherwise, we need to change it, and also change in ticket_db->
         * delete bill if all tickets are gone. */
        bool task_refund_ticket() {
            int user_id;
            int refund_num;
            char train_id[ID_SIZE];
            char loc1[NAME_SIZE];
            char loc2[NAME_SIZE];
            char date[DATE_SIZE];
            char seat_kind[TICKET_KIND_SIZE];
            int num_date;
            int loc1_idx;
            int loc2_idx;
            int seat_cat_idx;

            scanf("%d %d %s %s %s %s %s", &user_id, &refund_num, train_id, loc1, loc2, date, seat_kind);
            num_date = date_to_int(date);

            // 额外查询车辆信息来确定 catalog和loc_idx.
            train train_info;
            get_train_info_from_id(train_id, train_info);
            get_locidx_from_loc(train_info, loc1, loc2, loc1_idx, loc2_idx);

            order_key bill_key(user_id, num_date, train_info.train_catalog, train_id, loc1_idx, loc2_idx);
            // 查询交易信息。
            order_val bill_val = bill_db->find(bill_key);

            // 如果交易信息根本他妈就不存在。
            if(bill_val.null_obj()) {
                return false;
            }
            else {
                // 找到特定座位的票是不是足够退。
                for(int i = 0; i < train_info.seat_catalog_num; ++i) {
                    if(strcmp(seat_kind, train_info.seat_catalog[i]) == 0){
                        seat_cat_idx = i;
                        break;
                    }
                }
                // 如果不够退，你还玩个猴子。
                if(bill_val.tickets[seat_cat_idx] < refund_num)
                    return false;
                else {
                    // 决定了，我要退票。
                    bill_val.tickets[seat_cat_idx] -= refund_num;
                    // 看看这个订单会不会变得trivial。
                    bool trivial = true;
                    for(int i = 0; i < train_info.seat_catalog_num; ++i) {
                        if(bill_val.tickets[i] > 0) {
                            trivial = false;
                            break;
                        }
                    }
                    // 如果这个订单变得trivial，那就删掉它；否则修改它。
                    if(trivial)
                        bill_db->erase(bill_key);

                    else
                        bill_db->modify(bill_key, bill_val);

                    // 然后我们要在票堆里面也进行删除。所幸在ticket_db当中必定存在购票信息。
                    ticket_key low_tk = get_low_ticket_key(train_id, num_date);
                    ticket_key high_tk = get_high_ticket_key(train_id, num_date);
                    ticket_key tk(train_id, num_date);
                    // 根据这个车过了多少站，定义车票信息的数组。
                    ticket_val t_val[train_info.station_num];
                    int station_len;    // 没必要的参数，它必定等于train_info.station_num.

                    ticket_db->get_range(low_tk, high_tk, t_val, station_len);

                    // sanity check.
                    if(station_len != train_info.station_num)
                        std::cerr << "refund ticket error" << std::endl;

                    for(int i = loc1_idx; i < loc2_idx; ++i) {
                        // 在每个station都减掉相应的退票数。因为已经被买掉的票变少了。
                        t_val[i].ticket_sold[seat_cat_idx] -= refund_num;
                        // 修改回去。
                        tk.loc_idx = i;
                        ticket_db->modify(tk, t_val[i]);
                    }
                    return true;
                }
            }
        }



        bool task_query_transfer() {
            char loc1[LOCATION_SIZE];   // 出发地
            char loc2[LOCATION_SIZE];   // 到达地
            char date[DATE_SIZE];       // 当前日期
            char catalog[CATALOG_SIZE]; // 所有要查询的车次种类
            size_t cat_len;             // 车次种类数量
            loc_val loc2_v[1000];       // 所有经过到达地的简易车辆信息
            int len2;
            train train_info2;          // 换乘后的车辆信息


            int min_time = INT_MAX;     // 最短时间
            ticket_ret min_ret1;        // 最快车辆信息1
            ticket_ret min_ret2;        // 最快车辆信息2

            scanf("%s %s %s %s", loc1, loc2, date, catalog);
            cat_len = strlen(catalog);

            // 遍历所有车辆种类
            for(int l = 0; l < cat_len; ++l) {
                get_all_trains_loc(loc2, catalog[l], loc2_v, len2); // 拿到loc2_v

                // 遍历所有到达loc2的车子。
                for (int i = 0; i < len2; ++i) {
                    // 如果loc2是起始车站，那么放弃它。
                    if (loc2_v[i].station_idx == 0)
                        continue;

                    get_train_info_from_id(loc2_v[i].train_id, train_info2);    // 取得当前车辆的详细信息。
                    // 遍历当前换乘后的第二辆车的所有前面的车站，也就是寻找换乘车站。
                    int station_idx = 0;
                    while(strcmp(train_info2.stations[station_idx].station_name, loc2) != 0)
                        station_idx++;

                    for (int j = 0; j < station_idx; ++j) {
                        // 先看看票还剩多少。
                        ticket_val t_val[train_info2.station_num];
                        int station_len;
                        int ticket_left[5];

                        get_ticket_sale_info(train_info2.train_id, date_to_int(date), t_val, station_len);

                        if(station_len == 0) {
                            for(int t = 0; t < 5; ++t)
                                ticket_left[t] = 2000;
                        }
                        else {
                            // sanity check.
                            if(station_len != train_info2.station_num)
                                std::cerr << "transfer error" << std::endl;
                            for(int t = 0; t < 5; ++t) {
                                ticket_left[t] = INT_MAX;
                                for(int s = j; s < station_idx; ++s) {
                                    ticket_left[t] = std::min(ticket_left[t], 2000 - t_val[s].ticket_sold[t]);
                                }
                            }
                        }
                        // 如果票全部卖完了，直接跳过把。
                        bool skip = true;
                        for(int t = 0; t < 5; ++t) {
                            if(ticket_left[t] != 0)
                                skip = false;
                            break;
                        }
                        if(skip)
                            continue;
                        // 我们继续查找。

                        ticket_ret ticketRet1[100];     // 找到从出发地到换乘车站的所有车票
                        int len1;
                        len1 = len2 = 0;
                        query_ticket_per_catalog(loc1, train_info2.stations[j].station_name, date, catalog[l], ticketRet1, len1);
                        // 遍历这些车票。
                        for(int k = 0; k < len1; ++k) {
                            // 我们得保证第一辆车车和后一辆train_info2不同。
                            if(strcmp(ticketRet1[k].train_id, train_info2.train_id) == 0)
                                continue;
                            // 否则的话我们计算所需时间。
                            int train1_begT = time_to_int(ticketRet1[k].beg_time);
                            int train1_endT = time_to_int(ticketRet1[k].tail_time);
                            int train2_begT = train_info2.stations[j].start;
                            int train2_endT = train_info2.stations[loc2_v[i].station_idx].arrive;

                            // 如果在前一辆车车到达的时候，后一辆车车已经开走了，那不就凉凉么。
                            if(train1_endT > train2_begT)
                                continue;

                            // 如果我们找到时间更短的方案咯，那就更新！！！
                            if(train2_endT - train1_begT < min_time) {
                                min_time = train2_endT - train1_begT;
                                min_ret1 = ticketRet1[k];

                                // 第二辆列车的更新比较繁琐。
                                strcpy(min_ret2.train_id, train_info2.train_id);

                                strcpy(min_ret2.beg, train_info2.stations[j].station_name);
                                time_to_str(train_info2.stations[j].start, min_ret2.beg_time);
                                strcpy(min_ret2.beg_date, date);

                                strcpy(min_ret2.tail, train_info2.stations[station_idx].station_name);
                                time_to_str(train_info2.stations[station_idx].arrive, min_ret2.tail_time);
                                strcpy(min_ret2.tail_date, date);


                                // 找到价格。
                                double price[5];
                                for(int t = 0; t < train_info2.seat_catalog_num; ++t) {
                                    price[t] = 0;
                                    for(int s = j + 1; s <= station_idx; ++s) {
                                        price[t] += train_info2.stations[s].price[t];
                                    }
                                }

                                for(int t = 0; t < train_info2.seat_catalog_num; ++t) {
                                    strcpy(min_ret2.seat_cat[t], train_info2.seat_catalog[t]);
                                    min_ret2.seat_num_left[t] = ticket_left[t];
                                    min_ret2.seat_price[t] = price[t];
                                }
                                min_ret2.num_catalog = train_info2.seat_catalog_num;
                            }
                        }
                        /*
                        // 因为已经按字典序拍好了，所以取出同一辆车子的车票
                        int cur1, cur2;
                        cur1 = cur2 = 0;
                        while (cur1 < len1 && cur2 < len2) {
                            int cmp = strcmp(ticketRet1[cur1].train_id, ticketRet2[cur2].train_id);
                            // 如果车次不相同，我们开始操作。
                            if (cmp != 0) {
                                // 如果前一辆车子在后一辆车子之后，那么简直是凉凉。
                                if (time_to_int(ticketRet1[cur1].tail_time) > time_to_int(ticketRet2[cur2].beg_time))
                                    continue;
                                // 比较时间是不是短，如果是的话，那么我们就更新。
                                int time = time_to_int(ticketRet2[cur2].tail_time) -
                                           time_to_int(ticketRet1[cur1].beg_time);
                                if (time < min_time) {
                                    min_time = time;

                                    strcpy(min_ret.train_id, ticketRet1[cur1].train_id);

                                    strcpy(min_ret.beg, ticketRet1[cur1].beg);
                                    strcpy(min_ret.beg_date, ticketRet1[cur1].beg_date);
                                    strcpy(min_ret.beg_time, ticketRet1[cur1].beg_time);

                                    strcpy(min_ret.tail, ticketRet2[cur2].tail);
                                    strcpy(min_ret.tail_date, ticketRet2[cur2].tail_date);
                                    strcpy(min_ret.tail_time, ticketRet2[cur2].tail_time);

                                    for (int k = 0; k < ticketRet1[cur1].num_catalog; ++k) {
                                        strcpy(min_ret.seat_cat[k], ticketRet1[cur1].seat_cat[k]);
                                        min_ret.seat_num_left[k] = std::min(ticketRet1[cur1].seat_num_left[k],
                                                                            ticketRet2[cur2].seat_num_left[k]);
                                        min_ret.seat_price[k] =
                                                ticketRet1[cur1].seat_price[k] + ticketRet2[cur2].seat_price[k];
                                    }
                                }
                            }
                            else if (cmp < 0)
                                ++cur1;
                            else
                                ++cur2;
                        }
                         */
                    }
                }

            }
            // 如果他妈更本就没车，骗鬼呢！
            if(min_time == INT_MAX)
                return false;
            else {
                // 如果有一辆车，输出来给大爷们瞧瞧啊。
                min_ret1.view();
                min_ret2.view();
                return true;
            }
        }

        bool task_clean() {
            // 如果已经删库了，你还删个猴子。
            if(clean)
                return false;
            else {
                close_file();
                unlink(".user_db->txt");
                unlink(".train_id_db->txt");
                unlink(".train_pile.txt");
                unlink(".location_db.txt");
                unlink(".ticket_db->txt");
                unlink(".bill_db->txt");
                clean = true;
                return true;
            }
        }
        /*
        bool task_query_transfer() {
            char loc1[LOCATION_SIZE];
            char loc2[LOCATION_SIZE];
            char date[DATE_SIZE];
            char catalog[CATALOG_SIZE];

            // loc1--start, loc2--end, loc3--transfer.
            char loc3[LOCATION_SIZE];
            loc_val loc1_v[1000];
            int len1;
            loc_val loc2_v[1000];
            int len2;
            loc_val loc3_v[1000];
            int len3;
            train train_info2;

            int min_time = INT_MAX;
            char q_train1_id[ID_SIZE];  // q: quick.
            char q_train2_id[ID_SIZE];
            // 位置序号。
            int q_loc11_idx;
            int q_loc12_idx;
            int q_loc21_idx;
            int q_loc22_idx;

            get_all_trains_loc(loc1, catalog, loc1_v, len1);
            get_all_trains_loc(loc2, catalog, loc2_v, len2);

            if(len1 == 0 || len2 == 0)
                return false;

            // 遍历所有到达loc2的车子。
            for(int i = 0; i < len2; ++i) {
                // 如果loc2是起始车站，那么放弃它。
                if(loc2_v[i].station_idx == 0)
                    continue;
                // 遍历这辆车的前置站点。
                get_train_info_from_id(loc2_v[i].train_id, train_info2);
                for(int j = 0; j < loc2_v[i].station_idx; ++j) {
                    // 把loc3弄的更清楚一些。
                    int loc11_idx;
                    int loc12_idx;
                    int loc21_idx;
                    int loc22_idx;

                    loc21_idx = j;
                    loc22_idx = loc2_v[i].station_idx;

                    strcpy(loc3, train_info2.stations[j].station_name);
                    get_all_trains_loc(loc3, catalog, loc3_v, len3);
                    // 找到loc1_v和loc3_v中重叠的train_id.
                    int cur1, cur3;
                    cur1 = cur3 = 0;
                    while(cur1 < len1 && cur3 < len3) {
                        int cmp;
                        cmp = strcmp(loc1_v[cur1].train_id, loc3_v[cur3].train_id);
                        // 如果train_id相等。
                        if(cmp == 0) {
                            // 仔细比较。
                            train train_info1;
                            get_train_info_from_id(loc1_v[cur1].train_id, train_info1);

                           // 找到loc_idx.
                            loc11_idx = loc12_idx = -1;
                            for(int k = 0; k < train_info1.station_num; ++k) {
                                if(loc11_idx != -1 && loc12_idx != -1)
                                    break;
                                if(strcmp(train_info1.stations[k].station_name, loc1) == 0)
                                    loc11_idx = k;
                                if(strcmp(train_info1.stations[k].station_name, loc3) == 0)
                                    loc12_idx = k;
                            }
                            // 如果loc3不在loc1后面，那么神仙也救不了你。
                            if(loc12_idx <= loc11_idx) {
                                ++cur1, ++cur3;
                                continue;
                            }
                            // 如果train_info1的到中间站的时间晚于train_info2，那么神仙也救不了你。
                            if(train_info1.stations[loc12_idx].arrive > train_info2.stations[loc21_idx].start) {
                                ++cur1, ++cur3;
                                continue;
                            }
                            // 如果时间短诶，记录一下当前的。
                            if(train_info2.stations[loc22_idx].arrive - train_info1.stations[loc11_idx].start < min_time) {
                                min_time = train_info2.stations[loc22_idx].arrive - train_info1.stations[loc11_idx].start;
                                q_loc11_idx = loc11_idx;
                                q_loc12_idx = loc12_idx;
                                q_loc21_idx = loc21_idx;
                                q_loc22_idx = loc22_idx;
                                strcpy(q_train1_id, train_info1.train_id);
                                strcpy(q_train2_id, train_info2.train_id);
                                ++cur1, ++cur3;
                            }
                        }
                        else if(cmp < 0)
                            ++cur1;
                        else
                            ++cur3;
                    }
                }
            }
            if(min_time == INT_MAX)
                return false;

        }
         */
    private:

        /**
         * get all the trains passing one station with a certain train_catalog. */
        void get_all_trains_loc(const char *loc, const char &cat, loc_val loc_ret[], int &len) {
            loc_key low;
            loc_key high;

            low = get_low_loc_key(loc, cat);
            high = get_high_loc_key(loc, cat);
            loc_db->get_range(low, high, loc_ret, len);
        }

        /**
         * get train information from train_id. */
        void get_train_info_from_id(const char *train_id, train &train_info_ret) {
            train_id_val id_val;

            id_val = train_id_db->find(train_id);
            train_info_db->get_train_by_offset(id_val.address, train_info_ret);
        }

        /**
         * get location index associated with a train from its location name char string.
         * if invalid name, return false. */
        bool
        get_locidx_from_loc(const train &train_info, const char *loc1, const char *loc2, int &loc1_idx, int &loc2_idx) {
            loc1_idx = loc2_idx = -1;
            for (int i = 0; i < train_info.station_num; ++i) {
                if (loc1_idx == -1) {
                    if (strcmp(loc1, train_info.stations[i].station_name) == 0)
                        loc1_idx = i;
                } else if (loc2_idx == -1) {
                    if (strcmp(loc2, train_info.stations[i].station_name) == 0)
                        loc2_idx = i;
                } else
                    break;
            }
            if (loc1_idx == -1 || loc2_idx == -1)
                return false;
            else
                return true;
        }

        /**
         * get ticket sale information given train_id and date. store info in an array of ticket_val, and return length. */
        void get_ticket_sale_info(const char *train_id, const int &date, ticket_val t_val[], int &station_len) {
            ticket_key low_tk = get_low_ticket_key(train_id, date);
            ticket_key high_tk = get_high_ticket_key(train_id, date);
            ticket_db->get_range(low_tk, high_tk, t_val, station_len);
        }

        /**
         * given an array of ticket_val, and location indices, return the maximum ticket sold for each kind. */
        void
        get_max_ticket_sold(const ticket_val t_val[], const int &seat_cat_num, const int &loc1_idx, const int &loc2_idx,
                            int ticket_sold[]) {
            // 寻找区间最小值。
            for (int i = loc1_idx; i < loc2_idx; ++i) {
                for (int cat = 0; cat < seat_cat_num; ++cat)
                    if (ticket_sold[cat] < t_val[i].ticket_sold[cat])
                        ticket_sold[cat] = t_val[i].ticket_sold[cat];
            }
        }

        /**
         * fill data into return data_wrapper. */
        void fill_query_ticket(const train &train_info, const int &loc1_idx, const int &loc2_idx, const char *date,
                               const int ticket_left[], ticket_ret &wrapper) {
            strcpy(wrapper.train_id, train_info.train_id);

            strcpy(wrapper.beg, train_info.stations[loc1_idx].station_name);
            strcpy(wrapper.beg_date, date);
            time_to_str(train_info.stations[loc1_idx].start, wrapper.beg_time);

            strcpy(wrapper.tail, train_info.stations[loc2_idx].station_name);
            strcpy(wrapper.tail_date, date);
            time_to_str(train_info.stations[loc2_idx].arrive, wrapper.tail_time);

            wrapper.num_catalog = train_info.seat_catalog_num;

            // 对第j种座位统计价格。
            for (int j = 0; j < wrapper.num_catalog; ++j) {
                // 复制座位名称。
                strcpy(wrapper.seat_cat[j], train_info.seat_catalog[j]);
                // 对价格求和。
                double price = 0;
                for (int k = loc1_idx + 1; k <= loc2_idx; ++k)
                    price += train_info.stations[k].price[j];
                // 输入wrapper。
                wrapper.seat_price[j] = price;
                wrapper.seat_num_left[j] = ticket_left[j];
            }
        }

        /**
         * return total time used going from station1 to station2 given a specific train. */
        int get_time(const train &t, const int &loc1_idx, const int &loc2_idx) {
            return t.stations[loc2_idx].arrive - t.stations[loc1_idx].start;
        }

        /**
         * create file. */
        void init_open_file() {
            user_db->open_file();
            train_id_db->open_file();
            train_info_db->open_file();
            loc_db->open_file();
            ticket_db->open_file();
            bill_db->open_file();
        }

        void close_file() {
            user_db->close_file();
            train_id_db->close_file();
            train_info_db->close_file();
            loc_db->close_file();
            ticket_db->close_file();
            bill_db->close_file();
        }

        /**
         * query order for a particular catalog. and print it directly.
         * if there is a record of order, return true.
         * otherwise return false.
         * 你只要把那个数组容器给我，把当前长度给我，我把我要到的东西都append在后面，然后更新数组长度变量。*/
        void query_order_per_catalog(const int &user_id, const char date[], const char &train_cat, order_val *orders, int &len) {
            int num_date;
            int add_len;

            num_date = date_to_int(date);
            order_key low = get_low_order_key(user_id, num_date, train_cat);
            order_key high = get_high_order_key(user_id, num_date, train_cat);
            bill_db->get_range(low, high, orders + len, add_len);
            len += add_len;
        }

        /**
         * sub-function used by task_query_ticket. */
        void query_ticket_per_catalog(const char *loc1, const char *loc2, const char *date, const char &train_cat, ticket_ret ret[], int &len_ret) {
            loc_val loc1_vals[1000];
            loc_val loc2_vals[1000];
            int len1;
            int len2;
            int num_date;

            num_date = date_to_int(date);
            // get all trains passing location_1.
            get_all_trains_loc(loc1, train_cat, loc1_vals, len1);
            // get all trains passing location_2.
            get_all_trains_loc(loc2, train_cat, loc2_vals, len2);

            // because the trains aligned in order of their id. check out all suitable ones.
            int cur1, cur2;
            cur1 = cur2 = 0;

            // 什么时候停止查找。
            while (cur1 < len1 && cur2 < len2) {
                // 进行一次改变。
                // 如果正好找到出入站相等的咯！
                int cmp = strcmp(loc1_vals[cur1].train_id, loc2_vals[cur2].train_id);
                if (cmp == 0) {
                    // 判断这个是否合适，加入ret，否则直接掠过+1。
                    // if loc1 is ahead of loc2.
                    if (loc1_vals[cur1].station_idx < loc2_vals[cur2].station_idx) {
                        train train_info;
                        int station_len;
                        int loc1_idx;
                        int loc2_idx;
                        int ticket_left[5];

                        // 找到车次信息。
                        get_train_info_from_id(loc1_vals[cur1].train_id, train_info);
                        // 根据这个车过了多少站，定义车票信息的数组。
                        ticket_val t_val[train_info.station_num];
                        // 根据车次信息，找到首末站的index。if invalid loc name, return false.

                        if (!get_locidx_from_loc(train_info, loc1, loc2, loc1_idx, loc2_idx))
                            return;

                        // 然后查找车票信息。
                        get_ticket_sale_info(loc1_vals[cur1].train_id, num_date, t_val, station_len);

                        // 看看这个票是不是全卖光了？如果是，那么我们要skip。
                        bool skip = true;
                        // 如果没有找到，说明已售车票为0，说明剩余车票为2000，可以显示。
                        if (station_len == 0) {
                            skip = false;
                            for (int cat = 0; cat < train_info.seat_catalog_num; ++cat)
                                ticket_left[cat] = 2000;
                        } else {
                            // 在首末站中，已售车票为区间最大值。
                            int ticket_sold[5];
                            memset(ticket_sold, 0, sizeof(ticket_sold));
                            // 寻找区间最小值。
                            get_max_ticket_sold(t_val, train_info.seat_catalog_num, loc1_idx, loc2_idx, ticket_sold);
                            // 检查如果所有座位种类的已卖出的票都为2000，那么就直接跳过，不把它加入返回值。
                            for (int cat = 0; cat < train_info.seat_catalog_num; ++cat) {
                                if (ticket_sold[cat] != 2000) {
                                    skip = false;
                                    break;
                                }
                            }
                            for (int cat = 0; cat < train_info.seat_catalog_num; ++cat)
                                ticket_left[cat] = 2000 - ticket_sold[cat];
                        }
                        // 如果不要skip，我们来操作。
                        if (!skip) {
                            fill_query_ticket(train_info, loc1_idx, loc2_idx, date, ticket_left, ret[len_ret]);
                            ++len_ret;
                        }
                    }
                    ++cur1, ++cur2;
                } else if (cmp < 0)
                    ++cur1;
                else
                    ++cur2;
            }
        }
    };
};

#endif //MYYMT_PROCESSOR_H
