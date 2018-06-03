#ifndef DATAS_HPP
#define DATAS_HPP
#include <cstring>
#include "constant.h"

using namespace sjtu;
    /* User_Data is the user information for registering */
    /* Using Keys: user_id */
    class User_Data
    {
        friend class User_Query;
    private:
        char name[maxn]{};
        char password[maxm]{};
        char email[maxm]{};
        char phone[maxm]{};
        char privilege; // 0 1 or 2
    public:
        User_Data()
        {
            memset(name, 0, sizeof(name));
            memset(password, 0, sizeof(password));
            memset(email, 0, sizeof(email));
            memset(phone, 0, sizeof(phone));
            privilege = 57;
        }
        User_Data(const User_Data &other)
        {
            memcpy(name, other.name, sizeof(name));
            memcpy(password, other.password, sizeof(password));
            memcpy(email, other.email, sizeof(email));
            memcpy(phone, other.phone, sizeof(phone));
            privilege = other.privilege;
        }
        User_Data(const char objname[], const char objpassword[], const char objemail[], const char objphone[], const char &objprivilege)
        {
            memcpy(name, objname, sizeof(name));
            memcpy(password, objpassword, sizeof(password));
            memcpy(email, objemail, sizeof(email));
            memcpy(phone, objphone, sizeof(phone));
            privilege = objprivilege;
        }
        ~User_Data() = default;
    
        User_Data& operator= (const User_Data &other)
        {
            if (this == &other)
                return *this;
            memcpy(name, other.name, sizeof(name));
            memcpy(password, other.password, sizeof(password));
            memcpy(email, other.email, sizeof(email));
            memcpy(phone, other.phone, sizeof(phone));
            privilege = other.privilege;
            return *this;
        }

        char* get_name() {return name;}
        char* get_passwd() {return password;}
        char* get_email() {return email;}
        char* get_phone() {return phone;}
        char  get_privilege() {return privilege;}
    };

    /* Train_Data is for the information of a train but not containing the station information. */
    /* Using Keys: Train_Key*/
    class Train_Data
    {
        friend class Train_Query;
    private:
        char train_name[maxn]{};
        char catalog[maxc]{};
        char ticket_kind[maxe][maxm]{};
        char nstation;
        char nprice;
        bool is_sale;
    public:
        Train_Data()
        {
            memset(train_name, 0, sizeof(train_name));
            memset(catalog, 0, sizeof(catalog));
            for (int i = 0;  i < maxe; i++)
            {
                memset(ticket_kind[i], 0, sizeof(ticket_kind[i]));
            }
            nstation = 0; nprice = 0;
            is_sale = false;
        }
        Train_Data(const Train_Data &other)
        {
            memcpy(train_name, other.train_name, sizeof(train_name));
            memcpy(catalog, other.catalog, sizeof(catalog));
            nstation = other.nstation;
            nprice = other.nprice;
            is_sale = other.is_sale;
            for (int i = 0; i < nprice; i++)
            {
                memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
            }
        }
//        Train_Data(const Train_Query &qtra)
//        {
//            memcpy(train_name, qtra.train_name, sizeof(train_name));
//            memcpy(catalog, qtra.catalog, sizeof(catalog));
//            nstation = qtra.nstation;
//            nprice = qtra.nprice;
//            for (int i = 0; i < nprice; i++)
//                memcpy(ticket_kind[i], qtra.ticket_kind[i], sizeof(ticket_kind[i]));
//            is_sale = false;
//        }
        Train_Data(const char objname[], char objticket_kind[][maxm], char objcatalog[], const int objstation, const int objprice)
        {
            memcpy(train_name, objname, sizeof(train_name));
            memcpy(catalog, objcatalog, sizeof(catalog));
            for (int i = 0; i < objprice; i++)
            {
                memcpy(ticket_kind[i], objticket_kind[i], sizeof(ticket_kind[i]));
            }
            nstation = (char)objstation;
            nprice = (char)objprice;
            is_sale = false;
        }
        ~Train_Data() = default;
	
	    Train_Data& operator= (const Train_Data &other)
        {
            if (this == &other)
                return *this;
            memset(train_name, 0, sizeof(train_name));
            memset(catalog, 0, sizeof(catalog));
            for (int i = 0;  i < maxe; i++)
            {
                memset(ticket_kind[i], 0, sizeof(ticket_kind[i]));
            }

            memcpy(train_name, other.train_name, sizeof(train_name));
            memcpy(catalog, other.catalog, sizeof(catalog));
            for (int i = 0; i < maxe; i++)
            {
                memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
            }
            nstation = other.nstation;
            nprice = other.nprice;
            is_sale = other.is_sale;
            return *this;
        }

        /*Used for get or modify private data*/
        void make_sale()
        {
            is_sale = true;
        }
        bool get_sale()                     {return is_sale;}
        char* get_train_name()              {return train_name;}
        char* get_ticket_kind(int i)  {return ticket_kind[i];}
        char** get_origin_ticket_kind()
        {
            char **tmp = new char*[maxe];
            for (int i = 0; i < maxe; i++)
            {
                tmp[i] = new char[maxm];
            }
            return tmp;
        }
        int get_kind_num(char objticket_kind[])
        {
            for (int i = 0; i < nprice; i++)
            {
                if (strcmp(objticket_kind, ticket_kind[i]) == 0)
                    return i;
            }
            return -1;
        }
        char* get_catalog()             {return catalog;}
        int get_price_num()             {return (int)nprice;}
        int get_station_num()           {return (int)nstation;}
    };

    /* Ticket_Data is specially designed for the station information of a train */
    /* Using Keys: Ticket_Key */
    class Ticket_Data
    {
    private:
        char arrtime[2]{}; //arrive time of a train on a specific station
        char statime[2]{}; //start time of a train on a specific station
        char stpover[2]{}; //stop time of a train on a specific station
        double price[maxe]{};

        /*Covert the time to time_string for output*/
        char* time_to_string(const char times[])
        {
            char *tmp = new char[6];
            char invaild[6] = "xx:xx";
            if (times[0] >= 100)
            {
                strcpy(tmp, invaild);
                return tmp;
            }
            else
            {
                tmp[5] = '\0';
                tmp[2] = ':';
                tmp[0] = static_cast<char>(times[0] / 10 + '0');
                tmp[1] = static_cast<char>(times[0] % 10 + '0');
                tmp[3] = static_cast<char>(times[1] / 10 + '0');
                tmp[4] = static_cast<char>(times[1] % 10 + '0');
            }
            return tmp;
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

    public:
        Ticket_Data()
        {
            memset(arrtime, 0, sizeof(arrtime));
            memset(statime, 0, sizeof(statime));
            memset(stpover, 0, sizeof(stpover));
            memset(price, 0, sizeof(price));
        }
        Ticket_Data(const Ticket_Data &other)
        {
            memcpy(arrtime, other.arrtime, sizeof(arrtime));
            memcpy(statime, other.statime, sizeof(arrtime));
            memcpy(stpover, other.stpover, sizeof(stpover));
            memcpy(price, other.price, sizeof(price));
        }
        /* Waring: objprice must have exactly maxe elements for memcpy */
        Ticket_Data(const char start[], const char stop[], const char over[], const double objprice[])
        {
            string_to_time(arrtime, start);
            string_to_time(statime, stop);
            string_to_time(stpover, over);
            memcpy(price, objprice, sizeof(price));
        }
    
//        explicit Ticket_Data(const Station_Query &qsta)
//        {
//            memcpy(arrtime, qsta.arrtime, sizeof(arrtime));
//            memcpy(statime, qsta.statime, sizeof(statime));
//            memcpy(stpover, qsta.stpover, sizeof(stpover));
//            memcpy(price, qsta.price, sizeof(price));
//        }
        ~Ticket_Data() = default;
    
        Ticket_Data& operator= (const Ticket_Data &other)
        {
            if (this == &other)
                return *this;
            memset(arrtime, 0, sizeof(arrtime));
            memset(statime, 0, sizeof(statime));
            memset(stpover, 0, sizeof(stpover));
            memset(price, 0, sizeof(price));

            memcpy(arrtime, other.arrtime, sizeof(arrtime));
            memcpy(statime, other.statime, sizeof(arrtime));
            memcpy(stpover, other.stpover, sizeof(stpover));
            memcpy(price, other.price, sizeof(price));
            return *this;
        }

        char* get_arrtime() {return arrtime;}
        char* get_statime() {return statime;}
        char* get_stpover() {return stpover;}
        double get_price(int num) {return price[num];}
        double* get_origin_price() {return price;}
    };

    /*
    *   Ticket_Sale_Data is for the Ticket_Sale_Key
    *   recording the num of the for bought ticket
    */
    class Ticket_Sale_Data
    {
    private:
        short num[maxe]{};
    public:
        Ticket_Sale_Data()
        {
            memset(num, 0, sizeof(num));
        }
        Ticket_Sale_Data(const Ticket_Sale_Data &other)
        {
            memcpy(num, other.num, sizeof(num));
        }
        Ticket_Sale_Data(short objnum[])
        {
            memcpy(num, objnum, sizeof(num));
        }
        ~Ticket_Sale_Data() = default;
    
        Ticket_Sale_Data& operator= (const Ticket_Sale_Data &other)
        {
            if (this == &other)
                return *this;
            memset(num, 0, sizeof(num));
            memcpy(num, other.num, sizeof(num));
            return *this;
        }

        short get_num(int i)    {return num[i];}
    };
    
#endif
