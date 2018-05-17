#include "keys.h"
#include "datas.h"
#include "BplusTree.h"

UniqueBPlusTree<User_id_Key, User_Data> user(user.db);
UniqueBPlusTree<Train_id_Key, Train_Data> train(train.db);
UniqueBPlusTree<Loc_Key, >
