#include <iostream>
#include <cstring>
#include "database.h"

using namespace sjtu;

//*analyse the orders*/
/*
*   Types:
*        Users:
*       11 -- register
*       12 -- login
*       13 -- query_profile
*       14 -- modify_profile
*       15 -- modify_privilege
*        Tickets:
*       21 -- query_ticket
*       22 -- query_transfer
*       23 -- buy_ticket
*       24 -- query_order
*       25 -- refund_ticket
*        Train:
*       31 -- add_train
*       32 -- sale_train
*       33 -- query_train
*       34 -- delete_train
*       35 -- modify_train
*        Managememt:
*       41 -- clean
*       42 -- exit
*/

int parser_reader(char order[])
{
    if (strcmp(order, "register") == 0)         return 11;
    if (strcmp(order, "login") == 0)            return 12;
    if (strcmp(order, "query_profile") == 0)    return 13;
    if (strcmp(order, "modify_profile") == 0)   return 14;
    if (strcmp(order, "modify_privilege") == 0) return 15;
    if (strcmp(order, "query_ticket") == 0)     return 21;
    if (strcmp(order, "query_transfer") == 0)   return 22;
    if (strcmp(order, "buy_ticket") == 0)       return 23;
    if (strcmp(order, "query_order") == 0)      return 24;
    if (strcmp(order, "refund_ticket") == 0)    return 25;
    if (strcmp(order, "add_train") == 0)        return 31;
    if (strcmp(order, "sale_train") == 0)       return 32;
    if (strcmp(order, "query_train") == 0)      return 33;
    if (strcmp(order, "delete_train") == 0)     return 34;
    if (strcmp(order, "modify_train") == 0)     return 35;
    if (strcmp(order, "clean") == 0)            return 41;
    if (strcmp(order, "exit") == 0)             return 42;
    return -1;
}

double price_reader(const char ch[])
{
    double a = 0;
    int i = 3; // a ￥ is 3-byte
    while (ch[i] != '.' && ch[i] != '\0')
    {
        a = a * 10 + ch[i] - '0';
        i++;
    }
    if (ch[i] == '.')
    {
        i++;
        double k = 0.1;
        while (ch[i] != '\0')
        {
            a += (ch[i] - '0') * k;
            k *= 0.1;
            i++;
        }
    }
    return a;
}

