#ifndef KEYS_HPP
#define KEYS_HPP
#include <cstring>
const int maxn = 41;
const int maxm = 21;
namespace sjtu
{
    /** User_Key is the class for user index **/
    class User_id_Key
    {
    private:
        unsigned int uesr_id; //Each user must have a specific id.
    public:
        User_id_Key()
        {
            user_id = -1;
        }
        User_id_Key(const Key &other)
        {
            user_id = other.user_id;
        }
        User_id_Key(const unsigned int num):user_id(num) {}
        ~User_id_Key() {}

        bool operator==(const Key &other) const
        {
            return user_id == other.user_id;
        }
        bool operator<(const Key &other) const
        {
            return user_id < other.user_id;
        }
        unsigned int getid() const
        {
            return user_id;
        }
    };

    class Loc_Key //Loc_Key is a type of key used for location except ticket
    {
    private:
        char loc[maxm];
    public:
        Loc_Key()
        {
            memset(loc, 0, sizeof(loc));
        }
        Loc_Key(const City_Main_Key &other)
        {
            for (int i = 0; i < maxn; i++)
            {
                loc[i] = other.loc[i];
            }
        }
        Loc_Key(const char objloc[])
        {
            for (int i = 0; i < maxn; i++)
            {
                loc[i] = objloc[i];
            }
        }
        ~Loc_Key() {}

        bool operator<(const char obj[])
        {
            return strcmp(loc, obj) < 0;
        }
        bool operator==(const char obj[])
        {
            return strcmp(loc, obj) == 0;
        }
        char* getloc() {return loc;}
    };

    class Train_id_Key
    {
    private:
        char train_id[maxm];
    public:
        Train_id_Key()
        {
            memset(train_id, 0, sizeof(train_id));
        }
        Train_id_Key(const Train_id_Key &other)
        {
            for (int i = 0; i < maxm; i++)
            {
                train_id[i] = other.train_id[i];
            }
        }
        Train_id_Key(const char objtrain_id[])
        {
            for (int i = 0; i < maxm; i++)
            {
                train_id[i] = objtrain_id[i];
            }
        }
        ~Train_id_Key() {}

        bool operator<(const char obj[])
        {
            return strcmp(train_id, obj) < 0;
        }
        bool operator==(const char obj[])
        {
            return strcmp(train_id, obj) == 0;
        }
        char* gettrain_id() {return train_id;}
    };

};
#endif
