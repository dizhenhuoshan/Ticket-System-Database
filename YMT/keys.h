#ifndef KEYS_HPP
#define KEYS_HPP
#include <cstring>
#include "constant.h"

using namespace sjtu;
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
    
        explicit User_id_Key(const unsigned int num):user_id(num) {}
        ~User_id_Key() = default;
    
        User_id_Key& operator= (const User_id_Key &other)
        {
            if (this == &other)
                return *this;
            user_id = other.user_id;
            return *this;
        }
        
        bool operator== (const User_id_Key &other) const
        {
            return user_id == other.user_id;
        }
        bool operator!= (const User_id_Key &other) const
        {
            return user_id != other.user_id;
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
        char train_id[maxm]{};
        char loc[maxm]{};
        char order; // 1-based order, recording the order of the passed station;
    public:
        Ticket_Key()
        {
            memset(train_id, 0, sizeof(train_id));
            memset(loc, 0, sizeof(loc));
            order = 0;
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
                order = 0;
            }
            else
            {
                memset(loc, 0, sizeof(loc));
                order = static_cast<char>(254);
            }
        }
        Ticket_Key(const Ticket_Key &other)
        {
            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(loc, other.loc, sizeof(loc));
            order = other.order;
        }
        Ticket_Key(char objtrain_id[], char objloc[], char objorder)
        {
            memcpy(train_id, objtrain_id, sizeof(train_id));
            memcpy(loc, objloc, sizeof(loc));
            order = objorder;
        }
        ~Ticket_Key() = default;
    
        Ticket_Key& operator= (const Ticket_Key &other)
        {
            if (this == &other)
                return *this;
            memset(train_id, 0, sizeof(train_id));
            memset(loc, 0, sizeof(loc));
            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(loc, other.loc, sizeof(loc));
            order = other.order;
            return *this;
        }

        bool operator< (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) >= 0)   return false;
            return order < other.order;
        }

        bool operator> (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) < 0)   return false;
            return order <= other.order;
        }

        bool operator<= (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) > 0)   return false;
            return order <= other.order;
        }

        bool operator>= (const Ticket_Key &other) const
        {
            if (strcmp(train_id, other.train_id) < 0)   return false;
            return order >= other.order;
        }

        bool operator== (const Ticket_Key &other)
        {
            if (strcmp(train_id, other.train_id) != 0)  return false;
            return order == other.order;
        }
        
        bool operator!= (const Ticket_Key &other)
        {
            if (strcmp(train_id, other.train_id) != 0)  return true;
            return order != other.order;
        }

        char* get_train_id() {return train_id;}
        char* get_loc()      {return loc;}
        char get_order()     {return order;}
    };

    /* Train_Key is the key used for information of a train except the station*/
    class Train_Key
    {
    private:
        char train_id[maxm]{};
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
    
        explicit Train_Key(const char objtrain_id[])
        {
            for (int i = 0; i < maxm; i++)
            {
                train_id[i] = objtrain_id[i];
            }
        }
        ~Train_Key() = default;
    
        Train_Key& operator= (const Train_Key &other)
        {
            if (this == &other)
                return *this;
            memset(train_id, 0, sizeof(train_id));
            memcpy(train_id, other.train_id, sizeof(train_id));
            return *this;
        }

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
        bool operator!=(const Train_Key &obj)
        {
            return strcmp(train_id, obj.train_id) != 0;
        }
        char* get_train_id() {return train_id;}
    };

    /*
    *   Station_Key is used to record the passed train of a station.
    */
    class Station_Key
    {
    private:
        char loc[maxm]{};
        char train_id[maxm]{};
    public:
        Station_Key()
        {
            memset(loc, 0, sizeof(loc));
            memset(train_id, 0, sizeof(train_id));
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
                    loc[i] = static_cast<char>(254);
            }
        }
        Station_Key(const Station_Key &other)
        {
            memcpy(loc, other.loc, sizeof(loc));
            memcpy(train_id, other.train_id, sizeof(train_id));
        }
        Station_Key(const char objloc[], const char objtrain_id[])
        {
            memcpy(loc, objloc, sizeof(loc));
            memcpy(train_id, objtrain_id, sizeof(train_id));
        }
        ~Station_Key() = default;
    
        Station_Key& operator= (const Station_Key &other)
        {
            if (this == &other)
                return *this;
            memset(loc, 0, sizeof(loc));
            memset(train_id, 0, sizeof(train_id));
            memcpy(loc, other.loc, sizeof(loc));
            memcpy(train_id, other.train_id, sizeof(train_id));
            return *this;
        }

        bool operator< (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) > 0) return false;
            return strcmp(train_id, other.train_id) < 0;
        }

        bool operator> (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) < 0) return false;
            return strcmp(train_id, other.train_id) > 0;
        }
        bool operator<= (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) > 0) return false;
            return strcmp(train_id, other.train_id) <= 0;
        }

        bool operator>= (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) < 0) return false;
            return strcmp(train_id, other.train_id) >= 0;
        }
        bool operator== (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) != 0)    return false;
            return strcmp(train_id, other.train_id) == 0;
        }
        bool operator!= (const Station_Key &other)
        {
            if (strcmp(loc, other.loc) != 0)    return true;
            return strcmp(train_id, other.train_id) != 0;
        }

        char* get_loc() {return loc;}
        char* get_train_id() {return train_id;}
    };

    /* Buyer_Key is a special key for the buyer */
    class Buyer_Key
    {
    private:
        char train_id[maxm]{};
        char from[maxm]{};
        char des[maxm]{};
        char ticket_kind[maxm]{};
        unsigned int user_id;
        short date_year{}; //to store date in from
        char date_month{};
        char date_day{};

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

    public:
        /*
            0 -for create min Buyer_Key
            1 -for create max Buyer_Key
            2 -for create a null Buyer_Key
        */
        explicit Buyer_Key(unsigned int objuser_id = 0, int i = 2)
        {
            user_id = objuser_id;
            memset(ticket_kind, 0, sizeof(ticket_kind));
            if (i == 2)
            {
                memset(train_id, 0, sizeof(train_id));
                memset(from, 0, sizeof(from));
                memset(des, 0, sizeof(des));
                date_year = 0; date_month = 0; date_day = 0;
            }
            else
            {
                if (i == 0)
                {
                    for (int j = 0; j < maxm; j++)
                    {
                        train_id[j] = static_cast<char>(254);
                        from[j] = static_cast<char>(254);
                        des[j] = static_cast<char>(254);
                        ticket_kind[j] = static_cast<char>(254);
                    }
                    date_year = (short) 65535;
                    date_month = static_cast<char>(254);
                    date_day = static_cast<char>(254);
                } else
                {
                    memset(train_id, 0, sizeof(train_id));
                    memset(from, 0, sizeof(from));
                    memset(des, 0, sizeof(des));
                    date_year = 0;
                    date_month = 0;
                    date_day = 0;
                }
            }
        }
        Buyer_Key(const Buyer_Key &other)
        {
            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(from, other.from, sizeof(from));
            memcpy(des, other.des, sizeof(des));
            memcpy(ticket_kind, other.ticket_kind, sizeof(ticket_kind));
            user_id = other.user_id;
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
        }
        ~Buyer_Key() = default;
    
        Buyer_Key& operator=(const Buyer_Key &other)
        {
            if (this == &other)
                return *this;
            memset(train_id, 0, sizeof(train_id));
            memset(from, 0, sizeof(from));
            memset(des, 0, sizeof(des));
            memset(ticket_kind, 0, sizeof(ticket_kind));

            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(from, other.from, sizeof(from));
            memcpy(des, other.des, sizeof(des));
            memcpy(ticket_kind, other.ticket_kind, sizeof(ticket_kind));
            user_id = other.user_id;
            date_year = other.date_year;
            date_month = other.date_month;
            date_day = other.date_day;
            return *this;
        }

        /*
            special operator
            privilege orders:
                train_id > date > from > des > ticket_kind
        */
        bool operator< (const Buyer_Key &obj)
        {
            if (user_id > obj.user_id)  return false;
            if (strcmp(train_id, obj.train_id) > 0) return false;
            if (date_year > obj.date_year)  return false;
            if (date_month > obj.date_month)    return false;
            if (date_day > obj.date_day)    return false;
            if (strcmp(from, obj.from) > 0) return false;
            if (strcmp(des, obj.des) > 0)   return false;
            return strcmp(ticket_kind, obj.ticket_kind) < 0;
        }
        bool operator> (const Buyer_Key &obj)
        {
            if (user_id < obj.user_id)  return false;
            if (strcmp(train_id, obj.train_id) < 0) return false;
            if (date_year < obj.date_year)  return false;
            if (date_month < obj.date_month)    return false;
            if (date_day < obj.date_day)    return false;
            if (strcmp(from, obj.from) < 0) return false;
            if (strcmp(des, obj.des) < 0)   return false;
            return strcmp(ticket_kind, obj.ticket_kind) > 0;
        }
        bool operator<= (const Buyer_Key &obj)
        {
            if (user_id > obj.user_id)  return false;
            if (strcmp(train_id, obj.train_id) > 0) return false;
            if (date_year > obj.date_year)  return false;
            if (date_month > obj.date_month)    return false;
            if (date_day > obj.date_day)    return false;
            if (strcmp(from, obj.from) > 0) return false;
            if (strcmp(des, obj.des) > 0)   return false;
            return strcmp(ticket_kind, obj.ticket_kind) <= 0;
        }
        bool operator>= (const Buyer_Key &obj)
        {
            if (user_id < obj.user_id)  return false;
            if (strcmp(train_id, obj.train_id) < 0) return false;
            if (date_year < obj.date_year)  return false;
            if (date_month < obj.date_month)    return false;
            if (date_day < obj.date_day)    return false;
            if (strcmp(from, obj.from) < 0) return false;
            if (strcmp(des, obj.des) < 0)   return false;
            return strcmp(ticket_kind, obj.ticket_kind) >= 0;
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
            return strcmp(ticket_kind, obj.ticket_kind) == 0;
        }
        bool operator!=(const Buyer_Key &obj)
        {
            if (user_id != obj.user_id) return false;
            if (strcmp(train_id, obj.train_id) != 0)    return true;
            if (date_year != obj.date_year)     return true;
            if (date_month != obj.date_month)   return true;
            if (date_day != obj.date_day)       return true;
            if (strcmp(from, obj.from) != 0)    return true;
            if (strcmp(des, obj.des) != 0)      return true;
            return strcmp(ticket_kind, obj.ticket_kind) != 0;
        }

        unsigned int get_user_id() {return user_id;}
        char* get_train_id()    {return train_id;}
        char* get_from()        {return from;}
        char* get_des()         {return des;}
        char* get_ticket_kind() {return ticket_kind;}
        short get_year()        {return date_year;}
        char get_month()        {return date_month;}
        char get_day()          {return date_day;}
    };

    //Ticket_Sale_Key is to store the
    class Ticket_Sale_Key
    {
    private:
        char train_id[maxm]{};
        char station[maxm]{};
        short num[maxe]{}; //store the left ticket in the station of the train
        short date_year{}; //to store date
        char date_month{};
        char date_day{};

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
    public:
        Ticket_Sale_Key()
        {
            memset(train_id, 0, sizeof(train_id));
            memset(station, 0, sizeof(station));
            date_year = 0; date_month = 0; date_day = 0;
            for (int i = 0; i < maxe; i++)
                num[i] = origin_ticket;
        }
        Ticket_Sale_Key(const Ticket_Sale_Key &other)
        {
            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(station, other.station, sizeof(station));
            date_year = other.date_year;
            date_month = other.date_month;
            date_day = other.date_day;
            for (int i = 0; i < maxe; i++)
                num[i] = other.num[i];
        }
        Ticket_Sale_Key(const char objtrain_id[], const char objstation[], const char date[])
        {
            memcpy(train_id, objtrain_id, sizeof(train_id));
            memcpy(station, objstation, sizeof(station));
            string_to_date(date_year, date_month, date_day, date);
            memset(num, 0, sizeof(num));
        }
        Ticket_Sale_Key(const char objtrain_id[], const char objstation[], const char date[], short objnum[])
        {
            memcpy(train_id, objtrain_id, sizeof(train_id));
            memcpy(station, objstation, sizeof(station));
            string_to_date(date_year, date_month, date_day, date);
            for (int i = 0; i < maxe; i++)
                num[i] = objnum[i];
        }
        ~Ticket_Sale_Key() = default;
    
        Ticket_Sale_Key& operator= (const Ticket_Sale_Key &other)
        {
            if (this == &other)
                return *this;
            memset(train_id, 0, sizeof(train_id));
            memset(station, 0, sizeof(station));

            memcpy(train_id, other.train_id, sizeof(train_id));
            memcpy(station, other.station, sizeof(station));
            date_year = other.date_year;
            date_month = other.date_month;
            date_day = other.date_day;
            for (int i = 0; i < maxe; i++)
                num[i] = other.num[i];
            return *this;
        }

        /*
            special operator
            privilege orders:
                train_id > date > from > des > ticket_kind
        */
        bool operator< (const Ticket_Sale_Key &obj)
        {
            if (strcmp(train_id, obj.train_id) > 0) return false;
            if (date_year > obj.date_year)  return false;
            if (date_month > obj.date_month)    return false;
            return date_day < obj.date_day;
        }
        bool operator> (const Ticket_Sale_Key &obj)
        {
            if (strcmp(train_id, obj.train_id) < 0) return false;
            if (date_year < obj.date_year)  return false;
            if (date_month < obj.date_month)    return false;
            return date_day > obj.date_day;
        }
        bool operator<= (const Ticket_Sale_Key &obj)
        {
            if (strcmp(train_id, obj.train_id) > 0) return false;
            if (date_year > obj.date_year)  return false;
            if (date_month > obj.date_month)    return false;
            return date_day <= obj.date_day;
        }
        bool operator>= (const Ticket_Sale_Key &obj)
        {
            if (strcmp(train_id, obj.train_id) < 0) return false;
            if (date_year < obj.date_year)  return false;
            if (date_month < obj.date_month)    return false;
            return date_day >= obj.date_day;
        }
        bool operator==(const Ticket_Sale_Key &obj)
        {
            if (strcmp(train_id, obj.train_id) != 0)    return false;
            if (date_year != obj.date_year)     return false;
            if (date_month != obj.date_month)   return false;
            return date_day == obj.date_day;
        }
        bool operator!=(const Ticket_Sale_Key &obj)
        {
            if (strcmp(train_id, obj.train_id) != 0)    return true;
            if (date_year != obj.date_year)     return true;
            if (date_month != obj.date_month)   return true;
            return date_day != obj.date_day;
        }
    };
    
#endif
