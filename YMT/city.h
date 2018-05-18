#ifndef CITY_HPP
#define CITY_HPP
const int maxn = 21;

class City_Main_Key
{
private:
    char loc[maxn];
public:
    City_Main_Key()
    {
        for (int i = 0; i < maxn; i++)
        {
            loc[i] = 0;
        }
    }
    City_Main_Key(const City_Main_Key &other)
    {
        for (int i = 0; i < maxn; i++)
        {
            loc[i] = other.loc[i];
        }
    }
    City_Main_Key(char objloc[])
    {
        for (int i = 0; i < maxn; i++)
        {
            loc[i] = objloc[i];
        }
    }
    ~City_Main_Key() {}

    char* getloc() {return loc;}
};

class City_Sub_Key
{

};

#endif
