#include "processor.h"
#define DEBUG 1

bool ticket_idx_table[100];

/**
 * public function: */
int sjtu::processor::task_register() {
    user_key key;
    user_val val;

    // find user_id and privilege first.
    key = CR.append_id;
    CR.increment();

    if(key == 2018) {
        val.privilege = 2;
    }
    else {
        val.privilege = 1;
    }

    // read in user information.
    val.read();

    // 插入名册元素。
    user_db->insert(key, val);
    return key;
}
bool sjtu::processor::task_query_profile()  {
    user_key key;
    user_val val;

    scanf("%d", &key);
    val = user_db->find(key);

    // check whether found.
    if (val.null_obj()) {
        return false;
    }
    else {
        printf("%s %s %s %hd\n", val.name, val.email, val.phone, val.privilege);
        return true;
    }
}
bool sjtu::processor::task_login()  {
    user_key id;
    char pswd[PASSWORD_SIZE];
    user_val user_found;

    scanf("%d %s", &id, pswd);
    user_found = user_db->find(id);
    // printf("%s\n", user_found.password);
    // user not found or password not match.
    if (user_found.null_obj() || strcmp(pswd, user_found.password) != 0)
        return false;
    else
        return true;
}
bool sjtu::processor::task_modify_profile()  {
    user_key id;
    user_val ret;

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
bool sjtu::processor::task_modify_privilege()  {
    int id1, id2;
    short privilege;
    user_val user1, user2;

    scanf("%d %d %hd", &id1, &id2, &privilege);
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

bool sjtu::processor::task_add_train() {
    train_info_val newTrain;
    newTrain.read();

    // check whether train_id has existed.
    train_id_val tmp;
    tmp = train_id_db->find(train_id_key(newTrain.train_id));
    if (!tmp.null_obj())
        return false;

    // insert both in pile and B+.
    // pile:
    int tail_offset = train_info_db->append_train(newTrain);
    // B+:
    train_id_key id_key(newTrain.train_id);
    train_id_val id_val(tail_offset, false);
    train_id_db->insert(newTrain.train_id, id_val);   // implicitly call constructor.
    return true;
}
bool sjtu::processor::task_query_train(sjtu::train_info_val &ret)  {
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
bool sjtu::processor::task_sale_train()  {
    char id[ID_SIZE];
    train_id_val id_val;

    scanf("%s", id);
    id_val = train_id_db->find(id);

    // if the train doesn't exist, or the train has been on sale, return false.
    // otherwise sale it, and put its information into station database.
    if (id_val.null_obj() || id_val.sale)
        return false;
    else {
        train_info_val ret;

        train_info_db->get_train_by_offset(id_val.address, ret);

        // record all ret's stations into station_db->
        for (int i = 0; i < ret.station_num; ++i) {
            station_db->insert(station_key{ret.stations[i].station_name, ret.train_catalog, ret.train_id},
                           station_val{ret.train_id, i, ((i + 1) == ret.station_num)});
        }
        // change id_val.sale to be true, and store it.
        id_val.sale = true;
        train_id_db->modify(id, id_val);
        return true;
    }
}
bool sjtu::processor::task_delete_train()  {
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
bool sjtu::processor::task_modify_train()  {
    train_info_val modified_val;
    train_id_val val;

    modified_val.read();
    val = train_id_db->find(modified_val.train_id);

    // if train doesn't exist, or the train has been on sale, return false.
    if (val.null_obj() || val.sale)
        return false;
    else {
        train_info_db->write_train_by_offset(val.address, modified_val);
        return true;
    }
}

bool sjtu::processor::task_buy_ticket()  {
    // input data cache.
    int user_id;
    short num;
    char train_id[ID_SIZE];
    char station1[LOCATION_SIZE];
    char station2[LOCATION_SIZE];
    short date_num;
    char seat_kind[TICKET_KIND_SIZE]; // correspond to seat catalog.

    // other data.
    train_info_val train_info;
    char date_str[DATE_SIZE];
    char station_idx1, station_idx2, seat_idx;

    // read in data from stdin.
    scanf("%d %hd %s %s %s %s %s", &user_id, &num, train_id, station1, station2, date_str, seat_kind);

    // if train doesn't exit or not on sale.
    if(!get_train_info(train_id, train_info)) {
        return false;
    }
    else {
        // if get indices fails.
        if(!get_train_station_indices(train_info, station1, station2, station_idx1, station_idx2) ||
           !get_train_seat_catalog_index(train_info, seat_kind, seat_idx)) {
            return false;
        }
        // modify date_num to match the state date of head station.
        modify_date(date_str, - train_info.stations[station_idx1].date_offset);

        // check ticket number left to determine whether train tickets are sufficient.
        ticket_key low_key, high_key;
        ticket_val tickets_val[train_info.station_num];
        ticket_key tickets_key[train_info.station_num];
        int range_get_len;
        short ticket_sold[train_info.station_num][5];

        date_num = (short) date_to_int(date_str);
        get_low_ticket_key(train_id, date_num, low_key);
        get_high_ticket_key(train_id, date_num, high_key);

        ticket_db->get_range(low_key, high_key, tickets_val, range_get_len, tickets_key, true);

        // if this train today hasn't been bought, just create, check, and insert it.
        get_ticket_sold(train_info, tickets_key, tickets_val, range_get_len, ticket_sold);
        /*
        for(int i = 0; i < train_info.station_num; ++i) {
            for(int j = 0; j < train_info.seat_catalog_num; ++j) {
                std::cout << ticket_sold[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        */
        // if tickets aren't enough.
        if(!check_sufficient(station_idx1, station_idx2, seat_idx, ticket_sold, num)) {
            return false;
        }
        else {
            // buy ticket.
            for(char i = station_idx1; i < station_idx2; ++i) {
                ticket_sold[i][seat_idx] += num;
            }
            // if ticket_key and val hasn't been inserted, insert it, otherwise modify it.
            ticket_key insert_key(train_id, date_num);
            ticket_val im_val;
            // use a table to record inserted station index.
            memset(ticket_idx_table, false, sizeof(ticket_idx_table));
            char index;
            for(int i = 0; i < range_get_len; ++i) {
                index = tickets_key[i].station_idx;
                ticket_idx_table[index] = true;
            }
            // insert ? modify ? note that between station_idx1 and station_idx2.
            for(char i = station_idx1; i < station_idx2; ++i) {
                // exist, modify
                if(ticket_idx_table[i]) {
                    insert_key.station_idx = i;
                    for(char j = 0; j < train_info.seat_catalog_num; ++j) {
                        im_val.ticket_sold[j] = ticket_sold[i][j];
                    }
                    ticket_db->modify(insert_key, im_val);
                }
                // no exist, insert.
                else {
                    insert_key.station_idx = i;
                    for(char j = 0; j < train_info.seat_catalog_num; ++j) {
                        im_val.ticket_sold[j] = ticket_sold[i][j];
                    }
                    ticket_db->insert(insert_key, im_val);
                }
            }
            // after buying ticket, a record of transaction needs to be inserted or modified.
            // in bill_db, we use original date.
            modify_date(date_str, train_info.stations[station_idx1].date_offset);
            date_num = (short) date_to_int(date_str);

            bill_key trans_key(user_id, date_num, train_info.train_catalog, train_id, station_idx1, station_idx2);
            bill_val trans_val;
            trans_val = bill_db->find(trans_key);
            // create a trans_val.
            if(trans_val.null_obj()) {
                memset(trans_val.ticket_bought, 0, sizeof(trans_val.ticket_bought));
                trans_val.ticket_bought[seat_idx] = num;
                bill_db->insert(trans_key, trans_val);
            }
            // modify the trans_val.
            else {
                trans_val.ticket_bought[seat_idx] += num;
                bill_db->modify(trans_key, trans_val);
            }
            return true;
        }
    }
}

bool sjtu::processor::task_clean() {
    // 如果已经删库了，你还删个猴子。
    if (clean)
        return false;
    else {
        close_file();
        unlink("zuser_db.txt");
        unlink("ztrain_id_db.txt");
        unlink("ztrain_pile.txt");
        unlink("zstation_db.txt");
        unlink("zconst.txt");
        unlink("zticket_db.txt");
        unlink("zbill_db.txt");
        clean = true;
        return true;
    }
}

bool sjtu::processor::task_query_bill() {
    int user_id;
    char date[DATE_SIZE];
    char train_cat[CATALOG_SIZE];
    size_t cat_num;

    bill_key bills_key[10];
    bill_val bills_val[10];
    int bill_len = 0;
    short num_date;

    scanf("%d %s %s", &user_id, date, train_cat);
    cat_num = strlen(train_cat);
    num_date = (short) date_to_int(date);

    // query different catalog.
    for(int i = 0; i < cat_num; ++i) {
        query_bill_per_catalog(user_id, num_date, train_cat[i], bills_val, bill_len, bills_key);
    }
    // if data doesn't exist.
    if (bill_len == 0)
        return false;
    else {
        // output.
        printf("%d\n", bill_len);
        // 对每一个购票信息进行输出。
        for(int i = 0; i < bill_len; ++i) {
            print_bill(bills_key[i], bills_val[i]);
        }
        return true;
    }
}

bool sjtu::processor::task_query_ticket()  {
    char station1[NAME_SIZE];
    char station2[NAME_SIZE];
    char date[DATE_SIZE];
    char train_cat[CATALOG_SIZE];
    ticket_wrap ret[132];
    int len_ret;
    size_t cat_len;

    scanf("%s %s %s %s", station1, station2, date, train_cat);
    cat_len = strlen(train_cat);
    len_ret = 0;

    for(int i = 0; i < cat_len; ++i) {
        query_ticket_per_catalog(station1, station2, date, train_cat[i], ret, len_ret);
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
 * sub-function used by task_query_ticket.
 * append after len directly. */
void sjtu::processor::query_ticket_per_catalog(const char *station1, const char *station2, const char *date,
                                               const char &train_cat, sjtu::ticket_wrap *wrap, int &len)  {
    /// wait for tuning.
    station_val station_val1[1000];
    station_val station_val2[1000];
    int station_len1;
    int station_len2;

    // get all trains passing location_1.
    get_trains_pass_loc(station1, train_cat, station_val1, station_len1);
    // get all trains passing location_2.
    get_trains_pass_loc(station2, train_cat, station_val2, station_len2);

    // because the trains aligned in order of their id. check out all suitable ones.
    int cur1, cur2;
    cur1 = cur2 = 0;

    // 什么时候停止查找。
    while (cur1 < station_len1 && cur2 < station_len2) {
        // 进行一次改变。
        // 如果正好找到出入站相等的咯！
        int cmp = strcmp(station_val1[cur1].train_id, station_val2[cur2].train_id);
        if (cmp == 0) {
            // 判断这个是否合适，加入wrap，否则直接掠过+1。
            // if station1 is ahead of station2.
            if (station_val1[cur1].station_idx < station_val2[cur2].station_idx) {
                train_info_val train_info;
                char station_idx1;
                char station_idx2;
                short ticket_left[5];
                char date_head[11];

                // 找到train_info
                get_train_info(station_val1[cur1].train_id, train_info);
                // 根据车次信息，找到首末站的index。if invalid loc name, return false.
                /// debug code
                if (!get_train_station_indices(train_info, station1, station2, station_idx1, station_idx2)) {
                    std::cerr << "query_ticket_per_catalog error" << std::endl;
                    return;
                }
                // 我们的query给出了出发日期，但是要找已售车票的话，就要把出发日期推到始发站的出发日期。
                strcpy(date_head, date);
                modify_date(date_head, -train_info.stations[station_idx1].date_offset);

                get_ticket_left(train_info, station_idx1, station_idx2, date_head, ticket_left);

                // get ticket left along the lane.
                bool skip = true;
                for(char kind = 0; kind < train_info.seat_catalog_num; ++kind) {
                    if(ticket_left[kind] > 0) {
                        skip = false;
                        break;
                    }
                }

                // 看看这个票是不是全卖光了？如果是，那么我们要skip。
                if (skip)
                    return;
                else {
                    fill_query_ticket(train_info, station_idx1, station_idx2, date_head, ticket_left, wrap[len]);
                    ++len;
                }
            }
            ++cur1, ++cur2;
        }
        else if (cmp < 0) {
            ++cur1;
        }
        else {
            ++cur2;
        }
    }
}


bool sjtu::processor::task_refund_ticket()  {
    int user_id;
    int refund_num;
    char train_id[ID_SIZE];
    char station1[NAME_SIZE];
    char station2[NAME_SIZE];
    char date[DATE_SIZE];
    char seat_kind[TICKET_KIND_SIZE];
    short num_date;
    char station_idx1;
    char station_idx2;
    char seat_cat_idx;
    train_info_val train_info;

    scanf("%d %d %s %s %s %s %s", &user_id, &refund_num, train_id, station1, station2, date, seat_kind);
    // date hasn't been changed in bill.
    num_date = (short) date_to_int(date);

    // 额外查询车辆信息来确定 catalog和loc_idx.
    get_train_info(train_id, train_info);

    // prevent invalid query.
    if(!get_train_station_indices(train_info, station1, station2, station_idx1, station_idx2)) {
        return false;
    }
    get_train_seat_catalog_index(train_info, seat_kind, seat_cat_idx);

    bill_key b_key(user_id, num_date, train_info.train_catalog, train_id, station_idx1, station_idx2);
    // 查询交易信息。
    bill_val b_val = bill_db->find(b_key);

    // 如果交易信息根本他妈就不存在。
    if(b_val.null_obj()) {
        return false;
    }
    else {
        // 如果不够退，你还玩个猴子。
        if(b_val.ticket_bought[seat_cat_idx] < refund_num)
            return false;
        else {
            // 决定了，我要退票。
            b_val.ticket_bought[seat_cat_idx] -= refund_num;
            // 看看这个订单会不会变得trivial。
            bool trivial = true;
            for(int i = 0; i < train_info.seat_catalog_num; ++i) {
                if(b_val.ticket_bought[i] > 0) {
                    trivial = false;
                    break;
                }
            }
            // 如果这个订单变得trivial，那就删掉它；否则修改它。
            if(trivial) {
                bill_db->erase(b_key);
            }
            else {
                bill_db->modify(b_key, b_val);
            }
            // 然后我们要在票堆里面也进行删除。所幸在ticket_db当中必定存在购票信息。
            ticket_key low_key, high_key, modify_key(train_id, num_date);

            get_low_ticket_key(train_id, num_date, low_key);
            get_high_ticket_key(train_id, num_date, high_key);

            // 根据这个车过了多少站，定义车票信息的数组。
            ticket_key t_key[train_info.station_num];
            ticket_val t_val[train_info.station_num];
            int station_len;    // 没必要的参数，它必定等于train_info.station_num.

            ticket_db->get_range(low_key, high_key, t_val, station_len, t_key, true);

            // find the first station's index in t_key and t_val
            int start_idx = -1;
            for(int i = 0; i < train_info.station_num; ++i) {
                if(t_key[i].station_idx == station_idx1) {
                    start_idx = i;
                    break;
                }
            }
            /// debug code
            if(start_idx == -1) {
                std::cerr << "task_refund_ticket error" << std::endl;
            }

            for(int i = 0; i < station_idx2 - station_idx1; ++i) {
                // 在每个station都减掉相应的退票数。因为已经被买掉的票变少了。
                t_val[start_idx + i].ticket_sold[seat_cat_idx] -= refund_num;
                // 修改回去。
                modify_key.station_idx = (char) (start_idx + i);
                ticket_db->modify(modify_key, t_val[i]);
            }
            return true;
        }
    }
}

bool sjtu::processor::task_query_transfer() {
    char loc1[LOCATION_SIZE];   // 出发地
    char loc2[LOCATION_SIZE];   // 到达地
    char date[DATE_SIZE];       // 当前日期
    char catalog[CATALOG_SIZE]; // 所有要查询的车次种类
    size_t cat_len;             // 车次种类数量
    station_val loc2_v[1000];       // 所有经过到达地的简易车辆信息
    int len2;
    train_info_val train_info2;          // 换乘后的车辆信息


    int min_time = INT_MAX;     // 最短时间
    ticket_wrap min_ret1;        // 最快车辆信息1
    ticket_wrap min_ret2;        // 最快车辆信息2

    scanf("%s %s %s %s", loc1, loc2, date, catalog);
    cat_len = strlen(catalog);

    // 遍历所有车辆种类
    for(int l = 0; l < cat_len; ++l) {
        get_trains_pass_loc(loc2, catalog[l], loc2_v, len2); // 拿到loc2_v

        // 遍历所有到达loc2的车子。
        for (int i = 0; i < len2; ++i) {
            // 如果loc2是起始车站，那么放弃它。
            if (loc2_v[i].station_idx == 0)
                continue;

            get_train_info(loc2_v[i].train_id, train_info2);    // 取得当前车辆的详细信息。
            // 遍历当前换乘后的第二辆车的所有前面的车站，也就是寻找换乘车站。
            char station_idx = 0;
            while(strcmp(train_info2.stations[station_idx].station_name, loc2) != 0)
                station_idx++;

            for (char j = 0; j < station_idx; ++j) {
                // 先看看票还剩多少。
                ticket_key t_key[train_info2.station_num];
                ticket_val t_val[train_info2.station_num];
                int station_len;
                short ticket_left[5];

                // here station_idx1 = j, station_idx2 = station_idx.
                char &station_idx1 = j;
                char &station_idx2 = station_idx;

                char date_head[11];
                strcpy(date_head, date);
                modify_date(date_head, - train_info2.stations[station_idx1].date_offset);

                get_ticket_left(train_info2, station_idx1, station_idx2, date_head, ticket_left);

                // 如果票全部卖完了，直接跳过把。
                bool skip = true;
                for(int t = 0; t < train_info2.seat_catalog_num; ++t) {
                    if(ticket_left[t] > 0) {
                        skip = false;
                        break;
                    }
                }
                if(skip)
                    continue;
                // 我们继续查找。

                ticket_wrap ticketRet1[132];     // 找到从出发地到换乘车站的所有车票
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
                            min_ret2.seat_num[t] = ticket_left[t];
                            min_ret2.seat_price[t] = price[t];
                        }
                        min_ret2.num_catalog = train_info2.seat_catalog_num;
                    }
                }

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

/**
 * private and utility function:
 * Note: all functions are dangerous for efficiency. */

/**
 * get train_info using train_id. if train_info doesn't exist or hasn't been on sale, return false to indicate that. */
bool sjtu::processor::get_train_info(const char *train_id, sjtu::train_info_val &train_info) {
    train_id_val id_val;
    id_val = train_id_db->find(train_id);

// if the train doesn't exist or it hasn't been on sale, return false.
    // otherwise continue.
    if (id_val.null_obj() || !id_val.sale)
        return false;
    else {
        train_info_db->get_train_by_offset(id_val.address, train_info);
        return true;
    }
}
bool sjtu::processor::get_train_station_indices(const sjtu::train_info_val &train_info, const char *station1,
                                                const char *station2, char &station_idx1, char &station_idx2) {
    // find all indices, note that station1 must be ahead of station2.
    station_idx1 = station_idx2 = -1;
    for (char i = 0; i < train_info.station_num; ++i) {
        if (station_idx1 == -1) {
            if (strcmp(station1, train_info.stations[i].station_name) == 0) {
                station_idx1 = i;
            }
        } else if (station_idx2 == -1) {
            if (strcmp(station2, train_info.stations[i].station_name) == 0) {
                station_idx2 = i;
            }
        } else
            break;
    }
#ifdef DEBUG
    if (station_idx1 == -1 || station_idx2 == -1) {
        // std::cerr << "get_train_station_indices error" << std::endl;
        return false;
    }
#endif
    return true;
}
bool sjtu::processor::get_train_seat_catalog_index(const sjtu::train_info_val &train_info, const char *seat_kind,
                                                   char &seat_idx) {
    seat_idx = -1;
    for (char i = 0; i < train_info.seat_catalog_num; ++i) {
        if (strcmp(seat_kind, train_info.seat_catalog[i]) == 0)
            seat_idx = i;
    }
#ifdef DEBUG
    if (seat_idx == -1) {
        std::cerr << "get_train_seat_catalog_index" << std::endl;
        return false;
    }
#endif
    return true;
}
/**
 * get the ticket sold for a particular train on a particular day along all stations passed by this train.
 * given tickets_key and tickets_val. */
void sjtu::processor::get_ticket_sold(const sjtu::train_info_val &train_info, sjtu::ticket_key *tickets_key,
                                      sjtu::ticket_val *tickets_val, const int &len, short ticket_sold[][5]) {
    /// 还可以卡一卡。
    // initialize sold tickets.
    for(char i = 0; i < train_info.station_num; ++i) {
        for(int j = 0; j < 5; ++j) {
            ticket_sold[i][j] = 0;
        }
    }

    // consider the influence by ticket_db.

    // traverse existing tickets_val and key.
    for(int i = 0; i < len; ++i) {
        char &station_idx = tickets_key[i].station_idx;
        for(int j = 0; j < train_info.seat_catalog_num; ++j) {
            ticket_sold[station_idx][j] += tickets_val[i].ticket_sold[j];
        }
    }
}



bool sjtu::processor::check_sufficient(const char &station_idx1, const char &station_idx2, const char &seat_kind_idx,
                                       short ticket_sold[][5], const short &bought_num) {
    short max_sold = -1;
    for(char i = station_idx1; i < station_idx2; ++i) {
        if(max_sold < ticket_sold[i][seat_kind_idx]) {
            max_sold = ticket_sold[i][seat_kind_idx];
        }
    }
    if(max_sold + bought_num > 2000)
        return false;
    else
        return true;
}

void sjtu::processor::query_bill_per_catalog(const int &user_id, const short &num_date, const char &train_cat,
                                              sjtu::bill_val *bills_val, int &len, sjtu::bill_key *bills_key) {
    int add_len;

    bill_key low_key = get_low_bill_key(user_id, num_date, train_cat);
    bill_key high_key = get_high_bill_key(user_id, num_date, train_cat);
    bill_db->get_range(low_key, high_key, bills_val + len, add_len, bills_key + len, true);
    len += add_len;
}

/**
 * print log without hold it in a struct. */
void sjtu::processor::print_bill(const sjtu::bill_key &b_key, const sjtu::bill_val &b_val) {
    train_info_val train_info;
    get_train_info(b_key.train_id, train_info);
    char date_str[11];
    char time_str[6];
    char date_offset;

    // train_id
    printf("%s ", b_key.train_id);

    // start station
    printf("%s ", train_info.stations[b_key.station_idx1].station_name);
    // start date
    date_to_str(b_key.date, date_str);
    printf("%s ", date_str);
    // start time
    time_to_str(train_info.stations[b_key.station_idx1].start, time_str);
    printf("%s ", time_str);

    // terminal station
    printf("%s ", train_info.stations[b_key.station_idx2].station_name);
    // arrive date
    date_offset = train_info.stations[b_key.station_idx2].date_offset - train_info.stations[b_key.station_idx1].date_offset;
    date_to_str(b_key.date + date_offset, date_str);
    printf("%s ", date_str);
    // arrive time
    time_to_str(train_info.stations[b_key.station_idx2].arrive, time_str);
    printf("%s ", time_str);

    // seat name, ticket ordered, price
    for(char i = 0; i < train_info.seat_catalog_num; ++i) {
        printf("%s %d ", train_info.seat_catalog[i], b_val.ticket_bought[i]);
        // sum up price
        double price = 0;
        for(char j = (char) (b_key.station_idx1 + 1); j <= b_key.station_idx2; ++j) {
            price += train_info.stations[j].price[i];
        }
        printf("%lf", price);

        // take care of the last empty space
        if(i != train_info.seat_catalog_num)
            printf(" ");
    }
    printf("\n");
}

void sjtu::processor::get_trains_pass_loc(const char *loc, const char &cat, station_val *station_vals, int &len)  {
    station_key low, high;

    low = get_low_station_key(loc, cat);
    high = get_high_station_key(loc, cat);
    station_db->get_range(low, high, station_vals, len);
}

void sjtu::processor::fill_query_ticket(const train_info_val &train_info, const int &station_idx1, const int &station_idx2,
                                        const char *date_head, const short *ticket_left, ticket_wrap &wrapper)  {
    char date[11];

    strcpy(wrapper.train_id, train_info.train_id);

    strcpy(wrapper.beg, train_info.stations[station_idx1].station_name);
    /// note! we need to change date.
    strcpy(date, date_head);
    modify_date(date, train_info.stations[station_idx1].date_offset);
    strcpy(wrapper.beg_date, date);
    time_to_str(train_info.stations[station_idx1].start, wrapper.beg_time);

    strcpy(wrapper.tail, train_info.stations[station_idx2].station_name);
    /// note! we need to change date.
    strcpy(date, date_head);
    modify_date(date, train_info.stations[station_idx2].date_offset);
    strcpy(wrapper.beg_date, date);
    time_to_str(train_info.stations[station_idx2].arrive, wrapper.tail_time);

    wrapper.num_catalog = train_info.seat_catalog_num;

    // 对第j种座位统计价格。
    for (int j = 0; j < wrapper.num_catalog; ++j) {
        // 复制座位名称。
        strcpy(wrapper.seat_cat[j], train_info.seat_catalog[j]);
        // 对价格求和。
        double price = 0;
        for (int k = station_idx1 + 1; k <= station_idx2; ++k)
            price += train_info.stations[k].price[j];
        // 输入wrapper。
        wrapper.seat_price[j] = price;
        wrapper.seat_num[j] = ticket_left[j];
    }
}

/**
 * given a train and a head_date, as well as start and terminal stations. return the ticket_left to be bought for each seat catalog */
void sjtu::processor::get_ticket_left(const sjtu::train_info_val &train_info, const char &station_idx1,
                                      const char &station_idx2, const char *date_head, short *ticket_left) {
    ticket_key low_key, high_key;
    ticket_val tickets_val[train_info.station_num];
    ticket_key tickets_key[train_info.station_num];
    int range_get_len;
    short ticket_sold[train_info.station_num][5];
    short date_num;

    date_num = (short) date_to_int(date_head);
    get_low_ticket_key(train_info.train_id, date_num, low_key);
    get_high_ticket_key(train_info.train_id, date_num, high_key);

    ticket_db->get_range(low_key, high_key, tickets_val, range_get_len, tickets_key, true);

    // if this train today hasn't been bought, just create, check, and insert it.
    get_ticket_sold(train_info, tickets_key, tickets_val, range_get_len, ticket_sold);

    /*
    for(int i = 0; i < train_info.station_num; ++i) {
        for(int j = 0; j < train_info.seat_catalog_num; ++j) {
            std::cout << ticket_sold[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    */
    // get ticket left.
    for (char kind = 0; kind < train_info.seat_catalog_num; ++kind) {
        ticket_left[kind] = 2000;
        for (char i = station_idx1; i < station_idx2; ++i) {
            if (ticket_left[kind] > 2000 - ticket_sold[i][kind]) {
                ticket_left[kind] = 2000 - ticket_sold[i][kind];
            }
        }
    }
}