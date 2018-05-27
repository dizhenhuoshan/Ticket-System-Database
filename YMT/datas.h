#ifndef DATAS_HPP
#define DATAS_HPP
#include <cstring>
#include "constant.h"

namespace sjtu
{
    /* User_Data is the user information for registering */
    /* Using Keys: user_id */
    class User_Data
    {
    private:
        char name[maxn];
        char password[maxm];
        char email[maxm];
        char phone[maxm];
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
        }
        User_Data(const char objname[], const char objpassword[], const char objemail[], const char objphone[], const char objprivilege)
        {
            memcpy(name, objname, sizeof(name));
            memcpy(password, objpassword, sizeof(password));
            memcpy(email, objemail, sizeof(email));
            memcpy(phone, objphone, sizeof(phone));
            privilege = objprivilege;
        }
        ~User_Data() {}

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
    private:
        char train_name[maxn];
        char catalog[maxc];
        char ticket_kind[maxe][maxm];
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
            for (int i = 0; i < maxe; i++)
            {
                memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
            }
            nstation = other.nstation;
            nprice = other.nprice;
            is_sale = other.is_sale;
        }
        Train_Data(const Train_Data &other, bool flag)
        {
            memcpy(train_name, other.train_name, sizeof(train_name));
            memcpy(catalog, other.catalog, sizeof(catalog));
            for (int i = 0; i < maxe; i++)
            {
                memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
            }
            nstation = other.nstation;
            nprice = other.nprice;
            is_sale = flag;
        }
        Train_Data(const Train_Query &qtra)
        {
            memcpy(train_name, qtra.train_name, sizeof(train_name));
            memcpy(catalog, qtra.catalog, sizeof(catalog));
            nstation = qtra.nstation;
            nprice = qtra.nprice;
            for (int i = 0; i < nprice; i++)
                memcpy(ticket_kind[i], qtra.ticket_kind[i], sizeof(ticket_kind[i]));
            is_sale = false;
        }
        Train_Data(const char objname[], const char objticket_kind[][maxm], const char objcatalog[], const char objstation, const char objprice )
        {
            memcpy(train_name, objname, sizeof(train_name));
            memcpy(catalog, objcatalog, sizeof(catalog));
            for (int i = 0; i < objprice; i++)
            {
                memcpy(ticket_kind[i], objticket_kind[i], sizeof(ticket_kind[i]));
            }
            nstation = objstation;
            nprice = objprice;
            is_sale = false;
        }
        ~Train_Data() {}

        /*Used for get private data*/
        bool sale()
        {
            if (!is_sale)
            {
                is_sale = true;
                return true;
            }
            else return false;
        }
        bool get_sale() {return is_sale;}
        char* get_train_name() {return train_name;}
        char* get_ticket_kind(int i) {return ticket_kind[i];}
        char* get_catalog() {return catalog;}
        int get_price_num() {return (int)nprice;}
        int get_station_num() {return (int)nstation;}
    };

    /* Ticket_Data is specially designed for the station information of a train */
    /* Using Keys: Ticket_Key */
    class Ticket_Data
    {
    private:
        short num[maxe];
        char arrtime[2]; //arrive time of a train on a specific station
        char statime[2]; //start time of a train on a specific station
        char stpover[2]; //stop time of a train on a specific station
        double price[maxe];

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
                tmp[0] = times[0] / 10 + '0';
                tmp[1] = times[0] % 10 + '0';
                tmp[3] = times[1] / 10 + '0';
                tmp[4] = times[1] % 10 + '0';
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
                times[0] = (str[0] - '0') * 10 + (str[1] - '0');
                times[1] = (str[3] - '0') * 10 + (str[4] - '0');
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
            for (int i = 0; i < maxe; i++)
                num[i] = 2000;
        }
        Ticket_Data(const Ticket_Data &other)
        {
            memcpy(arrtime, other.arrtime, sizeof(arrtime));
            memcpy(statime, other.statime, sizeof(arrtime));
            memcpy(stpover, other.stpover, sizeof(stpover));
            memcpy(price, other.price, sizeof(price));
            for (int i = 0; i < maxe; i++)
                num = other.num;
        }
        /* Waring: objprice must have exactly maxe elements for memcpy */
        Ticket_Data(const char start[], const char stop[], const char over[], const double objprice[])
        {
            string_to_time(arrtime, start);
            string_to_time(statime, stop);
            string_to_time(stpover, over);
            memcpy(price, objprice, sizeof(price));
            for (int i = 0; i < maxe; i++)
                num[i] = 2000;
        }
        Ticket_Data(const Station_Query &qsta)
        {
            memcpy(arrtime, qsta.arrtime, sizeof(arrtime));
            memcpy(statime, qsta.statime, sizeof(statime));
            memcpy(stpover, qsta.stpover, sizeof(stpover));
            memcpy(price, qsta.price, sizeof(price));
            for (int i = 0; i < maxe; i++)
                num[i] = 2000;
        }
        ~Ticket_Data() {}

        bool buy_ticket(int cnt, int kind)
        {
            if (cnt > num[i])
                return false;
            else
            {
                num -= cnt[i];
                return true;
            }
        }
        char* get_arrtime() {return arrtime;}
        char* get_statime() {return statime;}
        char* get_stpover() {return stpover;}
        int get_num(int i) {return num[i];}
        double get_price(int num) {return price[num];}
    };

    /*
    *   Empty is for buyer database and station database for better performance.
    */

    class Empty{};
};
#endif
