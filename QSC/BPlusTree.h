#ifndef DATABASE_BPLUSTREE_H
#define DATABASE_BPLUSTREE_H

#define LOG 1
#include <string>
#include <cstdio>
#include <cmath>
#include "TreeNode.h"
#include "BufferManager.h"
#include "constant.h"
#include <queue>



namespace sjtu {
    /** B-plus tree with unique key. */

    /**
     * degree is stored in B-tree class instead of node class,
     * because node class is adaptable to B-tree with different degrees,
     * which means degree is BPlustree's feature.
     *
     * initially, I wanna design LeafNode and BranchNode separately.
     * but if we do this, data type will be heterogeneous, and hard to manage.
     * information about keys, vals, and childs are well encapsulated inside <vector> class.
     * we omit unnecessary fields. */


    template<class KeyType, class ValType>
    class UniqueBPlusTree {
    private:
        typedef TreeNode<KeyType, ValType> Node;

    private:
        std::string filename;
        BufferManager<KeyType, ValType> bm;

        /**
        * leaf node key number range: [ceil((degree - 1) / 2), degree - 1]
        * branch node childs range: [ceil(degree / 2), degree]
        * branch node key number = childs number - 1 */
        int branch_degree, leaf_degree;

        int K_byte, V_byte;

    private:
        /**
         * return the leaf node K belongs.
         * Note that I'm always able to land in a leaf node. */
        void search_to_leaf(KeyType K, Node &ret) {
            bm.get_root(ret);

            while (!ret.isLeaf) {
                int i = ret.search_sup(K);

                // fail to find, means K is larger than max key.
                if (i == -1)
                    bm.get_block_by_offset(ret.childs.back(), ret);
                else
                    bm.get_block_by_offset(ret.childs[i], ret);
            }
        }

        /** sub-function used by insert()
         *
         * insert key and value in node pointed by p
         * adapting to search_sup method of Node class
         *
         * return false if key exists. */
        bool insert_in_leaf(Node &cur, const KeyType &K, const ValType &V) {
        #ifdef LOG
            std::cout << "insert_in_leaf" << std::endl;
        #endif
            // find the first key greater than K, if exist.
            int i = cur.search_sup(K);

            // check K existence.
            if (i != -1 && i != 1 && cur.keys[i - 1] == K)
                return false;

            // insert.
            if (i == -1)     // no sup, insert in the back.
            {
                cur.keys.push_back(K);
                cur.vals.push_back(V);
            } else {
                cur.keys.insert(i, K);
                cur.vals.insert(i, V);
            }
            return true;
        }

        /** sub-function used by insert_in_leaf()
         *
         * when leaf key number exceeds degree - 1, it has to be split into two,
         * in which case key number will be exactly 'degree'.
         *
         * the first node receives ceil((degree - 1) / 2) keys
         * the second node receives the rest of them.
         * note that second node's key number always lies in BPlusTree key number range.
         *
         * example: degree = 5; range = [2, 4]; first = 2; second = 3.
         *          degree = 6; range = [3, 5]; first = 3; second = 3. */
        void split_leaf(Node &L, Node &L_next, const Node &tmp) {
        #ifdef LOG
            std::cout << "split_leaf" << std::endl;
        #endif
            short mid = (short)ceil((leaf_degree) / (2.0));

            L.keys.assign(tmp.keys, 0, mid);    // assign mid number of keys.
            L.vals.assign(tmp.vals, 0, mid);

            L_next.keys.assign(tmp.keys, mid, tmp.keys.size());
            L_next.vals.assign(tmp.vals, mid, tmp.vals.size());
        }

