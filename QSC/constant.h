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
     *      in total 4+1+2+2+2+4+4 = 19 bytes. */
    typedef int offsetNumber;                   // offset in file as address.
    const offsetNumber tree_utility_byte = 16;
    const offsetNumber node_utility_byte = 19;
    const int blockSize = 50;
};

#endif //MYDATABASE_CONSTANT_H
