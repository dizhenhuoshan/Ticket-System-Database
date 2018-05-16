#ifndef DATABASE_TREENODE_H
#define DATABASE_TREENODE_H

#include "vector.h"
#include "constant.h"

namespace sjtu {



    template<class KeyType, class ValType>
    class TreeNode {
    public:
        offsetNumber addr;          // exact address in file.

        /** 先new，再卡常。*/
        vector<offsetNumber> childs;
        vector<KeyType> keys;
        vector<ValType> vals;

        offsetNumber next;

        bool isLeaf;

        explicit TreeNode(offsetNumber address = -1, bool leaf = true):
                // vectors are empty after init.
                childs(blockSize / sizeof(offsetNumber)),
                vals(blockSize / sizeof(ValType)),
                keys(blockSize / sizeof(KeyType))
        {
            addr = address;
            next = -1;
            isLeaf = leaf;
        }

        TreeNode(const TreeNode &other) noexcept:
                childs(blockSize / sizeof(offsetNumber)),
                vals(blockSize / sizeof(ValType)),
                keys(blockSize / sizeof(KeyType))
        {
            addr = other.addr;
            next = other.next;

            isLeaf = other.isLeaf;

            keys = other.keys;
            if (isLeaf)
                vals = other.vals;
            else
                childs = other.childs;
        }

        TreeNode &operator= (const TreeNode &other) {
            addr = other.addr;
            childs = other.childs;
            keys = other.keys;
            vals = other.vals;
            next = other.next;
            isLeaf = other.isLeaf;
            return *this;
        }
        /**
         * search for i, where i is the smallest value with key[i] > K. if search fails, return -1
         * it may be regarded as finding sup of K in set of Kis */
        short search_sup(KeyType K) {
            short pos = 0;
            while (pos < keys.size() && keys[pos] <= K) ++pos;

            if (pos == keys.size()) return -1;
            else return pos;
        }

        /**
         * find exactly key[i] == K. if search fails, return -1
         * in the case of duplicated key, return the left-most one */
        short search_exact(const KeyType &K) {
            short pos = 0;
            while (pos < keys.size() && keys[pos] != K) ++pos;

            if (pos == keys.size()) return -1;
            else return pos;
        }

        /**
         * reset node content, parent, next, and isLeaf remains.
         * reset node to be split. */
        void clear_content() {
            childs.clear();
            keys.clear();
            vals.clear();
        }

        /**
         * clear node to be the container for a new one. */
        void clear() {
            addr = next = -1;
            isLeaf = true;

            childs.clear();
            keys.clear();
            vals.clear();
        }

        /**
         * Node must be branch node, search the position of the specific child in this node.
         * I must be able to find it, otherwise throw exception.
         *
         * change: if not found, return -1. */
        short search_child(offsetNumber child) {
            short i = 0;
            while (i < childs.size() && childs[i] != child) ++i;

            if (i == childs.size()) {
                // std::cerr << "unmatched father and childs" << std::endl;
                return -1;
            }
            else return i;
        }
    };
};
#endif //DATABASE_TREENODE_H
