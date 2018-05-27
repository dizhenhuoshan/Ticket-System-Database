#ifndef database_hpp
#define database_hpp
#include <iostream>
#include "BplusTree.h"
#include "keys.h"
#include "datas.h"

namespace sjtu
{
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
     *  3. ticket --ticket(every station) information of one train
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
    class Database
    {
    private:
        UniqueBPlusTree<User_id_Key, User_Data>     user("user.db");
        UniqueBPlusTree<Train_Key, Train_Data>      train("train.db");
        UniqueBPlusTree<Ticket_Key, Ticket_Data>    ticket("ticket.db");
        UniqueBPlusTree<Station_Key, Empty>          station("station.db");
        UniqueBPlusTree<Buyer_Key, Empty>           buyer("buyer.db");

        /*quick_sort for Ticket_Data to ensure that the */
        void quick_sort(T a[], int l, int r)
        {
            if (l >= r)
                return;
            else
            {
                int pos = rand()%(r - l) + l + 1;
                T k = a[pos];
                int low = l, high = r;
                T tmp;
                while(low < high)
                {
                    while(a[low] < k)   low++;
                    while(a[high] > k)  high--;
                    if (low <= high)
                    {
                        tmp = a[low];
                        a[low] = a[high];
                        a[high] = tmp;
                        low++; high--;
                    }
                }
            }
            quick_sort(a, l, high);
            quick_sort(a, low, r);
        }

    public:
        Database()
        {
            user.open_file();
            train.open_file();
            ticket.open_file();
            station.open_file();
            buyer.open_file();
        }
        ~Database()
        {
            user.close_file();
            train.close_file();
            ticket.close_file();
            station.close_file();
            buyer.close_file();
        }

        /*
            These are orders for user database
        */

        unsigned int user_register(char name[], char passwd[], char email[], char phone[])
        {
            unsigned int id = 2018 + user.size();
            char privilege = 57;
            User_id_Key key(id);
            if (id == 2018)
            {
                privilege = 2;
            }
            else privilege = 1;
            User_Data data(name, passwd, email, phone, privilege);
            bool flag = user.insert(key, data);
            if (flag)   return id;
                else return -1;
        }

        bool user_login(unsigned int id, char passwd[])
        {
            User_Data data = user.find(id);
            if (data.get_privilege() == 57)
                return false;
            if (strcmp(data.get_passwd(), passwd) != 0)
                return false;
            return true;
        }

        User_Data user_query_profile(unsigned int id)
        {
            User_Data data = user.find(id);
            return data;
        }

        bool modify_profile(unsigned int id, char name[], char passwd[], char email[], char phone[])
        {
            User_id_Key key(id);
            User_Data obj = user.find(key);
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
                if (privilege != 2 && data2.get_privilege() == 2)
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
            Train_Key train_key(id);
            if (train.find(train_id).get_station_num() != 0)
                return false;
            Train_Data train_data(train_name, ticket_kind, catalog, nstation, nprice);
            train.insert(train_key, train_data);
            for (int i = 0; i < nstation, i++)
            {
                Ticket_Key ticket_key(train_id, qsta[i].station, (char)i + 1);
                Ticket_Data ticket_data(qsta[i]);
                ticket.insert(ticket_key, ticket_data);
            }
            return true;
        }

        bool sale_train(char train_id[])
        {
            Train_Key key(train_id);
            Train_Data data = train.find(key);
            if (data.get_sale() == 1)
                return false;
            else
            {
                Train_Data mdata = (data, true);
                train.modify(key, mdata);
            }
        }

        bool query_train(char train_id[], Train_Query &tra, Station_Query sta[], int &num)
        {
            Train_Key train_key(train_id);
            Train_Data train_data = train.find(train_id);
            if (train_data.get_station_num() == 0)
                return false;
            tra = Train_Query(train_data);
            Ticket_Key min_key(train_id, 0);
            Ticket_Key max_key(train_id, 1);
            Ticket_Data *stations = new Ticket_Data[maxl];
            ticket.get_range(min_key, max_key, stations, num);
            for (int i = 0; i < num; i++)
                sta[i] = Station_Query(stations[i]);
            return true;
        }

