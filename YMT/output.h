#ifndef OUTPUT_HPP
#define OUTPUT_HPP
#include <iostream>
#include <cstring>
#include "constant.h"
#include "keys.h"
#include "datas.h"

using namespace sjtu;

class User_Query
{
    friend class User_Data;
public:
    char name[maxn]{};
    char email[maxm]{};
    char phone[maxm]{};
    char privilege;

    User_Query()
    {
        privilege = 0;
        memset(name, 0, sizeof(name));
        memset(email, 0, sizeof(email));
        memset(phone, 0, sizeof(phone));
    }

    User_Query(const User_Query &other)
    {
        privilege = other.privilege;
        memcpy(name, other.name, sizeof(name));
        memcpy(email, other.email, sizeof(email));
        memcpy(phone, other.phone, sizeof(phone));
    }

    User_Query(const User_Data &data)
    {
        privilege = data.privilege;
        memcpy(name, data.name, sizeof(name));
        memcpy(email, data.email, sizeof(email));
        memcpy(phone, data.phone, sizeof(phone));
    }

    User_Query(char objname[], const char &objprivilege, char objemail[], char objphone[])
    {
        privilege = objprivilege;
        memcpy(name, objname, sizeof(name));
        memcpy(email, objemail, sizeof(email));
        memcpy(phone, objphone, sizeof(phone));
    }

    ~User_Query() = default;
    
    User_Query& operator= (const User_Query &other)
    {
        if (this == &other)
            return *this;
        privilege = other.privilege;
        memcpy(name, other.name, sizeof(name));
        memcpy(email, other.email, sizeof(email));
        memcpy(phone, other.phone, sizeof(phone));
        return *this;
    }

    void mycout()
    {
        std::cout << name << ' ' << email << ' ' << phone << ' ' << static_cast<char>(privilege + '0') << std::endl;
    }
};

class Ticket_Query
{
public:
    char train_id[maxm]{};
    char from[maxm]{};
    char time_sta[2]{}; //start time of a train on a specific station
    short from_y{}; //to store date
    char from_m{};
    char from_d{};
    char des[maxm]{};
    char time_arr[2]{}; //arrive time of a train on a specific station
    short des_y{}; //to store date
    char des_m{};
    char des_d{};
    char nprice;
    char ticket_kind[maxe][maxm]{};
    short num[maxe]{};
    double price[maxe]{};

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
            tmp[3 - i] = static_cast<char>(year % 10 + '0');
            year = static_cast<short>(year / 10);
        }
        for (int i = 0; i < 2; i++)
        {
            tmp[6 - i] = static_cast<char>(month % 10 + '0');
            tmp[9 - i] = static_cast<char>(day % 10 + '0');
            month = static_cast<char>(month / 10);
            day = static_cast<char>(day / 10);
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
        memcpy(time_sta, other.time_sta, sizeof(time_sta));
        from_y = other.from_y;
        from_m = other.from_m;
        from_d = other.from_d;
        memcpy(des, other.des, sizeof(des));
        memcpy(time_arr, other.time_arr, sizeof(time_arr));
        des_y = other.des_y;
        des_m = other.des_m;
        des_d = other.des_d;
        for (int i = 0; i < maxe; i++)
            memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
        memcpy(num, other.num, sizeof(num));
        memcpy(price, other.price, sizeof(price));
        nprice = other.nprice;
    }

    Ticket_Query(char objtrain_id[], char objfrom[], char objtime_sta[], char from_date[], char objdes[], char objtime_arr[], char** objticket_kind, short objnum[], double objprice[], int objnprice)
    {
        nprice = static_cast<char>(objnprice);
        memcpy(train_id, objtrain_id, sizeof(train_id));
        memcpy(from, objfrom, sizeof(from));
        memcpy(des, objdes, sizeof(des));
        memcpy(num, objnum, sizeof(num));
        memcpy(price, objprice, sizeof(price));
        for (int i = 0; i < nprice; i++)
        {
            memcpy(ticket_kind[i], objticket_kind[i], sizeof(ticket_kind[i]));
        }
        string_to_time(time_sta, objtime_sta);
        string_to_time(time_arr, objtime_arr);
        string_to_date(from_y, from_m, from_d, from_date);
        string_to_date(des_y, des_m, des_d, from_date);
        if (time_arr[0] > time_sta[0])
        {
            if (des_m == 2)
            {
                if ((des_y % 4 == 0 && des_y % 100 != 0) || des_y % 400 == 0)
                {
                    if (des_d < 29)   des_d++;
                    else
                    {
                        des_m = 3; des_d = 1;
                    }
                }
                else
                {
                    if (des_d < 28)   des_d++;
                    else
                    {
                        des_m = 3; des_d = 1;
                    }
                }
            }
            if (des_m == 12)
            {
                if (des_m < 31)   des_m++;
                else
                {
                    des_y++; des_m = 1; des_d = 1;
                }
            }
            if (des_m == 1 || des_m == 3 || des_m == 5 || des_m == 7 || des_m == 8 || des_m == 10)
            {
                if (des_d < 31)   des_d++;
                else
                {
                    des_m++; des_d = 1;
                }
            }
            if (des_m == 4 || des_m == 6 || des_m == 9 || des_m == 11)
            {
                if (des_d < 30)   des_d++;
                else
                {
                    des_m ++; des_d = 1;
                }
            }
        }
    }

    ~Ticket_Query() = default;
    
    Ticket_Query& operator= (const Ticket_Query &other)
    {
        if (this == &other)
            return *this;
        memset(train_id, 0, sizeof(train_id));
        memset(from, 0, sizeof(from));
        memset(des, 0, sizeof(des));
        for (int i = 0; i < maxe; i++)
            memset(ticket_kind[i], 0, sizeof(ticket_kind[i]));
        memset(num, 0, sizeof(num));
        memset(price, 0, sizeof(price));

        memcpy(train_id, other.train_id, sizeof(train_id));
        memcpy(from, other.from, sizeof(from));
        memcpy(time_sta, other.time_sta, sizeof(time_sta));
        from_y = other.from_y;
        from_m = other.from_m;
        from_d = other.from_d;
        memcpy(des, other.des, sizeof(des));
        memcpy(time_arr, other.time_arr, sizeof(time_arr));
        des_y = other.des_y;
        des_m = other.des_m;
        des_d = other.des_d;
        for (int i = 0; i < maxe; i++)
            memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
        memcpy(num, other.num, sizeof(num));
        memcpy(price, other.price, sizeof(price));
        nprice = other.nprice;

        return *this;
    }

    //get the total time int this train (in minutes)
    int get_total_time() const
    {
        int ans = 0;
        if (from_d != des_d || from_m != des_m || from_y != des_y)
        {
            ans = 60 * (24 - time_sta[0] - 1) + 60 - time_sta[1];
        }
        else
        {
            if (time_sta[1] > time_arr[1])
            {
                ans = 60 * (time_arr[1] - time_sta[1] - 1) + 60 - time_sta[1] + time_arr[1];
            }
            else ans = 60 * (time_arr[1] - time_sta[1]) + time_arr[1] - time_sta[1];
        }
        return ans;
    }

    void mycout()
    {
        char *d_from = date_to_string(from_y, from_m, from_d);
        char *d_des = date_to_string(des_y, des_m, des_d);
        char *t_from = time_to_string(time_sta);
        char *t_des = time_to_string(time_arr);

        std::cout << train_id << ' ' << from << ' ' << d_from << ' ' << t_from << ' ' << d_des << ' ' << t_des;
        for (int i = 0; i < nprice; i++)
        {
            std::cout << ticket_kind[i] << ' ' << num[i] << ' ' << price[i] << ' ';
        }
        std::cout << std::endl;
        delete[] d_from;
        delete[] d_des;
        delete[] t_from;
        delete[] t_des;
    }
};