MyDatabase DB;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    while(true)
    {
        char order[maxm];
        int parser = 0;
        std::cin >> order;
        parser = parser_reader(order);
        switch (parser)
        {
            case -1:
            {
                std::cout << "Invaild Input" << std::endl;
                break;
            }
            case 11:
            {
                char name[maxn];
                char password[maxm];
                char email[maxm];
                char phone[maxm];
                std::cin >> name >> password >> email >> phone;
                unsigned int id = DB.user_register(name, password, email, phone);
                if (id == 0)
                    std::cout << -1 << std::endl;
                else std::cout << id << std::endl;
                break;
            }
            case 12:
            {
                unsigned int user_id;
                char password[maxm];
                std::cin >> user_id >> password;
                if (DB.user_login(user_id, password))
                    std::cout << 1 << std::endl;
                else std::cout << 0 << std::endl;
                break;
            }
            case 13:
            {
                unsigned int user_id;
                std::cin >> user_id;
                User_Query ans;
                if (!DB.query_profile(user_id, ans))
                    std::cout << 0 << std::endl;
                else ans.mycout();
                break;
            }
            case 14:
            {
                unsigned int user_id;
                char name[maxn];
                char password[maxm];
                char email[maxm];
                char phone[maxm];
                std::cin >> user_id >> name >> password >> email >> phone;
                if (DB.modify_profile(user_id, name, password, email, phone))
                    std::cout << 1 << std::endl;
                else std::cout << 0 << std::endl;
                break;
            }
            case 15:
            {
                unsigned int user_id;
                unsigned int user_id2;
                char privilege;
                std::cin >> user_id >> user_id2 >> privilege;
                if (DB.modify_privilege(user_id, user_id2, privilege - '0'))
                    std::cout << 1 << std::endl;
                else
                    std::cout << 0 << std::endl;
                break;
            }
            case 21:
            {
                char from[maxm];
                char des[maxm];
                char date[11];
                char catalog[maxc];
                std::cin >> from >> des >> date >> catalog;
                auto *tic_query = new Ticket_Query[maxt];
                int num = 0;
                num = DB.query_ticket(from, des, date, catalog, tic_query);
                if (num == 0)
                    std::cout << -1 << std::endl;
                else
                {
                    for (int i = 0; i < num; i++)
                    {
                        tic_query[i].mycout();
                    }
                }
                delete[] tic_query;
                break;
            }
            case 22:
            {
                char from[maxm];
                char des[maxn];
                char date[11];
                char catalog[maxc];
                std::cin >> from >> des >> date >> catalog;
                Ticket_Query ans1;
                Ticket_Query ans2;
                if (!DB.query_transfer(from, des, date, catalog, ans1, ans2))
                    std::cout << -1 << std::endl;
                else
                {
                    ans1.mycout();
                    ans2.mycout();
                }
                break;
            }
            case 23:
            {
                unsigned int user_id;
                short num;
                char train_id[maxm];
                char from[maxm];
                char des[maxm];
                char date[11];
                char ticket_kind[maxm];
                std::cin >> user_id >> num >> train_id >> from >> des >> date >> ticket_kind;
                if (!DB.buy_ticket(user_id, num, train_id, from, des, date, ticket_kind))
                    std::cout << 0 << std::endl;
                else std::cout << 1 << std::endl;
                break;
            }
            case 24:
            {
                unsigned int user_id;
                char date[maxm];
                char catalog[maxc];
                auto *tic_query = new Ticket_Query[maxb];
                std::cin >> user_id >> date >> catalog;
                int num = DB.query_order(user_id, date, catalog, tic_query);
                if (num == 0)
                    std::cout << -1 << std::endl;
                else
                {
                    for (int i = 0; i < num; i++)
                        tic_query[i].mycout();
                }
                delete[] tic_query;
                break;
            }
            case 25:
            {
                unsigned int user_id;
                short num;
                char train_id[maxm];
                char from[maxm];
                char des[maxm];
                char date[11];
                char ticket_kind[maxm];
                std::cin >> user_id >> num >> train_id >> from >> des >> date >> ticket_kind;
                if (!DB.refund_ticket(user_id, num, train_id, from, des, date, ticket_kind))
                    std::cout << 1 << std::endl;
                else std::cout << 0 << std::endl;
                break;
            }
            case 31:
            {
                char train_id[maxm];
                char train_name[maxn];
                char catalog[maxc];
                char ticket_kind[maxe][maxm];
                int num_sta = 0;
                int num_pri = 0;
                std::cin >> train_id >> train_name >> catalog >> num_sta >> num_pri;
                for (int i = 0; i < num_pri; i++)
                    std::cin >> ticket_kind[i];
                std::cout << train_name << std::endl;
                for (int i = 0; i < num_pri; i++)
                    std::cout << ticket_kind[i];
                auto *tmp_sta = new Station_Query[num_sta];
                for (int i = 0; i < num_sta; i++)
                {
                    char loc[maxm];
                    char arr_time[6];
                    char sta_time[6];
                    char sto_time[6];
                    char origin_price[maxm];
                    double price[maxe];
                    std::cin >> loc >> arr_time >> sta_time >> sto_time;
                    for (int j = 0; j < num_pri; j++)
                    {
                        memset(origin_price, 0, sizeof(origin_price));
                        std::cin >> origin_price;
                        price[j] = price_reader(origin_price);
                    }
                    tmp_sta[i] = Station_Query(loc, arr_time, sta_time, sto_time, price);
                }
                if (DB.add_train(train_id, train_name, catalog, static_cast<char>(num_sta), static_cast<char>(num_pri), ticket_kind, tmp_sta))
                    std::cout << 1 << std::endl;
                else std::cout << 0 << std::endl;
                delete[] tmp_sta;
                break;
            }
            case 32:
            {
                char train_id[maxm];
                std::cin >> train_id;
                if (!DB.sale_train(train_id))
                    std::cout << 0 << std::endl;
                else std::cout << 1 << std::endl;
                break;
            }
            case 33:
            {
                char train_id[maxm];
                std::cin >> train_id;
                Train_Query train_ans;
                auto *sta_ans = new Station_Query[maxl];
                int sta_num = 0;
                if (!DB.query_train(train_id, train_ans, sta_ans, sta_num))
                    std::cout << 0 << std::endl;
                else
                {
                    train_ans.mycout();
                    for (int i = 0; i < sta_num; i++)
                        sta_ans[i].mycout();
                }
                delete[] sta_ans;
                break;
            }
            case 34:
            {
                char train_id[maxm];
                std::cin >> train_id;
                if (!DB.delete_train(train_id))
                    std::cout << 0 << std::endl;
                else std::cout << 1 << std::endl;
                break;
            }
            case 35:
            {
                char train_id[maxm];
                char train_name[maxn];
                char catalog[maxc];
                char ticket_kind[maxe][maxm];
                int num_sta = 0;
                int num_pri = 0;
                std::cin >> train_id >> train_name >> catalog >> num_sta >> num_pri;
                for (int i = 0; i < num_pri; i++)
                    std::cin >> ticket_kind[i];
                auto *tmp_sta = new Station_Query[num_sta];
                for (int i = 0; i < num_sta; i++)
                {
                    char loc[maxm];
                    char arr_time[6];
                    char sta_time[6];
                    char sto_time[6];
                    char origin_price[maxm];
                    double price[maxe];
                    std::cin >> loc >> arr_time >> sta_time >> sto_time;
                    for (int j = 0; j < num_pri; j++)
                    {
                        memset(origin_price, 0, sizeof(origin_price));
                        std::cin >> origin_price;
                        price[j] = price_reader(origin_price);
                    }
                    tmp_sta[i] = Station_Query(loc, arr_time, sta_time, sto_time, price);
                }
                if (DB.modify_train(train_id, train_name, catalog, static_cast<char>(num_sta), static_cast<char>(num_pri), ticket_kind, tmp_sta))
                    std::cout << 1 << std::endl;
                else std::cout << 0 << std::endl;
                delete[] tmp_sta;
                break;
            }
            case 41:
            {
                if (DB.clean())
                    std::cout << 1 << std::endl;
                else std::cout << 0 << std::endl;
                MyDatabase();
                break;
            }
            case 42:
            {
                std::cout << "BYE" << std::endl;
                return 0;
            }
            default:break;
        }
    }
}
