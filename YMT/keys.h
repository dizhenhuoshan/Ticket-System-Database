#ifndef KEYS_HPP
#define KEYS_HPP
#include <cstring>
#include "constant.h"

namespace sjtu
{
    const int maxn = 41;
    const int maxm = 21;
    /** User_id_Key is the class for user index **/
    class User_id_Key
    {
    private:
        unsigned int user_id; //Each user must have a specific id.
    public:
        User_id_Key()
        {
            user_id = 0;
        }
        User_id_Key(const User_id_Key &other)
        {
            user_id = other.user_id;
        }
        User_id_Key(const unsigned int num):user_id(num) {}
        ~User_id_Key() {}

        bool operator== (const User_id_Key &other) const
        {
            return user_id == other.user_id;
        }
        bool operator> (const User_id_Key &other) const
        {
            return user_id > other.user_id;
        }
        bool operator< (const User_id_Key &other) const
        {
            return user_id < other.user_id;
        }
        bool operator>= (const User_id_Key &other) const
        {
            return user_id >= other.user_id;
        }
        bool operator<= (const User_id_Key &other) const
        {
            return user_id <= other.user_id;
        }
        unsigned int get_user_id() const
        {
            return user_id;
        }
    };

    /*
    *   Ticket_Key and Train_Key is used to store ticket information of a train;
    */

    /* Ticket_Key is used to store the information of the passsed station of the train */
    class Ticket_Key
    {
    private:
        char train_id[maxm];
        char loc[maxm];
    public:
        Ticket_Key()
        {
            memset(train_id, 0, sizeof(train_id));
            memset(loc, 0, sizeof(loc));
        }
        /*
            Special creator of the Ticket_Key.
            Types:
                0 - min loc for the train_id;
                1 - max loc of the train_id;
        */
        Ticket_Key(char objtrain_id[], int type)
        {
            memcpy(train_id, objtrain_id, sizeof(train_id));
            if (type == 0)
            {
                memset(loc, 0, sizeof(loc));
            }
            else
            {
                for (int i = 0; i < maxm; i++)
                    loc[i] = 254;
            }
        }
        Ticket_Key(const Ticket_Key &other)
        {
            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(loc, other.loc, sizeof(loc));
        }
        Ticket_Key(char objtrain_id[], char objloc[])
        {
            memcpy(train_id, objtrain_id, sizeof(train_id));
            memcpy(loc, objloc, sizeof(loc));
        }
        ~Ticket_Key() {}

