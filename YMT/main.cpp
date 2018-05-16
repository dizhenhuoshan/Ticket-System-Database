#include "keys.h"
#include "datas.h"
#include "BplusTree.h"

UniqueBPlusTree<User_id_Key, User_Data> user(user.db);
UniqueBPlusTree<Train_id_Key, Train_Data> train(train.db);
BPlusTree<Train_id_Key, Loc_Key, Ticket_Data> ticket(ticket.db);
BPlusTree<User_id_Key, int, Buyer_Data> buyer()
