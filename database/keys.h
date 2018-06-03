#ifndef MYYMT_KEYS_H
#define MYYMT_KEYS_H

#include "constant.h"
#include <cstring>
#include <iostream>
#include "limits.h"

namespace sjtu {
    /** this file defines all necessary keys. */

    typedef int user_key;

    /**
     * when train_id is used as key, it's just char[ID_SIZE].
     * we explicitly define it to be a class to use template. */
    class train_id_key {
    public:
        char key[ID_SIZE];

        train_id_key() {}
        train_id_key(const char *other) {
            strcpy(key, other);
        }
        /// overload huge amount of functions.
        bool operator<(const train_id_key &other) {
            return strcmp(key, other.key) < 0;
        }
        bool operator<=(const train_id_key &other) {
            return strcmp(key, other.key) <= 0;
        }
        bool operator!=(const train_id_key &other) {
            return strcmp(key, other.key) != 0;
        }
        bool operator==(const train_id_key &other) {
            return strcmp(key, other.key) == 0;
        }

        train_id_key &operator=(const train_id_key &other) {
            strcpy(key, other.key);
            return *this;
        }
    };

    std::ostream &operator<< (std::ostream &os, const train_id_key &obj) {
        os << obj.key;
        return os;
    }

    /**
     * B+ tree returns an object constructed in default to show asked value
     * doesn't exist. So when I wanna store offset which is effectively an
     * integer, I have to make it a class with designed default constructor.
     *
     * also, it has to record whether a train has opened for sale. */
    class train_id_val {
    public:
        int address;
        bool sale;
    public:
        train_id_val() {
            address = -1;
            sale = false;
        }
        train_id_val(const int val) {
            address = val;
            sale = false;
        }
        train_id_val(const train_id_val &other) {
            address = other.address;
            sale = other.sale;
        }

        train_id_val &operator=(const int &val) {
            address = val;
            return *this;
        }
        train_id_val &operator=(const train_id_val &other) {
            if(this == &other)
                return *this;
            address = other.address;
            sale = other.sale;
            return *this;
        }
        /// return whether this object is null object, which means it's constructed in default.
        bool null_obj() {
            return address == -1;
        }
    };
    std::ostream &operator<<(std::ostream &os, const train_id_val &obj) {
        os << obj.address;
        return os;
    }

    /**
     * 在每一个车站，每一个train_id只会出现一次。
     * key and value classes for loc database.
     * key: location, catalog, train_id.
     * value: train_id, station index, whether terminal. */
    class loc_key {
    public:
        char location[LOCATION_SIZE];
        char train_cat;         // train's catalog.
        char train_id[ID_SIZE];

        loc_key() {
            memset(location, 0, sizeof(location));
            train_cat = 0;
            memset(train_id, 0, sizeof(train_id));
        }
        loc_key(const char *loc, const char catalog, const char *id = nullptr) {
            memset(location, 0, sizeof(location));
            train_cat = 0;
            memset(train_id, 0, sizeof(train_id));

            strcpy(location, loc);
            train_cat = catalog;
            if(id != nullptr)
                strcpy(train_id, id);
        }

        loc_key &operator=(const loc_key &other) {
            strcpy(location, other.location);
            train_cat = other.train_cat;
            strcpy(train_id, other.train_id);
            return *this;
        }

        /// overload huge amount of functions.
        bool operator<(const loc_key &other) {
            int cmp;

            cmp = cstrcmp(location, other.location);
            if (cmp < 0)
                return true;
            else if (cmp > 0)
                return false;
            else {
                cmp = train_cat - other.train_cat;
                if (cmp < 0)
                    return true;
                else if (cmp > 0)
                    return false;
                else {
                    cmp = strcmp(train_id, other.train_id);
                    if (cmp < 0)
                        return true;
                    else if (cmp > 0)
                        return false;
                    else
                        return false;  // equal isn't <.
                }
            }
        }

        bool operator==(const loc_key &other) {
            return cstrcmp(location, other.location) == 0 &&
                   train_cat == other.train_cat &&
                   strcmp(train_id, other.train_id) == 0;
        }

