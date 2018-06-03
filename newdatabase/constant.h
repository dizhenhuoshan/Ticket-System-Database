#ifndef MYYMT_CONSTANT_H
#define MYYMT_CONSTANT_H

namespace sjtu {
    // basic constant.
    const int MAXTICKET = 2000;

    // data size info.
    const int NAME_SIZE = 41;
    const int PASSWORD_SIZE = 21;
    const int EMAIL_SIZE = 21;
    const int PHONE_SIZE = 21;
    const int ID_SIZE = 21;
    const int LOCATION_SIZE = 21;
    const int DATE_SIZE = 11;
    const int CATALOG_SIZE = 11;
    const int TICKET_KIND_SIZE = 21;
    const int TRAIN_ID_SIZE = 21;
    const int COMMAND_SIZE = 21;
    const int PRIVILEGE_SIZE = 4;
    const int START_USER_ID = 2018;

    // interpreter message info.
    typedef int command;
    const command NULLCOMMAND = 0;
    const command REGISTER = 1;
    const command QUERY_PROFILE = 2;
    const command LOGIN = 3;
    const command MODIFY_PROFILE = 4;
    const command MODIFY_PRIVILEGE = 5;
    const command ADD_TRAIN = 6;
    const command QUERY_TRAIN = 7;
    const command DELETE_TRAIN = 8;
    const command MODIFY_TRAIN = 9;
    const command SALE_TRAIN = 10;
    const command BUY_TICKET = 11;
    const command QUERY_ORDER = 12;
    const command QUERY_TICKET = 13;
    const command REFUND_TICKET = 14;
    const command CLEAN = 15;
    const command QUERY_TRANSFER = 16;
    const command EXIT = 17;

    // upper limits in allocation.
    const int MAXSEATSCATALOG = 5;     // this can be reduced to five.
    const int MAXSTATION = 60;

    // filename used by train_storer.
    char TRAIN_STORER_NAME[] = "train_pile.txt";
    const int PILE_HEAD_OFFSET = 4;     // used by pile_trainstorer to store tail offset.
};

#endif //MYYMT_CONSTANT_H
