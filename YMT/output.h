#ifndef OUTPUT_HPP
#define OUTPUT_HPP
#include "constant.h"
#include <cstring>

class Ticket_Query
{
private:
    char train_id[maxm];
    char from[maxm];
    char from_sta[2]; //start time of a train on a specific station
    short from_y; //to store date
    char from_m;
    char from_d;
    char des[maxm];
    char des_arr[2]; //arrive time of a train on a specific station
    short des_y; //to store date
    char des_m;
    char des_d;
    char ticket_kind[maxe][maxm];
    short num[maxe];
    double price[maxe];

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

    Ticket_Query()
    {
        memset(train_id, 0, sizeof(train_id));
        memset(from, 0, sizeof(from));
        memset(des, 0, sizeof(des));
        for (int i = 0; i < maxe; i++)
            memset(ticket_kind[i], 0, sizeof(ticket_kind[i]));
        memset(num, 0, sizeof(num));
        memset(price, 0, sizeof(price));
        nprice = 0;
    }

    Ticket_Query(const Ticket_Query &other)
    {
        memcpy(train_id, other.train_id, sizeof(train_id));
        memcpy(from, other.from, sizeof(from));
        memcpy(from_sta, other.from_sta, sizeof(from_sta));
        from_y = other.from_y;
        from_m = other.from_m;
        from_d = other.from_d;
        memcpy(des, other.des, sizeof(des));
        memcpy(des_arr, other.des_arr, sizeof(des_arr));
        des_y = other.des_y;
        des_m = other.des_m;
        des_d = other.des_d;
        for (int i = 0; i < maxe; i++)
            memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
        memcpy(num, other.num, sizeof(num));
        memcpy(price, other.price, sizeof(price));
        nprice = other.nprice;
    }

    Ticket_Query(char objtrain_id[], char objfrom[], char objfrom_sta[], char from_date[], char objdes[], char des_arr[], char objticket_kind[][maxm], short objnum[], double objprice[], int objnprice)
    {
        nprice = objnprice;
        memcpy(train_id, objtrain_id, sizeof(train_id));
        memcpy(from, objfrom, sizeof(from));
        memcpy(des, objdes, sizeof(des));
        memcpy(num, objnum, sizeof(num));
        memcpy(price, objprice, sizeof(price));
        for (int i = 0; i < nprice; i++)
        {
            memcpy(ticket_kind[i], objticket_kind[i], sizeof(ticket_kind[i]));
        }
        string_to_time(from_sta, objfrom_sta);
        string_to_time(des_sta, objdes_sta);
        string_to_date(from_y, from_m, from_d, from_date);
        string_to_date(des_y, des_m, des_d, from_date);
        if (from_arr[0] > from_sta[0] || from_sta[0] > des_arr[0])
            des_d++;
    }

    ~Ticket_Query() {}

    void mycout()
    {
        char *d_from = date_to_string(from_y, from_m, from_d);
        char *d_des = date_to_string(des_y, des_m, des_d);
        char *t_from = time_to_string(from_sta);
        char *t_des = time_to_string(des_arr);

        cout << train_id << ' ' << from << ' ' << d_from << ' ' << t_from << ' ' << d_des << ' ' << t_des;
        for (int i = 0; i < nprice; i++)
        {
            cout << ticket_kind[i] << ' ' << num[i] << ' ' << price[i] << ' ';
        }
        cout << endl;
        delete d_from;
        delete d_des;
        delete t_from;
        delete t_des;
    }
};

class Train_Query
{
public:
    char train_id[maxm];
    char train_name[maxm];
    char catalog[maxc];
    int nstation;
    int nprice;
    char ticket_kind[maxe][maxm];

    Train_Query()
    {
        memset(train_id, 0, sizeof(train_id));
        memset(train_name, 0, sizeof(train_name));
        memset(catalog, 0, sizeof(catalog));
        nstation = 0; nprice = 0;
        for (int i = 0; i < maxe; i++)
            memset(ticket_kind[i], 0, sizeof(ticket_kind[i]));
    }
    Train_Query(const Train_Query &other)
    {
        memcpy(train_id, other.train_id, sizeof(train_id));
        memcpy(train_name, other.train_name, sizeof(train_name));
        memcpy(catalog, other.catalog, sizeof(catalog));
        nstation = other.nstation;
        nprice = other.nprice;
        for (int i = 0; i < maxe; i++)
            memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
    }
    Train_Query(char objtrain_id[], char objtrain_name[], char objcatalog[], int objnstation, int objnprice, char objticket_kind[][maxm])
    {
        memcpy(train_id, objtrain_id, sizeof(train_id));
        memcpy(train_name, objtrain_name, sizeof(train_name));
        memcpy(catalog, objcatalog, sizeof(catalog));
        nstation = objstation;
        nprice = objprice;
        for (int i = 0; i < nprice; i++)
            memcpy(ticket_kind[i], objticket_kind[i], sizeof(ticket_kind[i]));
    }

    void mycout()
    {
        cout << train_id << ' ' << train_name << ' ' << catalog << ' ';
        cout << nstation << ' ' << nprice << ' ';
        for (int i = 0; i < nprice; i++)
            cout << ticket_kind[i] << ' ';
        cout << endl;
    }
};

class Station_Query
{
public:
    int nprice;
    char station[maxm];
    char arrtime[2];
    char statime[2];
    char stpover[2];
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

    Station_Query()
    {
        memset(station, 0, sizeof(station));
        memset(arrtime, 0, sizeof(arrtime));
        memset(statime, 0, sizeof(statime));
        memset(stpover, 0, sizeof(stpover));
        memset(price, 0, sizeof(price));
        nprice = 0;
    }
    Station_Query(const Station_Query &other)
    {
        memcpy(station, other.station, sizeof(station));
        memcpy(arrtime, other.arrtime, sizeof(arrtime));
        memcpy(statime, other.statime, sizeof(statime));
        memcpy(stpover, other.stpover, sizeof(stpover));
        memcpy(price, other.price, sizeof(price));
        nprice = other.nprice;
    }
    Station_Query(char objstation[], char arrive[], char start[], char over[], double objprice[])
    {
        memcpy(station, objstation, sizeof(station));
        memcpy(price, objprice, sizeof(price));
        string_to_time(arrtime, arrive);
        string_to_time(statime, start);
        string_to_time(stpover, over);
    }

    void mycout()
    {
        cout << station << ' ';
        char *atime = time_to_string(arrtime);
        char *stime = time_to_string(statime);
        char *otime = time_to_string(stpover);
        cout << atime << ' ' << stime << ' ' << otime << ' ';
        for (int i = 0; i < nprice; i++)
        {
            cout << "￥" << price[i] << ' ';
        }
        cout << endl;
        delete atime;
        delete stime;
        delete otime;
    }
};

#endif