        /// slightly modify operator<.
        bool operator<=(const loc_key &other) {
            int cmp;

            cmp = cstrcmp(location, other.location);
            if (cmp < 0)
                return true;
            else if (cmp > 0)
                return false;
            else {
                cmp = train_cat - other.train_cat;
                if (cmp < 0)
                    return true;
                else if (cmp > 0)
                    return false;
                else {
                    cmp = strcmp(train_id, other.train_id);
                    if (cmp < 0)
                        return true;
                    else if (cmp > 0)
                        return false;
                    else
                        return true;  // equal is <=.
                }
            }
        }

        bool operator!=(const loc_key &other) {
            return !(*this == other);
        }
    };

    std::ostream &operator<< (std::ostream &os, const loc_key &val) {
        os << val.location << ' ' << val.train_cat << ' ' << val.train_id;
        return os;
    }
    class loc_val {
    public:
        char train_id[ID_SIZE];
        int station_idx;
        bool terminal;

        loc_val() {
            station_idx = -1;
        }

        loc_val(char *id, int idx, bool t) {
            strcpy(train_id, id);
            station_idx = idx;
            terminal = t;
        }

        loc_val &operator= (const loc_val &other) {
            strcpy(train_id, other.train_id);
            station_idx = other.station_idx;
            terminal = other.terminal;
            return *this;
        }

        bool null_obj() {
            return station_idx == -1;
        }
    };
    loc_key get_low_loc_key(const char *loc, const char train_cat) {
        loc_key ret(loc, train_cat);
        ret.train_id[0] = '\0';
        return ret;
    }
    loc_key get_high_loc_key(const char *loc, const char train_cat) {
        loc_key ret(loc, train_cat);
        ret.train_id[0] = 127;
        ret.train_id[1] = '\0';
        return ret;
    }

    std::ostream &operator<< (std::ostream &os, const loc_val &val) {
        os << val.train_id << ' ' << val.station_idx;
        return os;
    }
    struct ticket_ret {
        char train_id[ID_SIZE];

        char beg[NAME_SIZE];
        char beg_date[DATE_SIZE];
        char beg_time[10];

        char tail[NAME_SIZE];
        char tail_date[DATE_SIZE];
        char tail_time[10];

        int num_catalog;
        char seat_cat[5][TICKET_KIND_SIZE];
        double seat_price[5];
        int seat_num_left[5];

        void view() {
            printf("%s ", train_id);
            printf("%s %s %s ", beg, beg_date, beg_time);
            printf("%s %s %s ", tail, tail_date, tail_time);
            for(int i = 0; i < num_catalog; ++i) {
		// printf("%s %d ￥", seat_cat[i], seat_num_left[i]);
printf("%s %d ", seat_cat[i], seat_num_left[i]);
		print_double(seat_price[i]);
		if(i != num_catalog - 1) // so ugly
			printf(" ");
}
            printf("\n");
        }

        ticket_ret &operator= (const ticket_ret &other) {
            strcpy(train_id, other.train_id);

            strcpy(beg, other.beg);
            strcpy(beg_date, other.beg_date);
            strcpy(beg_time, other.beg_time);

            strcpy(tail, other.tail);
            strcpy(tail_date, other.tail_date);
            strcpy(tail_time, other.tail_time);

            num_catalog = other.num_catalog;
            for(int i = 0; i < 5; ++i) {
                strcpy(seat_cat[i], other.seat_cat[i]);
                seat_price[i] = other.seat_price[i];
                seat_num_left[i] = other.seat_num_left[i];
            }
            return *this;
        }
    };
    /**
     * key and value classes for ticket database.
     * key: train_id, date, location.
     * value: seat_catalog_num, ticket sold. */
    class ticket_key {
    public:
        char train_id[ID_SIZE];
        int date;
        int loc_idx;

        ticket_key() {}
        ticket_key(const char *id, const int d, const int &location_idx = -1) {
            strcpy(train_id, id);
            date = d;
            loc_idx = location_idx;
        }

