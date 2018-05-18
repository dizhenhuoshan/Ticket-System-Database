#include <iostream>
#include "keys.h"
#include "datas.h"
#include "BplusTree.h"

namespace wymt
{
    //some const data, same as the data in datas.h
    const int maxn = 40;
    const int maxm = 21;
    const int maxc = 10;
    const int maxe = 5;

/*
 *  Main Database List:
 *  1. user --information of the user
 *          --Using key:: User_id_Key
 *          --Using data:: User_Data
 *
 *  2. train --information of the train except the station
 *           --Using key::Train_Key
 *           --Using data::Train_Data
 *
 *  3. ticket --ticket information of one train
 *            --Uisng key::Ticket_Key
 *            --Using data::Ticket_Data
 *
 *  4. station --information of the passed trains of a station
 *             --Using key::Station_Key
 *             --Using data::char  Detials: '?'--Not defined; 'S' --start station; 'P' --passed station; 'F' --final station
 *
 *  5. buyer --information of the bought ticket for a user
 *           --Using key::Buyer_Key
 *           --Using data::short   Detials: short for the num of the ticket
 */
sjtu::UniqueBPlusTree<key::User_id_Key, data::User_Data> user("user.db");
sjtu::UniqueBPlusTree<key::Train_Key, data::Train_Data> train("train.db");
sjtu::UniqueBPlusTree<key::Ticket_Key, data::Ticket_Data> ticket("ticket.db");
sjtu::UniqueBPlusTree<key::Station_Key, char> station("station.db");
sjtu::UniqueBPlusTree<key::Buyer_Key, short> buyer("buyer.db");

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
int param_reader(char order[])
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