class Train_Query
{
public:
    char train_id[maxm]{};
    char train_name[maxm]{};
    char catalog[maxc]{};
    int nstation;
    int nprice;
    char ticket_kind[maxe][maxm]{};

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
    Train_Query(char objtrain_id[], char objtrain_name[], char objcatalog[], int objnstation, int objnprice, char **objticket_kind)
    {
        memcpy(train_id, objtrain_id, sizeof(train_id));
        memcpy(train_name, objtrain_name, sizeof(train_name));
        memcpy(catalog, objcatalog, sizeof(catalog));
        nstation = objnstation;
        nprice = objnprice;
        memcpy(ticket_kind, objticket_kind, sizeof(ticket_kind));
    }

    Train_Query& operator= (const Train_Query &other)
    {
        if (this == &other)
            return *this;
        memset(train_id, 0, sizeof(train_id));
        memset(train_name, 0, sizeof(train_name));
        memset(catalog, 0, sizeof(catalog));
        for (int i = 0; i < maxe; i++)
            memset(ticket_kind[i], 0, sizeof(ticket_kind[i]));

        memcpy(train_id, other.train_id, sizeof(train_id));
        memcpy(train_name, other.train_name, sizeof(train_name));
        memcpy(catalog, other.catalog, sizeof(catalog));
        nstation = other.nstation;
        nprice = other.nprice;
        memcpy(ticket_kind, other.ticket_kind, sizeof(ticket_kind));
        return *this;
    }

    void mycout()
    {
        std::cout << train_id << ' ' << train_name << ' ' << catalog << ' ';
        std::cout << nstation << ' ' << nprice << ' ';
        for (int i = 0; i < nprice; i++)
            std::cout << ticket_kind[i] << ' ';
        std::cout << std::endl;
    }
};

class Station_Query
{
public:
    int nprice;
    char station[maxm]{};
    char arrtime[2]{};
    char statime[2]{};
    char stpover[2]{};
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

    Station_Query& operator= (const Station_Query &other)
    {
        if (this == &other)
            return *this;
        memset(station, 0, sizeof(station));
        memset(arrtime, 0, sizeof(arrtime));
        memset(statime, 0, sizeof(statime));
        memset(stpover, 0, sizeof(stpover));
        memset(price, 0, sizeof(price));

        memcpy(station, other.station, sizeof(station));
        memcpy(arrtime, other.arrtime, sizeof(arrtime));
        memcpy(statime, other.statime, sizeof(statime));
        memcpy(stpover, other.stpover, sizeof(stpover));
        memcpy(price, other.price, sizeof(price));
        nprice = other.nprice;
        return *this;
    }

    void mycout()
    {
        std::cout << station << ' ';
        char *atime = time_to_string(arrtime);
        char *stime = time_to_string(statime);
        char *otime = time_to_string(stpover);
        std::cout << atime << ' ' << stime << ' ' << otime << ' ';
        for (int i = 0; i < nprice; i++)
        {
            std::cout << "￥" << price[i] << ' ';
        }
        std::cout << std::endl;
        delete[] atime;
        delete[] stime;
        delete[] otime;
    }
};

#endif
