#ifndef database_hpp
#define database_hpp
#include <iostream>
#include <cstring>
#include "BPlusTree.h"
#include "output.h"

using namespace sjtu;

    /*
     *  Main MyDatabase List:
     *  1. user --information of the user
     *          --Using key:: User_id_Key
     *          --Using data:: User_Data
     *
     *  2. train --information of the train except the station
     *           --Using key::Train_Key
     *           --Using data::Train_Data
     *
     *  3. ticket --ticket(every station) information of one train
     *            --Uisng key::Ticket_Key
     *            --Using data::Ticket_Data
     *
     *  4. station --information of the passed trains of a station
     *             --Using key::Station_Key
     *             --Using data::char (char order:  1-based order, recording the order of the passed station;
     *
     *  5. buyer --information of the bought ticket for a user
     *           --Using key::Buyer_Key
     *           --Using data::Empty
     *  6. ticket_sale --information of the bought ticket for a train_name
     *                 --Using key::Ticket_Sale_Key
     *                 --Using data::Ticket_Sale_Data
     */
    
class MyDatabase
{
private:
    BPlusTree<User_id_Key, User_Data>     user;
    BPlusTree<Train_Key, Train_Data>      train;
    BPlusTree<Ticket_Key, Ticket_Data>    ticket;
    BPlusTree<Station_Key, char>          station;
    BPlusTree<Buyer_Key, short>           buyer;
    BPlusTree<Ticket_Sale_Key, Ticket_Sale_Data>     ticket_sale;
public:
    MyDatabase():user("user.database"), train("train.database"), ticket("ticket.database"), station("station.database"), buyer("buyer.database"), ticket_sale("ticketsale.database")
    {
        user.open_file();
        train.open_file();
        ticket.open_file();
        station.open_file();
        buyer.open_file();
        ticket_sale.open_file();
    }
    ~MyDatabase()
    {
        user.close_file();
        train.close_file();
        ticket.close_file();
        station.close_file();
        buyer.close_file();
        ticket_sale.close_file();
    }

    /*
        These are assistant functions for the database
    */

    /*Covert the time to time_string for output*/
    void time_to_string(const char times[], char ans[])
    {
        char invaild[6] = "xx:xx";
        if (times[0] >= 100)
        {
            strcpy(ans, invaild);
            return;
        }
        else
        {
            ans[5] = '\0';
            ans[2] = ':';
            ans[0] = static_cast<char>(times[0] / 10 + '0');
            ans[1] = static_cast<char>(times[0] % 10 + '0');
            ans[3] = static_cast<char>(times[1] / 10 + '0');
            ans[4] = static_cast<char>(times[1] % 10 + '0');
        }
    }
    /*Covert the time_string to stored time*/
    void string_to_time(char times[], const char str[])
    {
        if (str[0] == 'x')
        {
            times[1] = times[0] = 118;
            return;
        }
        else
        {
            times[0] = static_cast<char>((str[0] - '0') * 10 + (str[1] - '0'));
            times[1] = static_cast<char>((str[3] - '0') * 10 + (str[4] - '0'));
            return;
        }
    }

    /*Covert the string to stored date*/
    void string_to_date(short &year, char &month, char &day, const char str[])
    {
        if (str[0] == 'x')
        {
            year = -1; month = 0; day = 0;
        }
        else
        {
            year = 0; month = 0; day = 0;
            for (int i = 0; i < 4; i++)
            {
                year = static_cast<short>(year * 10 + str[i] - '0');
            }
            for (int i = 0; i < 2; i++)
            {
                month = static_cast<char>(month * 10 + str[5 + i] - '0');
                day = static_cast<char>(day * 10 + str[8 + i] - '0');
            }
        }
    }
    /*Covert the date to stored time*/
    void date_to_string(short year, char month, char day, char ans[])
    {
        char invaild[11] = "xxxx-xx-xx";
        if (year == -1)
        {
            memcpy(ans, invaild, sizeof(invaild));
            return;
        }
        ans[10] = '\0';
        ans[4] = '-';
        ans[7] = '-';
        for (int i = 0; i < 4; i++)
        {
            ans[3 - i] = static_cast<char>(year % 10 + '0');
            year = static_cast<short>(year / 10);
        }
        for (int i = 0; i < 2; i++)
        {
            ans[6 - i] = static_cast<char>(month % 10 + '0');
            ans[9 - i] = static_cast<char>(day % 10 + '0');
            month = static_cast<char>(month / 10);
            day = static_cast<char>(day / 10);
        }
    }