        ticket_key &operator=(const ticket_key &other) {

            for(int i = 0; i < ID_SIZE; ++i)
                train_id[i] = other.train_id[i];

            date = other.date;

            loc_idx = other.loc_idx;
            return *this;
        }

        /// overload huge amount of functions.
        bool operator<(const ticket_key &other) {
            int cmp;

            cmp = strcmp(train_id, other.train_id);
            if (cmp < 0)
                return true;
            else if (cmp > 0)
                return false;
            else {
                cmp = date - other.date;
                if (cmp < 0)
                    return true;
                else if (cmp > 0)
                    return false;
                else {
                    cmp = loc_idx - other.loc_idx;
                    if (cmp < 0)
                        return true;
                    else if (cmp > 0)
                        return false;
                    else
                        return false;  // equal isn't <.
                }
            }
        }

        bool operator==(const ticket_key &other) {
            return  strcmp(train_id, other.train_id) == 0 &&
                   date == other.date &&
                    loc_idx == other.loc_idx;
        }

        /// slightly modify operator<.
        bool operator<=(const ticket_key &other) {
            int cmp;

            cmp = strcmp(train_id, other.train_id);
            if (cmp < 0)
                return true;
            else if (cmp > 0)
                return false;
            else {
                cmp = date - other.date;
                if (cmp < 0)
                    return true;
                else if (cmp > 0)
                    return false;
                else {
                    cmp = loc_idx - other.loc_idx;
                    if (cmp < 0)
                        return true;
                    else if (cmp > 0)
                        return false;
                    else
                        return true;  // equal is <=.
                }
            }
        }

        bool operator!=(const ticket_key &other) {
            return !(*this == other);
        }
    };

    std::ostream &operator<< (std::ostream &os, const ticket_key &val) {
        os << val.train_id << ' ' << val.loc_idx << ' ' << val.date;
        return os;
    }


    ticket_key get_low_ticket_key(const char *id, const int d) {
        ticket_key ret;
        strcpy(ret.train_id, id);
        ret.date = d;
        ret.loc_idx = -1;
        return ret;
    }
    ticket_key get_high_ticket_key(const char *id, const int d) {
        ticket_key ret;
        strcpy(ret.train_id, id);
        ret.date = d;
        ret.loc_idx = INT_MAX;
        return ret;
    }

    class ticket_val {
    public:
        int seat_cat_num;
        int ticket_sold[5];

        ticket_val() {
            seat_cat_num = -1;
            memset(ticket_sold, 0, sizeof(ticket_sold)); // initially, no ticket is sold.
        }
        ticket_val(int num, int t[]) {
            seat_cat_num = num;
            for(int i = 0; i < seat_cat_num; ++i)
                ticket_sold[i] = t[i];
        }

        ticket_val &operator=(const ticket_val &other) {
            seat_cat_num = other.seat_cat_num;
            for(int i = 0; i < 5; ++i)
                ticket_sold[i] = other.ticket_sold[i];
            return *this;
        }

        bool null_obj() {
            return seat_cat_num == -1;
        }
    };
    std::ostream &operator<< (std::ostream &os, const ticket_val &val) {
        for(int i = 0; i < val.seat_cat_num; ++i) {
            os << ' ' << val.ticket_sold[i];
        }
        return os;
    }


    /**
     * key and value classes for ticket query database.
     * key: user_id, date, catalog, train_id, loc1, loc2.
     * value: user_id, date, catalog, train_id, loc1, loc2.
     * 这个情况下所有的信息都可以被key所包含，现在还不想修改，所以key和value重复。*/
    struct order_key {
        int user_id;
        int date;
        char train_cat;
        char train_id[ID_SIZE];
        int loc1_idx;
        int loc2_idx;

        // set it to null is necessary because of memset.
        order_key() {}

        order_key(int u_id, const int &d, const char cat, const char *t_id = nullptr, const int loc1 = 0, const int loc2 = 0) {
            memset(this, 0, sizeof(order_key));
            user_id = u_id;
            // strcpy(user_id, u_id);
            date = d;
            train_cat = cat;
            if(t_id != nullptr)
                memcpy(train_id, t_id, sizeof(char) * ID_SIZE);
                // strcpy(train_id, t_id);
            loc1_idx = loc1;
            loc2_idx = loc2;
        }

