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
/*Covert the date to stored time*/
void date_to_string(short year, char month, char day, char ans[])
{
    char invaild[11] = "xxxx-xx-xx";
    if (year == -1)
    {
        memcpy(ans, invaild, sizeof(invaild));
        return;
    }
    ans[10] = '\0';
    ans[4] = '-';
    ans[7] = '-';
    for (int i = 0; i < 4; i++)
    {
        ans[3 - i] = static_cast<char>(year % 10 + '0');
        year = static_cast<short>(year / 10);
    }
    for (int i = 0; i < 2; i++)
    {
        ans[6 - i] = static_cast<char>(month % 10 + '0');
        ans[9 - i] = static_cast<char>(day % 10 + '0');
        month = static_cast<char>(month / 10);
        day = static_cast<char>(day / 10);
    }
}

//date_pushnext is to get next date_year
void date_pushnext(char objdate[])
{
    short year;
    char month;
    char day;
    string_to_date(year, month, day, objdate);
    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            if (day < 29)   day++;
            else
            {
                month = 3; day = 1;
            }
        }
        else
        {
            if (day < 28)   day++;
            else
            {
                month = 3; day = 1;
            }
        }
    }
    if (month == 12)
    {
        if (day < 31)   day++;
        else
        {
            year++; month = 1; day = 1;
        }
    }
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10)
    {
        if (day < 31)   day++;
        else
        {
            month++; day = 1;
        }
    }
    if (month == 4 || month ==6  || month == 9 || month == 11)
    {
        if (day < 30)   day++;
        else
        {
            month ++; day = 1;
        }
    }
    char *ans = new char[DATE_SIZE];
    date_to_string(year, month, day, ans);
    memcpy(objdate, ans, sizeof(ans));
    delete[] ans;
}

bool check_push(char sta_time[], char arr_time[])
{
    if (sta_time[0] < arr_time[0])  return false;
    if (sta_time[0] > arr_time[0])  return true;
    if (sta_time[1] < arr_time[1]) return false;
    if (sta_time[1] > arr_time[1])  return true;
    if (sta_time[3] < arr_time[3])  return false;
    if (sta_time[3] > arr_time[3]) return true;
    return sta_time[4] >= arr_time[4];
}

if(!check_push(sta_time, arr_time))
	date_pushnext(arr_date);
