#ifndef DATAS_HPP
#define DATAS_HPP
const int maxn = 41; //max length for user_name
const int maxm = 21; //max length for usually other things
const int maxc = 11; //max length for catalog
const int maxe = 5; //max length for ticket types

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
            strcpy(name, objname);
            strcpy(password, objpassword);
            strcpy(email, objemail);
            strcpy(phone, objphone);
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
    /* Using Keys: train_id*/
    class Train_Data
    {
    private:
        char train_name[maxn];
        char ticket_kind[maxe][maxm];
        char catalog[maxc];
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
            for (int i = 0; i < maxe; i++)
            {
                memcpy(ticket_kind[i], other.ticket_kind[i], sizeof(ticket_kind[i]));
            }
            nstation = other.nstation;
            nprice = other.nprice;
            is_sale = other.is_sale;
        }
        Train_Data(const char objname[], const char objticket_kind[maxe][], const char objcatalog[], const char objstation, const char objprice )
        {
            strcpy(train_name, objname);
            for (int i = 0; i < maxe; i++)
            {
                strcpy(ticket_kind[i], objticket_kind[i]);
            }
            strcpy(catalog, objcatalog);
            nstation = objstation;
            nprice = objprice;
            is_sale = false;
        }
        ~Train_Data() {}

        /*Used for get private data*/
        char* get_train_name() {return train_name;}
        char* get_ticket_kind(int i) {return ticket_kind[i];}
        char* get_catalog() {return catalog;}
        int get_price_kind() {return (int)nprice;}
        int get_station_kind() {return (int)nstation;}
    };

    /* Ticket_Data is specially designed for the station information of a train */
    /* Using Keys: train_id(main key) + loc(sub key) */
    class Ticket_Data
    {
    private:
        char statime[2]; //start time of a train on a specific station
        char stptime[2]; //stop time of a train on a specific station
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
        void string_to_time(char times[], char str[])
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
            memset(statime, 0, sizeof(statime));
            memset(stptime, 0, sizeof(stptime));
            memset(stpover, 0, sizeof(stpover));
            memset(price, 0, sizeof(price));
        }
        Ticket_Data(const Train_Data &other)
        {
            memcpy(statime, other.statime, sizeof(statime));
            memcpy(stptime, other.stptime, sizeof(statime));
            memcpy(stpover, other.stpover, sizeof(stpover));
            memcpy(price, other.price, sizeof(price));
        }
        /* Waring: objprice must have exactly maxe elements for memcpy */
        Ticket_Data(const char start[], const char stop[], const char over[], const double objprice[])
        {
            string_to_time(statime, start);
            string_to_time(stptime, stop);
            string_to_time(stpover, over);
            memcpy(price, objprice, sizeof(objprice));
        }
        ~Ticket_Data() {}

        char* get_statime() {return time_to_string(statime);}
        char* get_stptime() {return time_to_string(stptime);}
        char* get_stpover() {return time_to_string(stpover);}
        double get_price(int num) {return price[num];}
    };

    /* Buyer_Data is for the ticket a user has bought */
    /* Using Keys: user_id train_id */
    class Buyer_Data
    {
    private:
        int num;
        char train_id[maxm];
        char from[maxm]; //departure of the ticket
        char des[maxm]; //destination of the ticket
        char 
    };
};
#endif
