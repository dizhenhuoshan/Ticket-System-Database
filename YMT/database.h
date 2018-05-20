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
    class Database
    {
    private:
        UniqueBPlusTree<User_id_Key, User_Data>     user("user.db");
        UniqueBPlusTree<Train_Key, Train_Data>      train("train.db");
        UniqueBPlusTree<Ticket_Key, Ticket_Data>    ticket("ticket.db");
        UniqueBPlusTree<Station_Key, char>          station("station.db");
        UniqueBPlusTree<Buyer_Key, short>           buyer("buyer.db");

        template<T>
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

    /*
        These are orders for ticket database
    */
    //unfinished -- seg_search needed;
    int query_ticket(char loc1[], char loc2[], char date, char catalog[], vector &v;)
    {
        Station_Key max_key(loc1, 1);
        Station_Key min_key(loc1, 0);
        Station_Key *loc1_train = new Station_Key[maxs];
        Station_Key *loc2_train = new Station_Key[maxs];
    }

    bool query_transfer() {}

    bool buy_ticket(unsigned int id, short num, char train_id[], char loc1[], char loc2[], char date[], char ticket_kind[])
    {
        if (user.find(id))
    }


    };
}

#endif
