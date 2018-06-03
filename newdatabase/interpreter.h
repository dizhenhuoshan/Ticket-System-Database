#ifndef MYYMT_INTERPRETER_H
#define MYYMT_INTERPRETER_H

#include "constant.h"
#include "utility.h"
#include <cstring>

namespace sjtu {

    /**
     * interpreter class is an organizer that reads in command, and assign command to different modules
     * to process. */
    class interpreter {
    public:
        interpreter() {}
        command parser() {
            char cmd[COMMAND_SIZE];

            // read command, terminate when space, newline, eof encountered.
            reader(cmd);

            // interpret different command.
            if(strcmp(cmd, "register") == 0)
                return REGISTER;

            else if(strcmp(cmd, "query_profile") == 0)
                return QUERY_PROFILE;

            else if(strcmp(cmd, "login") == 0)
                return LOGIN;

            else if(strcmp(cmd, "modify_profile") == 0)
                return MODIFY_PROFILE;

            else if(strcmp(cmd, "modify_privilege") == 0)
                return MODIFY_PRIVILEGE;

            else if(strcmp(cmd, "add_train") == 0)
                return ADD_TRAIN;

            else if(strcmp(cmd, "query_train") == 0)
                return QUERY_TRAIN;

            else if(strcmp(cmd, "delete_train") == 0)
                return DELETE_TRAIN;

            else if(strcmp(cmd, "modify_train") == 0)
                return MODIFY_TRAIN;

            else if(strcmp(cmd, "sale_train") == 0)
                return SALE_TRAIN;

            else if(strcmp(cmd, "buy_ticket") == 0)
                return BUY_TICKET;

            else if(strcmp(cmd, "query_order") == 0)
                return QUERY_ORDER;

            else if(strcmp(cmd, "query_ticket") == 0)
                return QUERY_TICKET;

            else if(strcmp(cmd, "refund_ticket") == 0)
                return REFUND_TICKET;

            else if(strcmp(cmd, "query_transfer") == 0)
                return QUERY_TRANSFER;

            else if(strcmp(cmd, "clean") == 0)
                return CLEAN;

            else if(strcmp(cmd, "exit") == 0)
                return EXIT;

            else
                return NULLCOMMAND;
        }
    };
};

#endif //MYYMT_INTERPRETER_H
