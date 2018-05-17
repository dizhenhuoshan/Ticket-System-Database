#ifndef USER_HPP
#define USER_HPP
#include <cstring>
const int maxn = 41;
const int maxo = 21;

/** User_Key is the class for user index **/
class User_Key
{
private:
    unsigned int id; //Each user must have a specific id.
public:
    User_Key()
    {
        id = -1;
    }
    User_Key(const Key &other)
    {
        id = other.id;
    }
    User_Key(unsigned int num):id(num) {}
    ~User_Key() {}

    bool operator==(const Key &other) const
    {
        return id == other.id;
    }
    bool operator<(const Key &other) const
    {
        return id < other.id;
    }
    unsigned int getid() const
    {
        return id;
    }
};

/*User_Data is the user information for registering*/
class User_Data
{
private:
    char name[maxn];
    char password[maxo];
    char email[maxo];
    char phone[maxo];
    char privilege;
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
        for (int i = 0; i < maxo; i++)
        {
            name[i] = other.name[i];
            password[i] = other.password[i];
            email[i] = other.email[i];
            phone[i] = other.phone[i];
        }
        for (int i = maxo; i < maxn; i++)
        {
            name[i] = other.name[i];
        }
    }
    User_Data(char objname[], char objpassword[], char objemail[], char objphone[], char objprivilege)
    {
        for (int i = 0; i < maxo; i++)
        {
            name[i] = objname[i];
            password[i] = objpassword[i];
            email[i] = objemail[i];
            phone[i] = objphone[i];
        }
        for (int i = maxo; i < maxn; i++)
        {
            name[i] =objname[i];
        }
        privilege = objprivilege;
    }
    ~User_Data() {}

    char* getname() {return name;}
    char* getpasswd() {return password;}
    char* getemail() {return email;}
    char* getphone() {return phone;}
    char  getprivilege() {return privilege;}
};

#endif