    //date_pushnext is to get next date_year
    void date_pushnext(char objdate[])
    {
        short year;
        char month;
        char day;
        string_to_date(year, month, day, objdate);
        if (month == 2)
        {
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            {
                if (day < 29)   day++;
                else
                {
                    month = 3; day = 1;
                }
            }
            else
            {
                if (day < 28)   day++;
                else
                {
                    month = 3; day = 1;
                }
            }
        }
        if (month == 12)
        {
            if (day < 31)   day++;
            else
            {
                year++; month = 1; day = 1;
            }
        }
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10)
        {
            if (day < 31)   day++;
            else
            {
                month++; day = 1;
            }
        }
        if (month == 4 || month ==6  || month == 9 || month == 11)
        {
            if (day < 30)   day++;
            else
            {
                month ++; day = 1;
            }
        }
        char *ans = nullptr;
        date_to_string(year, month, day, ans);
        memcpy(objdate, ans, sizeof(ans));
        delete[] ans;
    }

    //check whether the catalog is the required catalog. (tested OK
    bool check_catalog(char input[], char all[])
    {
        auto input_len = static_cast<int>(strlen(input));
        auto all_len = static_cast<int>(strlen(all));
        for (int i = 0; i < all_len; i++)
        {
            if (all[i] == input[0])
            {
                bool flag = true;
                if (all_len - i - 1 < input_len)
                    return false;
                for (int j = 0; j < input_len && i + j < all_len; j++)
                {
                    if (input[j] != all[i + j])
                        flag = false;
                }
                if (flag)   return true;
            }
        }
        return false;
    }

    //cal_ticket_kind is to find the number of the ticket_kind in train_data
    int cal_ticket_kind(char ticket_kind[], Train_Data &train_data)
    {
        for (int i = 0; i < maxe; i++)
        {
            if (strcmp(ticket_kind, train_data.get_ticket_kind(i)) == 0)
                return i;
        }
    }

    //query_left_ticket is to get the left ticket from loc1 to loc2 in train_id with specific date.
    bool query_left_ticket(char train_id[], char date[], char loc1[], char loc2[], Ticket_Key ticket_key[], Ticket_Data ticket_data[], short ans[])
    {
        for (int i = 0; i < maxe; i++)
            ans[i] = origin_ticket;
        char mydate[11];
        memcpy(mydate, date, sizeof(mydate));
        for (int i = 0; i < maxl; i++)
        {
            if (strcmp(loc1, ticket_key[i].get_loc()) == 0)
            {
                bool flag = false;
                int cnt = 0;
                for (int j = 0; i + j < maxl; j++)
                {
                    if (strcmp(loc2, ticket_key[i + j].get_loc()) == 0)
                        flag = true;
                    cnt++;
                }
                if (!flag)  return false;
                for (int j = 0; j <= cnt; j++)
                {
                    if (j > 0)
                    {
                        if (strcmp(ticket_data[i + j - 1].get_arrtime(), ticket_data[i + j - 1].get_statime()) > 0 || strcmp(ticket_data[i + j - 1].get_statime(), ticket_data[i + j].get_arrtime()) > 0)
                        {
                            date_pushnext(mydate);
                        }
                    }
                    Ticket_Sale_Key ts_key(train_id, ticket_key[i + j].get_loc(), mydate);
                    Ticket_Sale_Data ts_data = ticket_sale.find(ts_key);
                    for (int k = 0; k < maxe; k++)
                    {
                        ans[k] = std::min(ans[k], ts_data.get_num(k));
                    }
                }
                return true;
            }
        }
    }


    //cal_tickets is to find the tickets with specific train_id and loc
    //warning: date in Ticket_Query will automatically changed when output
    bool cal_tickets(char train_id[], char loc1[], char loc2[], char date[], Train_Data train_data, Ticket_Key tkey[], Ticket_Data tdata[], Ticket_Query &ans)
    {
        double sumprice[maxe];
        short t_num[maxe];
        memset(sumprice, 0, sizeof(sumprice));
        memset(t_num, 0, sizeof(t_num));
        char start[2];
        char arrive[2];
        query_left_ticket(train_id, date, loc1, loc2, tkey, tdata, t_num);
        for (int i = 0; i < train_data.get_station_num(); i++)
        {
            if (strcmp(tkey[i].get_loc(), loc1) == 0)
            {
                char loc1_order = tkey[i].get_order();
                memcpy(start, tdata[i].get_statime(), sizeof(start));
                while (strcmp(tkey[i].get_loc(), loc2) != 0 && i < maxl - 1)
                {
                    i++;
                    for (int j = 0; j < maxe; j++)
                    {
                        sumprice[j] += tdata[i].get_price(j);
                    }
                }
                if (strcmp(tkey[i].get_loc(), loc2) != 0)
                    return false;
                else
                {
                    memcpy(arrive, tdata[i].get_arrtime(), sizeof(arrive));
                    char** tk_tmp = train_data.get_origin_ticket_kind();
                    ans = Ticket_Query(tkey[i].get_train_id(), loc1, start, date, loc2, arrive, tk_tmp, t_num, sumprice, train_data.get_price_num());
                    for (int k = 0; k < maxe; k++)
                    {
                        delete[] tk_tmp[k];
                    }
                    delete[] (tk_tmp);
                    return true;
                }
            }
        }
        return false;
    }

    //get_sta_order is to find the order of the station in a train
    //warning: not the real order in Ticket_Key, but the order in the array(0-based order)
    int get_sta_order(char loc[], Ticket_Key tic_key[], int sta_num)
    {
        for (int i = 0; i < sta_num; i++)
        {
            if (strcmp(loc, tic_key[i].get_loc()) == 0)
            {
                return (int)(tic_key[i].get_order()) - 1;
            }
        }
        return -1;
    }


    // direct_buy_ticket is to buy ticket without check
    void direct_buy_ticket(unsigned int user_id, char train_id[], short num, char loc1[], char loc2[], char date[], int kind, char ticket_kind[], Ticket_Key tic_key[])
    {
        short objnum[maxe];
        Buyer_Key buyer_key(user_id, num, train_id, loc1, loc2, ticket_kind, date);
        buyer.insert(buyer_key, num);
        for (int i = 0; i < maxl; i++)
        {
            bool flag = false;
            if (strcmp(loc1, tic_key[i].get_loc()) == 0)
            {
                for (int j = 0; i + j < maxl; j++)
                {
                    if (strcmp(loc2, tic_key[i + j].get_loc()) == 0)
                        flag = true;
                    Ticket_Sale_Key tic_sale_key(train_id, tic_key[i + j].get_loc(), date);
                    Ticket_Sale_Data tic_sale_data = ticket_sale.find(tic_sale_key);
                    bool exist = false;
                    for (int k = 0; k < maxe; k++)
                    {
                        if(tic_sale_data.get_num(k) != 0)
                        {
                                exist = true;
                        }
                    }
                    for (int t = 0; t < maxe; t++)
                    {
                        objnum[t] = tic_sale_data.get_num(t);
                        objnum[kind] += num;
                    }
                    Ticket_Sale_Data modified_data(objnum);
                    if(exist)
                    {
                        ticket_sale.modify(tic_sale_key, modified_data);
                    }
                    else ticket_sale.insert(tic_sale_key, modified_data);
                    if (flag)   return;
                }
            }
        }
    }

    // direct_refund_ticket is to refund ticket without user and train check
    bool direct_refund_ticket(unsigned int user_id, char train_id[], short num, char loc1[], char loc2[], char date[], int kind, char ticket_kind[], Ticket_Key tic_key[])
    {
        short objnum[maxe];
        Buyer_Key buyer_key(user_id, num, train_id, loc1, loc2, ticket_kind, date);
        short order_num = buyer.find(buyer_key);
        bool all_refund = false;
        if (num > order_num)    return false;
        if (num == order_num)
        {
            buyer.erase(buyer_key);
            all_refund = true;
        }
        else buyer.modify(buyer_key, order_num - num);
        for (int i = 0; i < maxl; i++)
        {
            bool flag = false;
            if (strcmp(loc1, tic_key[i].get_loc()) == 0)
            {
                for (int j = 0; i + j < maxl; j++)
                {
                    if (strcmp(loc2, tic_key[i + j].get_loc()) == 0)
                        flag = true;
                    Ticket_Sale_Key tic_sale_key(train_id, tic_key[i + j].get_loc(), date);
                    Ticket_Sale_Data tic_sale_data = ticket_sale.find(tic_sale_key);
                    bool exist = false;
                    for (int k = 0; k < maxe; k++)
                    {
                        objnum[k] = tic_sale_data.get_num(k);
                        objnum[kind] -= num;
                    }
                    Ticket_Sale_Data modified_data(objnum);
                    if(!all_refund)
                    {
                        ticket_sale.modify(tic_sale_key, modified_data);
                    }
                    else ticket_sale.erase(tic_sale_key);
                    if (flag)   return true;
                }
            }
        }
    }

    //cal_sum_time is to calculate the total time for the transfer. return time in minute.
    int cal_sum_time(const Ticket_Query &first_query, const Ticket_Query &second_query)
    {
        return first_query.get_total_time() + second_query.get_total_time();
    }

    /*
        These are orders for user database
    */

    unsigned int user_register(char name[], char passwd[], char email[], char phone[])
    {
        User_id_Key last = user.back();
        unsigned int id;
        if (last.get_user_id() == 0)
            id = static_cast<unsigned int>(2018);
        else id = last.get_user_id() + 1;
        char privilege;
        User_id_Key key(id);
        if (id == 2018)
        {
            privilege = 2;
        }
        else privilege = 1;
        User_Data data(name, passwd, email, phone, privilege);
        bool flag = user.insert(key, data);
        if (flag)   return id;
        else return 0;
    }

    bool user_login(unsigned int id, char passwd[])
    {
        User_Data data = user.find(User_id_Key(id));
        if (data.get_privilege() == 57)
            return false;
        return strcmp(data.get_passwd(), passwd) == 0;
    }

    bool query_profile(unsigned int id, User_Query &ans)
    {
        User_id_Key key(id);
        User_Data data = user.find(key);
        if (data.get_privilege() == 57)
            return false;
        ans = User_Query(data);
        return true;
    }

    bool modify_profile(unsigned int id, char name[], char passwd[], char email[], char phone[])
    {
        User_id_Key key(id);
        User_Data obj = user.find(key);
        if (obj.get_privilege() == 57)
        {
            return false;
        }
        User_Data data(name, passwd, email, phone, obj.get_privilege());
        return user.modify(key, data);
    }

    bool modify_privilege(unsigned int id1, unsigned int id2, char privilege)
    {
        User_id_Key key1(id1);
        User_id_Key key2(id2);
        User_Data data1 = user.find(key1);
        User_Data data2 = user.find(key2);
        if (data1.get_privilege() != 2)
            return false;
        else
        {
            if (data2.get_privilege() == 2 && privilege == 2)
                return true;
            if (data2.get_privilege() == 2 && privilege == 1)
                return false;
            else
            {
                User_Data data(data2.get_name(), data2.get_passwd(), data2.get_email(), data2.get_phone(), privilege);
                return user.modify(key2, data);
            }
        }
    }

    bool add_train(char train_id[], char train_name[], char catalog[], char nstation, char nprice, char ticket_kind[][maxm], Station_Query qsta[])
    {
        Train_Key train_key(train_id);
        if (train.find(train_key).get_station_num() != 0)
            return false;
        Train_Data train_data(train_name, ticket_kind, catalog, nstation, nprice);
        train.insert(train_key, train_data);
        for (int i = 0; i < nstation; i++)
        {
            Ticket_Key ticket_key(train_id, qsta[i].station, static_cast<char>(i + 1));
            Ticket_Data ticket_data(qsta[i].arrtime, qsta[i].statime, qsta[i].stpover, qsta[i].price);
            ticket.insert(ticket_key, ticket_data);
            Station_Key station_key(qsta[i].station, train_id);
            station.insert(station_key, (char)(i + 1));
        }
        return true;
    }

    bool sale_train(char train_id[])
    {
        Train_Key key(train_id);
        Train_Data data = train.find(key);
        if (data.get_station_num() == 0)
            return false;
        if (data.get_sale() == 1)
            return false;
        else
        {
            data.make_sale();
            train.modify(key, data);
            return true;
        }
    }

    bool query_train(char train_id[], Train_Query &tra, Station_Query sta[], int &num)
    {
        Train_Key train_key(train_id);
        Train_Data train_data = train.find(Train_Key(train_id));
        if (train_data.get_station_num() == 0)
            return false;
        char **tk_tmp = train_data.get_origin_ticket_kind();
        tra = Train_Query(train_id, train_data.get_train_name(), train_data.get_catalog(), train_data.get_station_num(), train_data.get_price_num(), tk_tmp);
        tra.mycout();
        for (int k = 0; k < maxe; k++)
        {
            delete[] tk_tmp[k];
        }
        delete[] (tk_tmp);
        Ticket_Key min_key(train_id, 0);
        Ticket_Key max_key(train_id, 1);
        auto *tic_keys = new Ticket_Key[maxl];
        auto *tic_datas = new Ticket_Data[maxl];
        ticket.get_range(min_key, max_key, tic_keys, tic_datas, num);
        for (int i = 0; i < num; i++)
            sta[i] = Station_Query(tic_keys[i].get_loc(), tic_datas[i].get_arrtime(), tic_datas[i].get_statime(), tic_datas[i].get_stpover(), tic_datas[i].get_origin_price());
        delete[] (tic_keys);
        delete[] (tic_datas);
        return true;
    }

    bool delete_train(char train_id[])
    {
        Train_Data tmp = train.find(Train_Key(train_id));
        if (tmp.get_sale() == 1 || tmp.get_station_num() == 0)
            return false;
        Ticket_Key min_tickey(train_id, 0);
        Ticket_Key max_tickey(train_id, 1);
        auto *tic_key = new Ticket_Key[maxl];
        int numsta = 0;
        ticket.get_range(min_tickey, max_tickey, tic_key, nullptr, numsta);
        for (int i = 0; i < numsta; i++)
        {
            station.erase(Station_Key(tic_key[i].get_loc(), train_id));
            ticket.erase(Ticket_Key(train_id, tic_key[i].get_loc(), static_cast<char>(i + 1)));
        }
        delete[] (tic_key);
        return true;
    }

    bool modify_train(char train_id[], char train_name[], char catalog[], char nstation, char nprice, char ticket_kind[][maxm], Station_Query qsta[])
    {
        if (!delete_train(train_id))
            return false;
        else
        {
            Train_Key train_key(train_id);
            Train_Data train_data(train_name, ticket_kind, catalog, nstation, nprice);
            train.insert(train_key, train_data);
            for (int i = 0; i < nstation; i++)
            {
                Ticket_Key ticket_key(train_id, qsta[i].station, static_cast<char>(i + 1));
                Ticket_Data ticket_data(qsta[i].arrtime, qsta[i].statime, qsta[i].stpover, qsta[i].price);
                ticket.insert(ticket_key, ticket_data);
                Station_Key station_key(qsta[i].station, train_id);
                station.insert(station_key, (char)(i + 1));
            }
        }
        return true;
    }


    int query_ticket(char loc1[], char loc2[], char date[], char catalog[], Ticket_Query ans[])
    {
        //find all the trains in loc1 and loc2.
        Station_Key max_stakey1(loc1, 1);
        Station_Key min_stakey1(loc1, 0);
        Station_Key max_stakey2(loc2, 1);
        Station_Key min_stakey2(loc2, 0);
        auto *loc1_sta = new Station_Key[maxs];
        auto *loc2_sta = new Station_Key[maxs];
        char loc1_order[maxs];
        char loc2_order[maxs];
        int loc1_sta_num = 0; //loc1_sta_num is the number of the train in station loc1
        int loc2_sta_num = 0; //loc2_sta_num is the number of the train in station loc2

        int cnt = 0;

        station.get_range(min_stakey1, max_stakey1, loc1_sta, loc1_order, loc1_sta_num);
        station.get_range(min_stakey2, max_stakey2, loc2_sta, loc2_order, loc2_sta_num);

        for (int i =0 ; i < loc1_sta_num; i++)
        {
            for (int j = 0; j < loc2_sta_num; j++)
            {
                if(strcmp(loc1_sta[i].get_train_id(), loc2_sta[j].get_train_id()) == 0 && loc1_order[i] < loc2_order[j])
                {
                    char train_id[maxm];
                    memcpy(train_id, loc1_sta[i].get_train_id(), sizeof(train_id));
                    Train_Data train_data = train.find(Train_Key(train_id));
                    if(check_catalog(train_data.get_catalog(), catalog))
                    {
                        Ticket_Key min_tickey(train_id, 0);
                        Ticket_Key max_tickey(train_id, 1);
                        auto *ticket_key = new Ticket_Key[maxl];
                        auto *ticket_data = new Ticket_Data[maxl];
                        int sta_num;
                        ticket.get_range(min_tickey, max_tickey, ticket_key, ticket_data, sta_num);
                        Ticket_Query tmp;
                        if(cal_tickets(train_id, loc1, loc2, date, train_data, ticket_key, ticket_data, tmp))
                        {
                            ans[cnt++] = tmp;
                        }
                        delete[] ticket_key;
                        delete[] ticket_data;
                    }
                }
            }
        }
        return cnt;
    }

    bool query_transfer(char loc1[], char loc2[], char date[], char catalog[], Ticket_Query &query1_ans, Ticket_Query &query2_ans)
    {
        //get all the train_id in loc1 and loc2
        Station_Key max_stakey1(loc1, 1);
        Station_Key min_stakey1(loc1, 0);
        Station_Key max_stakey2(loc2, 1);
        Station_Key min_stakey2(loc2, 0);
        auto *loc1_sta = new Station_Key[maxs];
        auto *loc2_sta = new Station_Key[maxs];
        int loc1_sta_num = 0;
        int loc2_sta_num = 0;
        station.get_range(min_stakey1, max_stakey1, loc1_sta, nullptr, loc1_sta_num);
        station.get_range(min_stakey2, max_stakey2, loc2_sta, nullptr, loc2_sta_num);
        if (loc1_sta_num == 0 || loc2_sta_num == 0)
        {
            delete[] loc1_sta;
            delete[] loc2_sta;
            return false;
        }

        //get train_data with all the known train_id
        auto *train1_data = new Train_Data[loc1_sta_num + 1];
        auto *train2_data = new Train_Data[loc2_sta_num + 1];
        for (int i = 0; i < loc1_sta_num; i++)
        {
            train1_data[i] = train.find(Train_Key(loc1_sta[i].get_train_id()));
        }
        for (int i = 0; i < loc2_sta_num; i++)
        {
            train2_data[i] = train.find(Train_Key(loc2_sta[i].get_train_id()));
        }

        //get ticket_data with all the known train.
        auto ** loc1_tic_key = new Ticket_Key*[loc1_sta_num];
        for (int i = 0; i < loc1_sta_num; i++)
                loc1_tic_key[i]=  new Ticket_Key[maxl];
        auto ** loc2_tic_key = new Ticket_Key*[loc2_sta_num];
        for (int i = 0; i < loc2_sta_num; i++)
            loc2_tic_key[i] = new Ticket_Key[maxl];
        auto ** loc1_tic_data = new Ticket_Data*[loc1_sta_num];
        for (int i = 0; i < loc1_sta_num; i++)
            loc1_tic_key[i] = new Ticket_Key[maxl];
        auto ** loc2_tic_data = new Ticket_Data*[loc2_sta_num];
        for (int i = 0; i < loc2_sta_num; i++)
            loc2_tic_key[i] = new Ticket_Key[maxl];
        auto *loc1_tic_num = new int[loc1_sta_num + 1];
        auto *loc2_tic_num = new int[loc2_sta_num + 1];
        for (int i = 0; i < loc1_sta_num; i++)
        {
            Ticket_Key min_tickey1(loc1_sta[i].get_train_id(), 0);
            Ticket_Key max_tickey1(loc1_sta[i].get_train_id(), 1);
            ticket.get_range(min_tickey1, max_tickey1, loc1_tic_key[i], loc1_tic_data[i], loc1_tic_num[i]);
        }
        for (int i = 0; i < loc2_sta_num; i++)
        {
            Ticket_Key min_tickey2(loc2_sta[i].get_train_id(), 0);
            Ticket_Key max_tickey2(loc2_sta[i].get_train_id(), 0);
            ticket.get_range(min_tickey2, max_tickey2, loc2_tic_key[i], loc2_tic_data[i], loc2_tic_num[i]);
        }

        //ready for calculate the transfer
        Ticket_Query query1_tmp;
        Ticket_Query query2_tmp;
        int min_time = 0x3f3f3f3f;
        char mydate[maxm];
        memcpy(mydate, date, sizeof(mydate));

        //find the pos of loc1 in train1 and the pos of loc2 in train2
        for (int i = 0; i < loc1_sta_num; i++)
        {
            for (int j = 0; j < loc2_sta_num; j++)
            {
                int loc1_order = get_sta_order(loc1, loc1_tic_key[i], loc1_tic_num[i]);
                int loc2_order = get_sta_order(loc2, loc2_tic_key[j], loc2_tic_num[j]);
                if (loc1_order >= 0 && loc2_order >= 0 && check_catalog(train1_data[i].get_catalog(), catalog) && check_catalog(train2_data[j].get_catalog(), catalog)) // find two trains that can transfer
                {
                    bool flag = false;
                    for (int k = loc1_order + 1; k < loc1_tic_num[i]; k++)
                    {
                        for (int t = loc2_order - 1; t >= 0; t--)
                        {
                            if(strcmp(loc1_tic_key[i][k].get_loc(), loc2_tic_key[j][t].get_loc()) == 0)
                            {
                                flag = true;
                                cal_tickets(loc1_sta[i].get_train_id(), loc1, loc1_tic_key[i][k].get_loc(), mydate, train1_data[i], loc1_tic_key[i], loc1_tic_data[i], query1_tmp);
                                if (query1_tmp.des_d != query2_tmp.des_d || query1_tmp.des_m != query2_tmp.des_m || query1_tmp.des_y != query2_tmp.des_y)
                                {
                                    date_pushnext(mydate);
                                }
                                cal_tickets(loc2_sta[j].get_train_id(), loc2_tic_key[j][t].get_loc(), loc2, mydate, train2_data[j], loc2_tic_key[j], loc2_tic_data[j], query2_tmp);
                                int total_time = query1_tmp.get_total_time() + query2_tmp.get_total_time();
                                if (min_time > total_time)
                                {
                                    min_time = total_time;
                                    query1_ans = query1_tmp;
                                    query2_ans = query2_tmp;
                                }
                                break;
                            }
                        }
                        if (flag)   break;
                    }
                }
            }
        }
        delete[] loc1_sta;
        delete[] loc2_sta;
        delete[] train1_data;
        delete[] train2_data;
        for (int i = 0; i < loc1_sta_num; i++)
        {
            delete[](loc1_tic_key[i]);
            delete[](loc1_tic_data[i]);
        }
        for (int i = 0; i < loc2_sta_num; i++)
        {
            delete[](loc2_tic_key[i]);
            delete[](loc2_tic_data[i]);
        }
        delete[] loc1_tic_key;
        delete[] loc1_tic_data;
        delete[] loc2_tic_key;
        delete[] loc2_tic_data;
        delete[] loc1_tic_num;
        delete[] loc2_tic_num;
        return min_time < 0x3f3f3f3f;
    }

    bool buy_ticket(unsigned int id, short num, char train_id[], char loc1[], char loc2[], char date[], char ticket_kind[])
    {
        //check user, check train is existed and train is saled
        if (user.find(User_id_Key(id)).get_privilege() == 57)  return false;
        Train_Key train_key(train_id);
        Train_Data train_data = train.find(train_key);
        if (train_data.get_station_num() == 0)    return false;
        if (train_data.get_sale() == 0)         return false;
        int kind = cal_ticket_kind(ticket_kind, train_data);
        if (kind == -1) return false;

        //ready to find all the station with the train_id
        Ticket_Key min_tickey(train_id, 0);
        Ticket_Key max_tickey(train_id, 1);
        auto *tic_key = new Ticket_Key[maxl];
        auto *tic_data = new Ticket_Data[maxl];

        //find the num of left tickets from loc1 to loc2;
        int sum = 0;
        ticket.get_range(min_tickey, max_tickey, tic_key, tic_data, sum);
        short t_num[maxe];
        query_left_ticket(train_id, date, loc1, loc2, tic_key, tic_data, t_num);
        if (t_num[kind] <= 0)
        {
            delete[] tic_key;
            delete[] tic_data;
            return false;
        }

        //buy ticket
        direct_buy_ticket(id, train_id, num, loc1, loc2, date, kind, ticket_kind, tic_key);
        return true;
    }

    bool refund_ticket(unsigned int id, short refund_num, char train_id[], char loc1[], char loc2[], char date[], char ticket_kind[])
    {
        //check user, check train is existed and train is saled
        if (user.find(User_id_Key(id)).get_privilege() == 57)  return false;
        Train_Key train_key(train_id);
        Train_Data train_data = train.find(train_key);
        if (train_data.get_station_num() == 0)    return false;
        if (train_data.get_sale() == 0)         return false;
        int kind = cal_ticket_kind(ticket_kind, train_data);
        if (kind == -1) return false;

        //check whether the bought ticket is existed
        Buyer_Key buyer_key(id, refund_num, train_id, loc1, loc2, ticket_kind, date);
        short order_num = buyer.find(buyer_key);
        if (order_num == 0) return false;

        //ready to find all the station with the train_id
        Ticket_Key min_tickey(train_id, 0);
        Ticket_Key max_tickey(train_id, 1);
        auto *tic_key = new Ticket_Key[maxl];
        auto *tic_data = new Ticket_Data[maxl];

        int sum = 0;
        ticket.get_range(min_tickey, max_tickey, tic_key, tic_data, sum);
        return direct_refund_ticket(id, train_id, refund_num, loc1, loc2, date, kind, ticket_kind, tic_key);
    }

    int query_order(unsigned int user_id, char date[], char catalog[], Ticket_Query ticket_query[])
    {
        if (user.find(User_id_Key(user_id)).get_privilege() == 57)  return false;

        Buyer_Key min_buyer_key(user_id, 0);
        Buyer_Key max_buyer_key(user_id, 1);
        Buyer_Key buyer_key[maxb];
        short     buyer_data[maxb];
        Train_Data train_data[maxb];
        bool vis[maxb];
        memset(vis, 0, sizeof(vis));
        int buyer_num = 0; //number of the kind of ticket a user has bought
        int cnt = 0; //number of the kind of the ticket that meet the requirement.
        auto *tic_key = new Ticket_Key[maxl];//to get the time
        auto *tic_data = new Ticket_Data[maxl];
        int sta_num = 0;
        char from_time[6];
        char des_time[6];
        char from_date[11];

        buyer.get_range(min_buyer_key, max_buyer_key, buyer_key, buyer_data, buyer_num);
        if (buyer_num == 0)
            return false;

        for (int i = 0; i < buyer_num; i++)
        {
            train_data[i] = train.find(Train_Key(buyer_key[i].get_train_id()));
        }
        for (int i = 0; i < buyer_num; i++)
        {
            if (check_catalog(train_data[i].get_catalog(), catalog))
            {
                Ticket_Key min_tickey(buyer_key[i].get_train_id(), 0);
                Ticket_Key max_tickey(buyer_key[i].get_train_id(), 1);
                ticket.get_range(min_tickey, max_tickey, tic_key, tic_data, sta_num);
                for (int j = 0; j < sta_num; j++)
                {
                    if (strcmp(buyer_key[i].get_from(), tic_key[j].get_loc()) == 0)
                        time_to_string(tic_data[j].get_statime(), from_time);
                    if (strcmp(buyer_key[i].get_des(), tic_key[j].get_loc()) == 0)
                    {
                        time_to_string(tic_data[j].get_arrtime(), des_time);
                        break;
                    }
                }
                date_to_string(buyer_key[i].get_year(), buyer_key[i].get_month(), buyer_key[i].get_day(), from_date);
                int kind = cal_ticket_kind(buyer_key[i].get_ticket_kind(), train_data[i]);
                short num[maxs];
                num[kind] = buyer_data[i];
                Ticket_Query tmp(buyer_key[i].get_train_id(), buyer_key[i].get_from(), from_time, from_date, buyer_key[i].get_des(), des_time, train_data[i].get_origin_ticket_kind(), num, tic_data[i].get_origin_price(), train_data[i].get_price_num());
                ticket_query[cnt++] = tmp;
            }
        }
        return cnt;
    }

    bool clean()
    {
        return !(remove("user.database") || remove("train.database") || remove("ticket.database") || remove("station.database") || remove("ticketsale.database") || remove("buyer.database"));
    }

};

#endif
