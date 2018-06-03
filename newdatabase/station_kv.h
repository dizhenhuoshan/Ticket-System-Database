#ifndef NEWDATABASE_STATION_KV_H
#define NEWDATABASE_STATION_KV_H

#include <cstring>
#include <iostream>

#include "constant.h"
#include "utility.h"

namespace sjtu {
    /**
     * 在每一个车站，每一个train_id只会出现一次。
     * key and value classes for loc database.
     * key: location, catalog, train_id.
     * value: train_id, station index, whether terminal. */
    class station_key {
    public:
        char location[LOCATION_SIZE];
        char train_cat;         // train's catalog.
        char train_id[ID_SIZE];

        station_key() {
            train_cat = 0;
        }
        station_key(const char *loc, const char catalog, const char *id = nullptr) {
            train_cat = 0;

            strcpy(location, loc);
            train_cat = catalog;
            if(id != nullptr)
                strcpy(train_id, id);
            else
                train_id[0] = '\0';
        }

        station_key &operator=(const station_key &other) {
            strcpy(location, other.location);
            train_cat = other.train_cat;
            strcpy(train_id, other.train_id);
            return *this;
        }

        /// overload huge amount of functions.
        bool operator<(const station_key &other) {
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

        bool operator==(const station_key &other) {
            return cstrcmp(location, other.location) == 0 &&
                   train_cat == other.train_cat &&
                   strcmp(train_id, other.train_id) == 0;
        }

        /// slightly modify operator<.
        bool operator<=(const station_key &other) {
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

        bool operator!=(const station_key &other) {
            return !(*this == other);
        }
    };
    std::ostream &operator<< (std::ostream &os, const station_key &val) {
        os << val.location << ' ' << val.train_cat << ' ' << val.train_id;
        return os;
    }

    station_key get_low_station_key(const char *loc, const char train_cat) {
        station_key ret(loc, train_cat);
        ret.train_id[0] = '\0';
        return ret;
    }
    station_key get_high_station_key(const char *loc, const char train_cat) {
        station_key ret(loc, train_cat);
        ret.train_id[0] = 127;
        ret.train_id[1] = '\0';
        return ret;
    }

    /**
     * station_db value and visualization. */
    class station_val {
    public:
        char train_id[ID_SIZE];
        char station_idx;
        bool terminal;

        station_val() {
            station_idx = -1;
        }

        station_val(char *id, int idx, bool t) {
            strcpy(train_id, id);
            station_idx = (char) idx;
            terminal = t;
        }

        station_val &operator= (const station_val &other) {
            strcpy(train_id, other.train_id);
            station_idx = other.station_idx;
            terminal = other.terminal;
            return *this;
        }

        bool null_obj() {
            return station_idx == -1;
        }
    };
    std::ostream &operator<< (std::ostream &os, const station_val &val) {
        os << val.train_id << ' ' << val.station_idx;
        return os;
    }
};

#endif //NEWDATABASE_STATION_KV_H