        order_key &operator=(const order_key &other) {
            if(this == &other)
                return *this;
            memcpy(this, &other, sizeof(order_key));
            return *this;
        }
        /// test code.
        void view() {
            printf("%d\n%d\n%c\n%s\n%d %d\n", user_id, date, train_cat, train_id, loc1_idx, loc2_idx);
        }

        bool operator< (const order_key &other) {
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
                            cmp = loc1_idx - other.loc1_idx;
                            if(cmp < 0)
                                return true;
                            else if(cmp > 0)
                                return false;
                            else {
                                cmp = loc2_idx - other.loc2_idx;
                                if(cmp < 0)
                                    return true;
                                else if(cmp > 0)
                                    return false;
                                else
                                    return false;
                            }
                        }
                    }
                }
            }
        }

        bool operator<= (const order_key &other) {
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
                            cmp = loc1_idx - other.loc1_idx;
                            if(cmp < 0)
                                return true;
                            else if(cmp > 0)
                                return false;
                            else {
                                cmp = loc2_idx - other.loc2_idx;
                                if(cmp < 0)
                                    return true;
                                else if(cmp > 0)
                                    return false;
                                else
                                    return true;
                            }
                        }
                    }
                }
            }
        }

        bool operator== (const order_key &other) {
            return user_id - other.user_id == 0 &&
                   date == other.date &&
                   train_cat == other.train_cat &&
                   strcmp(train_id, other.train_id) == 0 &&
                   loc1_idx == other.loc1_idx &&
                   loc2_idx == other.loc2_idx;
        }

        bool operator!= (const order_key &other) {
            return !(*this == other);
        }
    };

    order_key get_low_order_key(int u_id, const int &d, const char cat) {
        order_key ret(u_id, d, cat);
        ret.train_id[0] = '\0';
        return ret;
    }
    order_key get_high_order_key(int u_id, const int &d, const char cat) {
        order_key ret(u_id, d, cat);
        ret.train_id[0] = 127;
        ret.train_id[1] = '\0';
        return ret;
    }


    class order_val {
    public:
        char train_id[ID_SIZE];
        int loc1_idx;
        int loc2_idx;
        int tickets[5];     // total tickets for different kinds of seats.

        order_val() {
            loc1_idx = -1;
            memset(tickets, 0, sizeof(tickets));
        }
        order_val(const char *t_id, const int &loc1, const int &loc2, const int cat_idx, const int ticket_num) {
            memset(tickets, 0, sizeof(tickets));
            strcpy(train_id, t_id);
            loc1_idx = loc1;
            loc2_idx = loc2;
            tickets[cat_idx] = ticket_num;
        }

        order_val &operator=(const order_val &other) {
            for(int i = 0; i < ID_SIZE; ++i)
                train_id[i] = other.train_id[i];
            loc1_idx = other.loc1_idx;
            loc2_idx = other.loc2_idx;
            for(int i = 0; i < 5; ++i)
                tickets[i] = other.tickets[i];
            return *this;
        }

        bool null_obj() {
            return loc1_idx == -1;
        }
    };

    /**
     * 查询车票时返回数据的wrapper */
     struct order_ret {
         char train_id[ID_SIZE];

         char beg[NAME_SIZE];
         char beg_date[DATE_SIZE];
         char beg_time[10];

         char tail[NAME_SIZE];
         char tail_date[DATE_SIZE];
         char tail_time[10];

         int num_catalog;
         char seat_cat[5][TICKET_KIND_SIZE];
         double seat_price[5];
         int seat_num[5];

         void view() {
             printf("%s ", train_id);
             printf("%s %s %s ", beg, beg_date, beg_time);
             printf("%s %s %s ", tail, tail_date, tail_time);
             for(int i = 0; i < num_catalog; ++i) {
printf("%s %d ", seat_cat[i], seat_num[i]);
print_double(seat_price[i]);
printf(" "); 
}
                
             printf("\n");
         }
     };

};

#endif //MYYMT_KEYS_H
