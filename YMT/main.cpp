#include <iostream>
#include "keys.h"
#include "datas.h"
#include "BplusTree.h"

namespace sjtu
{
   //*analyse the orders*/
/*
*   Types:
*        Users:
*       11 -- register
*       12 -- login
*       13 -- query_profile
*       14 -- modify_profile
*       15 -- modify_privilege
*        Tickets:
*       21 -- query_ticket
*       22 -- query_transfer
*       23 -- buy_ticket
*       24 -- query_order
*       25 -- refund_ticket
*        Train:
*       31 -- add_train
*       32 -- sale_train
*       33 -- query_train
*       34 -- delete_train
*       35 -- modify_train
*        Managememt:
*       41 -- clean
*       42 -- exit
*/

int parser_reader(char order[])
{
    if (strcmp(order, "register") == 0)         return 11;
    if (strcmp(order, "login") == 0)            return 12;
    if (strcmp(order, "query_profile") == 0)    return 13;
    if (strcmp(order, "modify_profile") == 0)   return 14;
    if (strcmp(order, "modify_privilege") == 0) return 15;
    if (strcmp(order, "query_ticket") == 0)     return 21;
    if (strcmp(order, "query_transfer") == 0)   return 22;
    if (strcmp(order, "buy_ticket") == 0)       return 23;
    if (strcmp(order, "query_order") == 0)      return 24;
    if (strcmp(order, "refund_ticket") == 0)    return 25;
    if (strcmp(order, "add_train") == 0)        return 31;
    if (strcmp(order, "sale_train") == 0)       return 32;
    if (strcmp(order, "query_train") == 0)      return 33;
    if (strcmp(order, "delete_train") == 0)     return 34;
    if (strcmp(order, "modify_train") == 0)     return 35;
    if (strcmp(order, "clean") == 0)            return 41;
    if (strcmp(order, "exit") == 0)             return 42;
}

};
