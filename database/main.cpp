#include <iostream>
#include <cstring>
#include "utility.h"
#include "user.h"
#include "processor.h"
#include "train.h"
#include "./bptree/BPlusTree.h"
#include "keys.h"
#include "constant.h"
#include <unistd.h>
using namespace sjtu;


int main() {
    /*
    char train_id[sjtu::ID_SIZE];
    char cat[sjtu::CATALOG_SIZE];
    int date = 1203;

    sjtu::order_key low = sjtu::get_low_order_key(train_id, date, cat);
    sjtu::order_key high = sjtu::get_high_order_key(train_id, date, cat);

    sjtu::order_key_zip low_zip(low);
    sjtu::order_key_zip high_zip(high);

    if(low_zip < high_zip)
        cout << 1 << endl;
    else
        cout << 2  << endl;

    sjtu::processor prcs;
    prcs.master();
     */
    /*
    char u_id[ID_SIZE];
    char date[DATE_SIZE];
    char cat;
    char train_id[ID_SIZE];

    scanf("%s %s %c %s", u_id, date, &cat, train_id);
    int num_date = date_to_int(date);

    order_key key(u_id, num_date, cat, train_id, 1, 3);
    order_key low = get_low_order_key(u_id, num_date, cat);
    order_key high = get_high_order_key(u_id, num_date, cat);

    order_key_zip key_zip(key);
    order_key_zip low_zip(low);
    order_key_zip high_zip(high);

    key_zip.view();
    low_zip.view();
    high_zip.view();
    */
    /*
    if(low_zip < key_zip)
        std::cout << "yes" << std::endl;
    if(key_zip < high_zip)
        std::cout << "yes" << std::endl;
    */
    /*
    char tmp[20];
    strcpy(tmp, "first");
    char tmp1[20], tmp2[20];
    tmp1[0] = 127;
    tmp1[1] = '\0';
    tmp2[0] = 0;
    tmp2[1] = '\0';
    std::cout << strcmp(tmp, tmp1) << std::endl;
    std::cout << strcmp(tmp, tmp2) << std::endl;
     */
    // sjtu::processor prcs;
    // prcs.master();
    // prcs.debug();

    return 0;
}

/*
sjtu::BPlusTree<sjtu::train_id_key, sjtu::train_id_val> haha;
haha.set_filename("lala.txt");
haha.open_file();
char key[10];
sjtu::train_id_val val(1);

strcpy(key, "aa");
haha.insert(sjtu::train_id_key(key), val);

haha.view();

cout << "lalallalal" << endl;
strcpy(key, "ab");
val = 2;
haha.insert(sjtu::train_id_key(key), val);

haha.view();
 */

/*
char tmp1[100], tmp2[100];
char in[100];

scanf("%s", in);
strcpy(tmp1, in);
printf("%s\n", tmp1);

scanf("%s", in);
strcpy(tmp2, in);
printf("%s\n", tmp2);

strcat(tmp1, tmp2);
printf("%s\n", tmp1);
 */
/*
sjtu::processor prcs;
prcs.master();
 */
/*
char time[100];
while(true) {
    scanf("%s", time);
    std::cout << sjtu::time_to_int(time) << std::endl;
    sjtu::time_to_str(sjtu::time_to_int(time), time);
    printf("%s\n", time);
    break;
}*/
/*
float a;
char tmp[20];
strcpy(tmp, "¥10.32");
sscanf(tmp + 2, "%f", &a);
printf("%f\n", a);

sjtu::train t;
t.read();
t.view();
 */
/*
sjtu::train t;
t.read();
t.view();

sjtu::pile_trainstorer s;
s.open_file();
sjtu::train tmp;
int address;
address = s.append_train(t);
s.get_train_by_offset(address, tmp);
s.close_file();
cout << "got train: " << endl;
tmp.view();
 */
/*
sjtu::pile_trainstorer s;
s.open_file();
sjtu::train tmp;
s.get_train_by_offset(4, tmp);
tmp.view();
 */