        KeyType split_branch(Node &B, Node &B_next, Node &tmp) {
        #ifdef LOG
            std::cout << "split_branch" << std::endl;
        #endif
            short mid = (short) ceil(branch_degree / 2.0);
            KeyType mid_key = tmp.keys[mid - 1];

            B.childs.assign(tmp.childs, 0, mid);
            B.keys.assign(tmp.keys, 0, mid - (short)1);

            B_next.childs.assign(tmp.childs, mid, tmp.childs.size());
            B_next.keys.assign(tmp.keys, mid, tmp.keys.size());

            // adjust children's father. Very expensive!!! must optimize!!!
            Node ch;
            for(int i = 0; i < B_next.childs.size(); ++i) {
                bm.get_block_by_offset(B_next.childs[i], ch);
                ch.parent = B_next.addr;
                bm.write_block(ch);
            }

            return mid_key;
        }
        /**
         * p is old node which has been split into two
         * p_next is the new one
         * K is the smallest key in p_next (in leaf to branch case)
         *
         * Note that branch nodes' key is always present in the front of leaves,
         * which is sadly not true for branch-branch case
         *
         * returns whether sons' father is adjusted. */
        bool insert_in_parent(Node &oldSon, const KeyType &K, Node &newSon) {
        #ifdef LOG
            std::cout << "insert_in_parent" << std::endl;
        #endif
            // new root is needed.
            if(oldSon.addr == bm.get_root()) {
                Node fa;
                bm.append_block(fa, false);     // this time we need branch node.

                bm.set_root(fa.addr);
                fa.childs.push_back(oldSon.addr);
                fa.childs.push_back(newSon.addr);
                fa.keys.push_back(K);
                oldSon.parent = fa.addr;
                newSon.parent = fa.addr;

                bm.write_block(fa);
                return false;
            }
            // pick out p's parent, update parent field.
            Node fa;
            bm.get_block_by_offset(oldSon.parent, fa);
            newSon.parent = fa.addr;
            /* insert into parent first, regardless of whether there's empty slot available */
            short son_idx = fa.search_child(oldSon.addr);
            fa.childs.insert(son_idx + 1, newSon.addr);
            fa.keys.insert(son_idx, K);
            if(fa.childs.size() <= branch_degree) {
                bm.write_block(fa);
                return false;
            }
            // check whether split is needed, distribute keys and children.
            // note that the problem is adjust children's parent in each node is expensive.
            else {
                Node tmp(fa);
                Node fa_next;
                bm.append_block(fa_next, false);
                KeyType mid_key;

                fa.clear_content();
                mid_key = split_branch(fa, fa_next, tmp);   // we adjust fa_next's child inside this.

                /// we need to adjust oldSon and newSon's parent.
                if(fa.search_child(oldSon.addr) != -1)
                    oldSon.parent = fa.addr;
                else
                    oldSon.parent = fa_next.addr;

                if(fa.search_child(newSon.addr) != -1)
                    newSon.parent = fa.addr;
                else
                    newSon.parent = fa_next.addr;

                insert_in_parent(fa, mid_key, fa_next);
                bm.write_block(fa);
                bm.write_block(fa_next);

                return true;
            }
        }

        bool erase_leaf(Node &leaf, KeyType K) {
            short K_idx = leaf.search_exact(K);
            // if K doesn't exist.
            if(K_idx == -1)
                return false;


        }

        public:
        explicit UniqueBPlusTree(const std::string &fname) {
            filename = fname;
            bm.set_fileName(fname);

            K_byte = sizeof(KeyType), V_byte = sizeof(ValType);

            /**
             * tree utility is stored in file head, which is followed by a leaf node (the first node in an file is always leaf node).
             * Note key and child number differ by 1 in branch. */
            leaf_degree = (blockSize - tree_utility_byte - node_utility_byte) / (K_byte + V_byte);
            branch_degree = (blockSize - node_utility_byte + K_byte) / (sizeof(offsetNumber) + K_byte);

            #ifdef LOG
                std::cout << "leaf degree: " << leaf_degree << " branch degree: " << branch_degree << std::endl;
            #endif

            if(leaf_degree == 0)
                std::cerr << "leaf degree is 0" << std::endl;
        }
        ~UniqueBPlusTree() {
            if(bm.is_opened())
                bm.close_file();
        }

        /**
         * to maintain database integrity, file isn't opened in constructor.
         * use a separate method to read or create it.
         *
         * Note that given the same file name, only one B-plus tree can open file.
         *
         * if open an opened file, return false. else return true. */
        bool open_file() {
            return bm.open_file();
        }
        bool close_file() {
            return bm.close_file();
        }


        ValType find(const KeyType &K) {
            Node ret;
            search_to_leaf(K, ret);

            int i = ret.search_exact(K);
            if (i == -1)     /// return default when unfind.
                return ValType();
            else
                return ret.vals[i];
        }

        /**
         * overwrite existing key, return false if key isn't found. */
        bool modify(KeyType K, ValType V) {
            Node leafNode;
            short keyPos;

            search_to_leaf(K, leafNode);
            keyPos = leafNode.search_exact(K);
            if(keyPos == -1)                // fail to find.
                return false;
            else {
                leafNode.vals[keyPos] = V;  // modify and write back.
                bm.write_block(leafNode);
            }
        }