        bool operator< (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) > 0)   return false;
            if (strcmp(train_id, other.train_id) < 0)   return true;
            if (strcmp(loc, other.loc) > 0) return false;
            if (strcmp(loc, other.loc) < 0) return true;
        }

        bool operator> (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) > 0)   return true;
            if (strcmp(train_id, other.train_id) < 0)   return false;
            if (strcmp(loc, other.loc) > 0) return true;
            if (strcmp(loc, other.loc) < 0) return false;
        }

        bool operator<= (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) > 0)   return false;
            if (strcmp(train_id, other.train_id) <= 0)   return true;
            if (strcmp(loc, other.loc) > 0) return false;
            if (strcmp(loc, other.loc) <= 0) return true;
        }

        bool operator>= (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) >= 0)   return true;
            if (strcmp(train_id, other.train_id) < 0)   return false;
            if (strcmp(loc, other.loc) >= 0) return true;
            if (strcmp(loc, other.loc) < 0) return false;
        }

        bool operator== (const Ticket_Key &other)
        {
            if (strcmp(train_id, other.train_id) != 0)  return false;
            if (strcmp(loc, other.loc) != 0)    return false;
            return true;
        }

        char* get_train_id() {return train_id;}
        char* get_loc() {return loc;}
    };

    /* Train_Key is the key used for information of a train except the station*/
    class Train_Key
    {
    private:
        char train_id[maxm];
    public:
        Train_Key()
        {
            memset(train_id, 0, sizeof(train_id));
        }
        Train_Key(const Train_Key &other)
        {
            for (int i = 0; i < maxm; i++)
            {
                train_id[i] = other.train_id[i];
            }
        }
        Train_Key(const char objtrain_id[])
        {
            for (int i = 0; i < maxm; i++)
            {
                train_id[i] = objtrain_id[i];
            }
        }
        ~Train_Key() {}

        bool operator< (const Train_Key &obj)
        {
            return strcmp(train_id, obj.train_id) < 0;
        }
        bool operator> (const Train_Key &obj)
        {
            return strcmp(train_id, obj.train_id) > 0;
        }
        bool operator<= (const Train_Key &obj)
        {
            return strcmp(train_id, obj.train_id) <= 0;
        }
        bool operator>= (const Train_Key &obj)
        {
            return strcmp(train_id, obj.train_id) >= 0;
        }
        bool operator==(const Train_Key &obj)
        {
            return strcmp(train_id, obj.train_id) == 0;
        }
        char* get_train_id() {return train_id;}
    };

    /*
    *   Station_Key is used to record the passed train of a station.
    */
    class Station_Key
    {
    private:
        char loc[maxm];
        char train_id[maxm];
        char train_type;
    public:
        Station_Key()
        {
            memset(loc, 0, sizeof(loc));
            memset(train_id, 0, sizeof(train_id));
            train_type = 0;
        }
        /*
            Special creator:
            Types:
                0 - min train_id for the specific loc.
                1 - max train_id for the specific loc.
        */
        Station_Key(char objloc[], int type)
        {
            memcpy(loc, objloc, sizeof(loc));
            if (type == 0)
            {
                memset(train_id, 0, sizeof(train_id));
            }
            else
            {
                for (int i = 0; i < maxm; i++)
                    loc[i] = 254;
            }
            train_type = 0;
        }
        Station_Key(const Station_Key &other)
        {
            memcpy(loc, other.loc, sizeof(loc));
            memcpy(train_id, other.train_id, sizeof(train_id));
            train_type = other.train_type;
        }
        Station_Key(const char objloc[], const char objtrain_id[], const char objtrain_type)
        {
            memcpy(loc, objloc, sizeof(loc));
            memcpy(train_id, objtrain_id, sizeof(train_id));
            train_type = objtrain_type;
        }
        ~Station_Key() {}

        bool operator< (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) > 0) return false;
            if (strcmp(loc, other.loc) < 0) return true;
            if (strcmp(train_id, other.train_id) > 0)   return false;
            if (strcmp(train_id, other.train_id) < 0)   return true;
        }

        bool operator> (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) < 0) return false;
            if (strcmp(loc, other.loc) > 0) return true;
            if (strcmp(train_id, other.train_id) > 0)   return true;
            if (strcmp(train_id, other.train_id) < 0)   return false;
        }
        bool operator<= (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) > 0) return false;
            if (strcmp(loc, other.loc) <= 0) return true;
            if (strcmp(train_id, other.train_id) > 0)   return false;
            if (strcmp(train_id, other.train_id) <= 0)   return true;
        }

        bool operator>= (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) < 0) return false;
            if (strcmp(loc, other.loc) >= 0) return true;
            if (strcmp(train_id, other.train_id) >= 0)   return true;
            if (strcmp(train_id, other.train_id) < 0)   return false;
        }
        bool operator== (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) != 0)    return false;
            if (strcmp(train_id, other.train_id) != 0)  return false;
            return true;
        }

        char* get_loc() {return loc;}
        char* get_train_id() {return train_id;}
        char get_train_type() {return train_type;}
    };

    /* Buyer_Key is a special key for the buyer */
    class Buyer_Key
    {
    private:
        unsigned int user_id;
        short num;
        char train_id[maxm];
        char from[maxm];
        char des[maxm];
        char ticket_kind[maxm];
        short date_year; //to store date
        char date_month;
        char date_day;

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
                    year = year * 10 + str[i] - '0';
                }
                for (int i = 0; i < 2; i++)
                {
                    month = month * 10 + str[5 + i] - '0';
                    day = day * 10 + str[8 + i] - '0';
                }
            }
        }

        char* date_to_string(short year, char month, char day)
        {
            char *tmp = new char[11];
            char invaild[11] = "xxxx-xx-xx";
            if (year == -1)
            {
                memcpy(tmp, invaild, sizeof(invaild));
                return tmp;
            }
            tmp[10] = '\0';
            tmp[4] = '-'; tmp[7] = '-';
            for (int i = 0; i < 4; i++)
            {
                tmp[3 - i] = year % 10 + '0';
                year = year / 10;
            }
            for (int i = 0; i < 2; i++)
            {
                tmp[6 - i] = month % 10 + '0';
                tmp[9 - i] = day % 10 + '0';
                month = month / 10;
                day = day / 10;
            }
            return tmp;
        }
    public:
        /*
            0 -for create min Buyer_Key
            1 -for create max Buyer_Key
            2 -for create a null Buyer_Key
        */
        Buyer_Key(unsigned int objuser_id = 0, int i = 2)
        {
            num = 0;
            user_id = objuser_id;
            if (i == 2)
            {
                memset(train_id, 0, sizeof(train_id));
                memset(from, 0, sizeof(from));
                memset(des, 0, sizeof(des));
                memset(ticket_kind, 0, sizeof(ticket_kind));
                date_year = 0; date_month = 0; date_day = 0;
            }
            else if (i == 0)
                {
                    for(int i = 0; i < maxm; i++)
                    {
                        train_id[i] = 254;
                        from[i] = 254;
                        des[i] = 254;
                        ticket_kind[i] = 254;
                    }
                    date_year = (short)65535; date_month = 254; date_day = 254;
                }
                else
                {
                    memset(train_id, 0, sizeof(train_id));
                    memset(from, 0, sizeof(from));
                    memset(des, 0, sizeof(des));
                    memset(ticket_kind, 0, sizeof(ticket_kind));
                    date_year = 0; date_month = 0; date_day = 0;
                }
        }
        Buyer_Key(const Buyer_Key &other)
        {
            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(from, other.from, sizeof(from));
            memcpy(des, other.des, sizeof(des));
            memcpy(ticket_kind, other.ticket_kind, sizeof(ticket_kind));
            user_id = other.user_id;
            num = other.num;
            date_year = other.date_year;
            date_month = other.date_month;
            date_day = other.date_day;
        }
        Buyer_Key(const unsigned int objuser_id, short objnum, const char objtrain_id[], const char objfrom[], const char objdes[], const char objticket_kind[], const char date[])
        {
            memcpy(train_id, objtrain_id, sizeof(train_id));
            memcpy(from, objfrom, sizeof(from));
            memcpy(des, objdes, sizeof(des));
            memcpy(ticket_kind, objticket_kind, sizeof(ticket_kind));
            string_to_date(date_year, date_month, date_day, date);
            user_id = objuser_id;
            num = objnum;
        }
        ~Buyer_Key() {}

        /*
            special operator
            privilege orders:
                train_id > date > from > des > ticket_kind
        */
        bool operator< (const Buyer_Key &obj)
        {
            if (user_id > obj.user_id)  return false;
            if (user_id < obj.user_id)  return true;
            if (strcmp(train_id, obj.train_id) > 0) return false;
            if (strcmp(train_id, obj.train_id) < 0) return true;
            if (date_year > obj.date_year)  return false;
            if (date_year < obj.date_year)  return true;
            if (date_month > obj.date_month)    return false;
            if (date_month < obj.date_month)    return true;
            if (date_day > obj.date_day)    return false;
            if (date_day < obj.date_day)    return true;
            if (strcmp(from, obj.from) > 0) return false;
            if (strcmp(from, obj.from) < 0) return true;
            if (strcmp(des, obj.des) > 0)   return false;
            if (strcmp(des, obj.des) < 0)   return true;
            if (strcmp(ticket_kind, obj.ticket_kind) > 0)   return false;
            if (strcmp(ticket_kind, obj.ticket_kind) < 0)   return true;
        }
        bool operator> (const Buyer_Key &obj)
        {
            if (user_id > obj.user_id)  return true;
            if (user_id < obj.user_id)  return false;
            if (strcmp(train_id, obj.train_id) > 0) return true;
            if (strcmp(train_id, obj.train_id) < 0) return false;
            if (date_year > obj.date_year)  return true;
            if (date_year < obj.date_year)  return false;
            if (date_month > obj.date_month)    return true;
            if (date_month < obj.date_month)    return false;
            if (date_day > obj.date_day)    return true;
            if (date_day < obj.date_day)    return false;
            if (strcmp(from, obj.from) > 0) return true;
            if (strcmp(from, obj.from) < 0) return false;
            if (strcmp(des, obj.des) > 0)   return true;
            if (strcmp(des, obj.des) < 0)   return false;
            if (strcmp(ticket_kind, obj.ticket_kind) > 0)   return true;
            if (strcmp(ticket_kind, obj.ticket_kind) < 0)   return false;
        }
        bool operator<= (const Buyer_Key &obj)
        {
            if (user_id > obj.user_id)  return false;
            if (user_id <= obj.user_id)  return true;
            if (strcmp(train_id, obj.train_id) > 0) return false;
            if (strcmp(train_id, obj.train_id) <= 0) return true;
            if (date_year > obj.date_year)  return false;
            if (date_year <= obj.date_year)  return true;
            if (date_month > obj.date_month)    return false;
            if (date_month <= obj.date_month)    return true;
            if (date_day > obj.date_day)    return false;
            if (date_day <= obj.date_day)    return true;
            if (strcmp(from, obj.from) > 0) return false;
            if (strcmp(from, obj.from) <= 0) return true;
            if (strcmp(des, obj.des) > 0)   return false;
            if (strcmp(des, obj.des) <= 0)   return true;
            if (strcmp(ticket_kind, obj.ticket_kind) > 0)   return false;
            if (strcmp(ticket_kind, obj.ticket_kind) <= 0)   return true;
        }
        bool operator>= (const Buyer_Key &obj)
        {
            if (user_id >= obj.user_id)  return true;
            if (user_id < obj.user_id)  return false;
            if (strcmp(train_id, obj.train_id) >= 0) return true;
            if (strcmp(train_id, obj.train_id) < 0) return false;
            if (date_year >= obj.date_year)  return true;
            if (date_year < obj.date_year)  return false;
            if (date_month >= obj.date_month)    return true;
            if (date_month < obj.date_month)    return false;
            if (date_day >= obj.date_day)    return true;
            if (date_day < obj.date_day)    return false;
            if (strcmp(from, obj.from) >= 0) return true;
            if (strcmp(from, obj.from) < 0) return false;
            if (strcmp(des, obj.des) >= 0)   return true;
            if (strcmp(des, obj.des) < 0)   return false;
            if (strcmp(ticket_kind, obj.ticket_kind) >= 0)   return true;
            if (strcmp(ticket_kind, obj.ticket_kind) < 0)   return false;
        }
        bool operator==(const Buyer_Key &obj)
        {
            if (user_id != obj.user_id) return false;
            if (strcmp(train_id, obj.train_id) != 0)    return false;
            if (date_year != obj.date_year)     return false;
            if (date_month != obj.date_month)   return false;
            if (date_day != obj.date_day)       return false;
            if (strcmp(from, obj.from) != 0)    return false;
            if (strcmp(des, obj.des) != 0)      return false;
            if (strcmp(ticket_kind, obj.ticket_kind) != 0)  return false;
            return true;
        }

        unsigned int get_user_id() {return user_id;}
        short get_num()         {return num;}
        char* get_train_id()    {return train_id;}
        char* get_from()        {return from;}
        char* get_des()         {return des;}
        char* get_ticket_kind() {return ticket_kind;}
        char* get_date()        {return date_to_string(date_year, date_month, date_day);}
    };

};
#endif
