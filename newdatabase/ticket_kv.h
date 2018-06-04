#ifndef NEWDATABASE_TICKET_KV_H
#define NEWDATABASE_TICKET_KV_H

#include "constant.h"
#include <cstring>
#include <iostream>

namespace sjtu {
    /**
     * key and value classes for ticket database.
     * key: train_id, date, location.
     * value: seat_catalog_num, ticket sold. */
    class ticket_key {
    public:
        char train_id[ID_SIZE];
        short date;
        char station_idx;

        ticket_key() {}

        ticket_key(const char *id, const short &d, const char &si = -1) {
            strcpy(train_id, id);
            date = d;
            station_idx = si;
        }

        ticket_key &operator=(const ticket_key &other) {

            for (int i = 0; i < ID_SIZE; ++i)
                train_id[i] = other.train_id[i];

            date = other.date;

            station_idx = other.station_idx;
            return *this;
        }

        /// overload huge amount of functions.
        bool operator<(const ticket_key &other) const {
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
                    return station_idx < other.station_idx;
                }
            }
        }
        bool operator==(const ticket_key &other) const {
            return  strcmp(train_id, other.train_id) == 0 &&
                    date == other.date &&
                    station_idx == other.station_idx;
        }
        /// slightly modify operator<.
        bool operator<=(const ticket_key &other) const {
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
                    return station_idx <= other.station_idx;
                }
            }
        }
        bool operator!=(const ticket_key &other) const {
            return !(*this == other);
        }

    };
    std::ostream &operator<< (std::ostream &os, const ticket_key &val) {
        os << val.train_id << ' ' << val.station_idx << ' ' << val.date;
        return os;
    }
    void get_low_ticket_key(const char *id, const short &d, ticket_key &key) {
        strcpy(key.train_id, id);
        key.date = d;
        key.station_idx = -1;
    }
    void get_high_ticket_key(const char *id, const short &d, ticket_key &key) {
        strcpy(key.train_id, id);
        key.date = d;
        key.station_idx = 127;
    }

    /**
     * value type */
    class ticket_val {
    public:
        short ticket_sold[5];

        ticket_val() {
            ticket_sold[0] = -1;
        }
        ticket_val(short t[]) {
            for(int i = 0; i < 5; ++i)
                ticket_sold[i] = t[i];
        }

        ticket_val &operator=(const ticket_val &other) {
            for(int i = 0; i < 5; ++i)
                ticket_sold[i] = other.ticket_sold[i];
            return *this;
        }

        bool null_obj() {
            return ticket_sold[0] == -1;
        }
    };
    std::ostream &operator<< (std::ostream &os, const ticket_val &val) {
        for(int i = 0; i < 5; ++i) {
            os << ' ' << val.ticket_sold[i];
        }
        return os;
    }
};

#endif //NEWDATABASE_TICKET_KV_H
