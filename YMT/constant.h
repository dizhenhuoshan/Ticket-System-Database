#ifndef MYDATABASE_CONSTANT_H
#define MYDATABASE_CONSTANT_H

namespace sjtu {
    /**
     * in addition to content information, B+ tree has to store some configuration information.
     *
     * 1. (utility_offset) tree config: root_offset, head_offset, tail_offset, append_offset.
     *      in total 4+4+4+4 = 16 bytes.
     *
     * 2. (node_spare) node config: address, isLeaf, K_size, V_size, Ch_size, father, next.
     *      in total 4+1+2+2+2+4+4 = 19 bytes.
     *      note: now father is gone, new node_utility_byte is 15. */
    typedef int offsetNumber;                   // offset in file as address.
    const offsetNumber tree_utility_byte = 16;
    const offsetNumber node_utility_byte = 15;
    const int blockSize = 4096;
    const int maxl = 50; //max stations a train will passed.
    const int maxs = 1000; //max train at one station;
    const int maxt = 200; //max train from loc1 to loc2;
    const int maxn = 41; //max length for user_name
    const int maxm = 21; //max length for usually other things
    const int maxc = 11; //max length for catalog
    const int maxe = 5; //max length for ticket type
};

#endif //MYDATABASE_CONSTANT_H
