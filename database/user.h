#ifndef MYYMT_USER_H
#define MYYMT_USER_H

#include "constant.h"
#include "utility.h"
#include "keys.h"
#include <iostream>

/**
 * I'm rather confused about privilege.
 * privilege seems to be an integer, but which integer represents admin, what about common loser?
 * I current let privilege 0 mean loser, and non-zero mean admin. */
namespace sjtu {
    class user {
    public:

        user_key user_id;
        int privilege;
        char name[NAME_SIZE];
        char password[PASSWORD_SIZE];
        char email[EMAIL_SIZE];
        char phone[PHONE_SIZE];

    public:
        /// when fail to find, default constructed object is returned.
        user() {
            user_id = -1;
        }

        /// moving assign operator.

        user &operator=(const user &other) {
            user_id = other.user_id;
            privilege = other.privilege;
            strcpy(name, other.name);
            strcpy(password, other.password);
            strcpy(email, other.email);
            strcpy(phone, other.phone);
            return *this;
        }
        /**
         * read from std::cin. fill data field except user_id. */
        void read() {
            reader(name);
            reader(password);
            reader(email);
            reader(phone);
        }

        /// return whether this object is null object, which means it's constructed in default.
        bool null_obj() {
            return user_id == -1;
        }
    };
}

#endif //MYYMT_USER_H