        /**
         * return false if key has been inserted. */
        bool insert(const KeyType &K, const ValType &V) {
            static int cnt = 0;
            cnt++;
            #ifdef LOG
            std::cout << std::endl << cnt << " insert log starts: " << std::endl;
            #endif
            Node cur;
            if (bm.get_root() == -1) {
                bm.append_block(cur, true);     // append a new block.

                cur.keys.push_back(K);
                cur.vals.push_back(V);

                bm.set_root(cur.addr);          // update root.
                bm.write_block(cur);
                return true;
            }

            search_to_leaf(K, cur);
            if (cur.keys.size() < leaf_degree)   // if leaf node has extra slot.
            {
                bool flag = insert_in_leaf(cur, K, V);
                if(flag)                        // insert success
                {
                    bm.write_block(cur);
                    return flag;
                }
            }
            else                                // split leaf.
            {
                bool flag;
                Node newLeaf;
                bm.append_block(newLeaf, true); // get a new leaf node to the right of cur.

                newLeaf.next = cur.next;
                cur.next = newLeaf.addr;

                /* we can't do this so far, because cur->parent may not be created yet.
                 * next_leaf->parent = cur->parent; */

                Node tmp(cur);
                flag = insert_in_leaf(tmp, K, V);
                if (!flag) return false;         // key exists.

                cur.clear_content();
                split_leaf(cur, newLeaf, tmp);

                // see whether we can't overwrite father.
                insert_in_parent(cur, newLeaf.keys[0], newLeaf);  // modify parent keys and children.
                bm.write_block(cur);
                bm.write_block(newLeaf);
            }
        }

        /**
         * this erase operation can only adapt to unique key situation
         *
         * erase is an iterative process which goes bottom up, and uses erase_leaf first,
         * which then invokes erase_branch to do back track.
         *
         * if erased K is not found, return false. */
        bool erase(KeyType K) {
            if(bm.get_root() == -1) {
                std::cerr << "erase empty B plus tree" << std::endl;
                return false;
            }

            Node leaf;
            search_to_leaf(K, leaf);
            return erase_leaf(leaf, K);
        }

        /**
         * get the first and last element in tree.
         * if tree is empty, return object constructed by default constructor. */
        std::pair<KeyType, ValType> get_head() {
            Node head;
            if(bm.get_head(head))
                return std::pair<KeyType, ValType>(head.keys[0], head.vals[0]);
            else
                return std::pair<KeyType, ValType>(KeyType(), ValType());
        }
        std::pair<KeyType, ValType> get_tail() {
            Node tail;
            if(bm.get_tail(tail))
                return std::pair<KeyType, ValType>(tail.keys[0], tail.vals[0]);
            else
                return std::pair<KeyType, ValType>(KeyType(), ValType());
        }

        #ifdef LOG
        /** perform a level order traversal for B tree. */
        struct qElem {
            offsetNumber addr;
            int depth;
            qElem(offsetNumber ad, int dpt) : addr(ad), depth(dpt) { }
        };
        void view() {
            if(bm.get_root() == -1) {
                std::cout << "tree is empty" << std::endl;
                return;
            }

            std::queue<qElem> q;
            q.emplace(bm.get_root(), 1);

            while(!q.empty()) {
                qElem tmpQ = q.front();
                q.pop();

                Node tmpN;
                bm.get_block_by_offset(tmpQ.addr, tmpN);
                printf("address: %d; depth: %d; parent: %d ", tmpQ.addr, tmpQ.depth, tmpN.parent);
                if(tmpN.isLeaf) {
                    printf("next: %d; leafNode\nkeys: ", tmpN.next);
                    for(int i = 0; i < tmpN.keys.size(); ++i) {
                        std::cout << tmpN.keys[i] << ' ';
                    }

                    printf("\nvals: ");
                    for(int i = 0; i < tmpN.vals.size(); ++i) {
                        std::cout << tmpN.vals[i] << ' ';
                    }

                    printf("\n\n");
                }
                else {
                    printf("branchNode\nkeys: ");
                    for(int i = 0; i < tmpN.keys.size(); ++i) {
                        std::cout << tmpN.keys[i] << ' ';
                    }

                    printf("\nchildren: ");
                    for(int i = 0; i < tmpN.childs.size(); ++i) {
                        std::cout << tmpN.childs[i] << ' ';
                    }
                    printf("\n\n");

                    for(int i = 0; i < tmpN.childs.size(); ++i)
                        q.emplace(tmpN.childs[i], tmpQ.depth + 1);
                }
            }
        }
        #endif
    };
};
#endif //DATABASE_BPLUSTREE_H
