#ifndef NEWDATABASE_TRAIN_ID_KV_H
#define NEWDATABASE_TRAIN_ID_KV_H

#include <iostream>
#include "train_info_kv.h"

namespace sjtu {

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

    /**
     * B+ tree returns an object constructed in default to show asked value
     * doesn't exist. So when I wanna store offset which is effectively an
     * integer, I have to make it a class with designed default constructor.
     *
     * also, it has to record whether a train has opened for sale. */
    class train_id_val {
    public:
        train_info_key address;
        bool sale;
    public:
        train_id_val() {
            address = -1;
            sale = false;
        }
        explicit train_id_val(const int val, bool s = false) {
            address = val;
            sale = s;
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
};

#endif //NEWDATABASE_TRAIN_ID_KV_H
