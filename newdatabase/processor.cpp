#include "processor.h"

int sjtu::processor::task_register() {
    user_key key;
    user_val val;

    // find user_id and privilege first.
    key = CR.append_id;
    CR.increment();

    if(key == 2018) {
        val.privilege = 2;
    }
    else {
        val.privilege = 1;
    }

    // read in user information.
    val.read();

    // 插入名册元素。
    user_db->insert(key, val);
    return key;
}
bool sjtu::processor::task_query_profile()  {
    user_key key;
    user_val val;

    scanf("%d", &key);
    val = user_db->find(key);

    // check whether found.
    if (val.null_obj()) {
        return false;
    }
    else {
        printf("%s %s %s %hd\n", val.name, val.email, val.phone, val.privilege);
        return true;
    }
}
bool sjtu::processor::task_login()  {
    user_key id;
    char pswd[PASSWORD_SIZE];
    user_val user_found;

    scanf("%d %s", &id, pswd);
    user_found = user_db->find(id);
    // printf("%s\n", user_found.password);
    // user not found or password not match.
    if (user_found.null_obj() || strcmp(pswd, user_found.password) != 0)
        return false;
    else
        return true;
}
bool sjtu::processor::task_modify_profile()  {
    user_key id;
    user_val ret;

    scanf("%d", &id);
    ret = user_db->find(id);
    // even if this query is invalid, we have to read a user obj to clear stdin stream.
    if (ret.null_obj()) {
        ret.read();
        return false;
    }
    else {
        ret.read();
        user_db->modify(id, ret);
        return true;
    }
}
bool sjtu::processor::task_modify_privilege()  {
    int id1, id2;
    short privilege;
    user_val user1, user2;

    scanf("%d %d %hd", &id1, &id2, &privilege);
    user1 = user_db->find(id1);
    user2 = user_db->find(id2);
    if(id1 == id2)
        return false;
    // if users exist.
    if (user1.null_obj() || user2.null_obj())
        return false;

    // simple user can't use this command.
    if (user1.privilege == 1)
        return false;
    // admin can't degenerate other admin to simple user.
    if (user2.privilege > 1 && privilege == 1)
        return false;
    // do things.
    user2.privilege = privilege;
    user_db->modify(id2, user2);
    return true;
}

bool sjtu::processor::task_add_train() {
    train_info_val newTrain;
    newTrain.read();

    // check whether train_id has existed.
    train_id_val tmp;
    tmp = train_id_db->find(train_id_key(newTrain.train_id));
    if (!tmp.null_obj())
        return false;

    // insert both in pile and B+.
    // pile:
    int tail_offset = train_info_db->append_train(newTrain);
    // B+:
    train_id_key id_key(newTrain.train_id);
    train_id_val id_val(tail_offset, false);
    train_id_db->insert(newTrain.train_id, id_val);   // implicitly call constructor.
    return true;
}
bool sjtu::processor::task_query_train(sjtu::train_info_val &ret)  {
    char id[ID_SIZE];
    train_id_val val;

    scanf("%s", id);
    val = train_id_db->find(id);

    // check whether id exists or whether it's on sale.
    if (val.null_obj() || !val.sale)
        return false;
    // query for train information.
    train_info_db->get_train_by_offset(val.address, ret);
    return true;
}
bool sjtu::processor::task_sale_train()  {
    char id[ID_SIZE];
    train_id_val id_val;

    scanf("%s", id);
    id_val = train_id_db->find(id);

    // if the train doesn't exist, or the train has been on sale, return false.
    // otherwise sale it, and put its information into station database.
    if (id_val.null_obj() || id_val.sale)
        return false;
    else {
        train_info_val ret;

        train_info_db->get_train_by_offset(id_val.address, ret);

        // record all ret's stations into station_db->
        for (int i = 0; i < ret.station_num; ++i) {
            station_db->insert(station_key{ret.stations[i].station_name, ret.train_catalog, ret.train_id},
                           station_val{ret.train_id, i, ((i + 1) == ret.station_num)});
        }
        // change id_val.sale to be true, and store it.
        id_val.sale = true;
        train_id_db->modify(id, id_val);
        return true;
    }
}
bool sjtu::processor::task_delete_train()  {
    char id[ID_SIZE];
    train_id_val val;

    scanf("%s", id);
    val = train_id_db->find(id);

    // if id doesn't exist or the train has been on sale, return false.
    // otherwise erase it.
    if (val.null_obj() || val.sale)
        return false;
    else {
        train_id_db->erase(id); // erase train_id.
        return true;
    }
}
bool sjtu::processor::task_modify_train()  {
    train_info_val modified_val;
    train_id_val val;

    modified_val.read();
    val = train_id_db->find(modified_val.train_id);

    // if train doesn't exist, or the train has been on sale, return false.
    if (val.null_obj() || val.sale)
        return false;
    else {
        train_info_db->write_train_by_offset(val.address, modified_val);
        return true;
    }
}

bool sjtu::processor::task_clean() {
    // 如果已经删库了，你还删个猴子。
    if (clean)
        return false;
    else {
        close_file();
        unlink("zuser_db.txt");
        unlink("ztrain_id_db.txt");
        unlink("ztrain_pile.txt");
        unlink("zstation_db.txt");
        unlink("zconst.txt");
        clean = true;
        return true;
    }
}
