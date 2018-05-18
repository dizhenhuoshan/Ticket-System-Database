#include <iostream>
<<<<<<< HEAD
#include "keys.h"
#include "datas.h"
=======
#include <cstring>
using namespace std;

int main()
{
    int tmp;
    char a;
    char b[4];
    b[2] = '\0';
    cin >> tmp;
    a = (char)tmp;
    b[0] = a / 10 + '0';
    b[1] = a % 10 + '0';
    cout << b << endl;
    return 0;
}
>>>>>>> 70ed6848bbca783446ff48b66f66273eccba4ec1
