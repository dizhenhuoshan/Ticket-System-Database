#ifndef NEWDATABASE_BILL_KV_H
#define NEWDATABASE_BILL_KV_H

#include <cstring>
#include <cstdio>
#include "constant.h"
#include "utility.h"

namespace sjtu {
    /**
     * key and value classes for ticket query database.
     * key: user_id, date, catalog, train_id, loc1, loc2.
     * value: user_id, date, catalog, train_id, loc1, loc2.
     * 这个情况下所有的信息都可以被key所包含，现在还不想修改，所以key和value重复。*/
    class bill_key {
    public:
        int user_id;
        short date;
        char train_cat;
        char train_id[ID_SIZE];
        char station_idx1;
        char station_idx2;

        // set it to null is necessary because of memset.
        bill_key() {}

        bill_key(int u_id, const short &d, const char cat, const char *t_id = nullptr, const char &loc1 = -1, const char &loc2 = -1) {
            user_id = u_id;
            // strcpy(user_id, u_id);
            date = d;
            train_cat = cat;
            if(t_id != nullptr)
                memcpy(train_id, t_id, sizeof(char) * ID_SIZE);
            // strcpy(train_id, t_id);
            station_idx1 = loc1;
            station_idx2 = loc2;
        }

        bill_key &operator=(const bill_key &other) {
            if(this == &other)
                return *this;
            memcpy(this, &other, sizeof(bill_key));
            return *this;
        }
        /// test code.
        void view() {
            printf("%d\n%d\n%c\n%s\n%d %d\n", user_id, date, train_cat, train_id, station_idx1, station_idx2);
        }

        bool operator< (const bill_key &other) const {
            int cmp;
            cmp = user_id - other.user_id;
            if(cmp < 0)
                return true;
            else if(cmp > 0)
                return false;
            else {
                cmp = date - other.date;
                if(cmp < 0)
                    return true;
                else if(cmp > 0)
                    return false;
                else {
                    cmp = train_cat - other.train_cat;
                    if(cmp < 0)
                        return true;
                    else if(cmp > 0)
                        return false;
                    else {
                        cmp = strcmp(train_id, other.train_id);
                        if(cmp < 0)
                            return true;
                        else if(cmp > 0)
                            return false;
                        else {
                            cmp = station_idx1 - other.station_idx1;
                            if(cmp < 0)
                                return true;
                            else if(cmp > 0)
                                return false;
                            else {
                                return station_idx2 < other.station_idx2;
                            }
                        }
                    }
                }
            }
        }

        bool operator<= (const bill_key &other) const {
            int cmp;
            cmp = user_id - other.user_id;
            if(cmp < 0)
                return true;
            else if(cmp > 0)
                return false;
            else {
                cmp = date - other.date;
                if(cmp < 0)
                    return true;
                else if(cmp > 0)
                    return false;
                else {
                    cmp = train_cat - other.train_cat;
                    if(cmp < 0)
                        return true;
                    else if(cmp > 0)
                        return false;
                    else {
                        cmp = strcmp(train_id, other.train_id);
                        if(cmp < 0)
                            return true;
                        else if(cmp > 0)
                            return false;
                        else {
                            cmp = station_idx1 - other.station_idx1;
                            if(cmp < 0)
                                return true;
                            else if(cmp > 0)
                                return false;
                            else {
                                return station_idx2 <= other.station_idx2;
                            }
                        }
                    }
                }
            }
        }

        bool operator== (const bill_key &other) const {
            return user_id - other.user_id == 0 &&
                   date == other.date &&
                   train_cat == other.train_cat &&
                   strcmp(train_id, other.train_id) == 0 &&
                   station_idx1 == other.station_idx1 &&
                   station_idx2 == other.station_idx2;
        }

        bool operator!= (const bill_key &other) const {
            return !(*this == other);
        }
    };
    bill_key get_low_bill_key(int u_id, const short &d, const char &cat) {
        bill_key ret(u_id, d, cat);
        ret.train_id[0] = '\0';
        return ret;
    }
    bill_key get_high_bill_key(int u_id, const short &d, const char &cat) {
        bill_key ret(u_id, d, cat);
        ret.train_id[0] = 127;
        ret.train_id[1] = '\0';
        return ret;
    }

    class bill_val {
    public:
        short ticket_bought[5];

        bill_val() {
            ticket_bought[0] = -1;
        }
        bill_val(const short t[]) {
            for(int i = 0; i < 5; ++i)
                ticket_bought[i] = t[i];
        }

        bill_val &operator=(const bill_val &other) {
            for(int i = 0; i < 5; ++i)
                ticket_bought[i] = other.ticket_bought[i];
            return *this;
        }

        bool null_obj() {
            return ticket_bought[0] == -1;
        }
    };

    /**
     * 查询车票时返回数据的wrapper */
    struct ticket_wrap {
        char train_id[ID_SIZE];

        char beg[NAME_SIZE];
        char beg_date[DATE_SIZE];
        char beg_time[10];

        char tail[NAME_SIZE];
        char tail_date[DATE_SIZE];
        char tail_time[10];

        char num_catalog;
        char seat_cat[5][TICKET_KIND_SIZE];
        double seat_price[5];
        short seat_num[5];

        void view() {
            printf("%s ", train_id);
            printf("%s %s %s ", beg, beg_date, beg_time);
            printf("%s %s %s ", tail, tail_date, tail_time);
            for (int i = 0; i < num_catalog; ++i) {
                printf("%s %d ", seat_cat[i], seat_num[i]);
                print_double(seat_price[i]);
                if(i != num_catalog - 1)
                    printf(" ");
            }

            printf("\n");
        }
    };
};
#endif //NEWDATABASE_BILL_KV_H