        bool delete_train(char train_id[])
        {
            if (train.find(Train_Key(train_id)).is_sale == 1)
                return false;
            Train_Query tra;
            Station_Query *sta = new Station_Query[maxl];
            int numsta = 0;
            query_ticket(train_id, tra, sta, numsta);
            for (int i = 0; i < numsta; i++)
            {
                station.erase(Station_Key(sta[i].station, train_id));
                ticket.erase(Ticket_Key(train_id, sta[i].station, 0));
            }
        }

        bool modify_train(char train_id[], char train_name[], char catalog[], char nstation, char nprice, char ticket_kind[][maxm], Station_Query qsta[])
        {
            if (!delete_train(train_id))
                return false;
            else
            {
                Train_Key train_key(id);
                Train_Data train_data(train_name, ticket_kind, catalog, nstation, nprice);
                train.insert(train_key, train_data);
                for (int i = 0; i < nstation, i++)
                {
                    Ticket_Key ticket_key(train_id, qsta[i].station, (char)i + 1);
                    Ticket_Data ticket_data(qsta[i]);
                    ticket.insert(ticket_key, ticket_data);
                }
            }
            return true;
        }

        /*
        *   cal_tickets is to find the tickets with specific train_id and loc
        */
        bool cal_tickets(char loc1[], char loc2[], char data[], Train_Data traink, Ticket_Key tkey[], Ticket_Data tdata[], int sum, Ticket_Query &ans)
        {
            double sumprice[maxe];
            short t_num[maxe];
            memset(sumprice, 0, sizeof(sumprice));
            memset(t_num, 0, sizeof(t_num));
            char start[2];
            char arrive[2];
            for (int i = 0; i < maxl; i++)
            {
                if (strcmp(tkey[i].get_loc(), loc1) == 0)
                {
                    loc1_order = tkey[i].get_order();
                    memcpy(start, tdata[i].get_statime(), sizeof(start));
                    while (strcmp(tkey[i].get_loc(), loc1) != 0 && i < maxl - 1)
                    {
                        i++;
                        for (int i = 0; i < maxe; i++)
                        {
                            sumprice[i] += tdata.get_price(i);
                            t_num[i] = min(t_num[i], tdata[i].get_num(i));
                        }
                    }
                    if (strcmp(tkey[i].get_loc, loc2) != 0)
                        return false;
                    else
                    {
                        memcpy(arrive, tdata[i].get_arrtime(), sizeof(arrive));
                        ans = Ticket_Query(tkey.get_train_id(), loc1, start, date[], loc2, arrive, traink.get_ticket_kind(), t_num, sumprice, nprice);
                        return true;
                    }

                }
            }
        }

        int query_ticket(char loc1[], char loc2[], char date[], char catalog[])
        {
            Station_Key max_key1(loc1, 1);
            Station_Key min_key1(loc1, 0);
            Station_Key max_key2(loc2, 1);
            Station_Key min_key2(loc2, 0);
            Station_Key *loc1_train = new Station_Key[maxs];
            Station_Key *loc2_train = new Station_Key[maxs];


            int loc1_train_num = 0;
            int loc2_train_num = 0;
            int cnt = 0;

            station.get_range(min_key1, max_key1, loc1_train, loc1_train_num);
            station.get_range(min_key2, max_key2, loc2_train, loc2_train_num);
            for (int i = 0; i < loc1_train_num; i++)
            {
                for (int j = 0; j < loc2_train_num; i++)
                {
                    if (strcmp(loc1_train[i], loc2_train[j]) == 0)
                    {

                    }
                }
            }

        }

        bool query_transfer() {}

        bool buy_ticket(unsigned int id, short num, char train_id[], char loc1[], char loc2[], char date[], char ticket_kind[])
        {
            if (user.find(id).privilege == 57)  return false;
        }

        bool clean()
        {
            if(remove("user.db") || remove("train.db") || remove("ticket.db") || remove("station.db"))
                return false;
            else return true;
        }

    };
}

#endif
