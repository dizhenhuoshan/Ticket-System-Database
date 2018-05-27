#ifndef DATABASE_BPLUSTREE_H
#define DATABASE_BPLUSTREE_H

#include "BufferManager.h"
#include <cstring>
#include <cmath>
#include <queue>
// #define LOG 1
// #define DEBUG 2

namespace sjtu {
    template<class KeyType, class ValType>
    class UniqueBPlusTree {
        friend class debugger;

    private:
        typedef TreeNode<KeyType, ValType> Node;

        struct retT {
            bool success;
            bool modified;
            retT(bool a, bool b): success(a), modified(b) { }
            retT() {}
        };


        char filename[50];
        BufferManager<KeyType, ValType> bm;

    private:

        /**
        * leaf node key number range: [ceil((degree - 1) / 2), degree - 1]
        * branch node childs range: [ceil(degree / 2), degree]
        * branch node key number = childs number - 1 */
        int branch_degree, leaf_degree;
        int K_byte, V_byte;

        Node pool[50];
        int cnt;


    private:

        retT erase_node(Node &cur, const KeyType &K) {
            static short sbl_off = 1;

            if(cur.isLeaf) {
                short delpos = cur.search_exact(K);

                // key doesn't exist.
                if(delpos == -1)
                    return retT(false, false);
                else {
                    cur.keys.erase(delpos);
                    cur.vals.erase(delpos);
                    return retT(true, true);
                }
            }
            else {
                Node &ch = pool[cnt++];
                ch.clear();

                short ch_pos = find_child(cur, K, ch);

                // 向下找子，递归进去。
                retT ret_info = erase_node(ch, K);

                // 删除未成功。
                if(!ret_info.success) {
                    cnt--;
                    return retT(false, false);
                }

                // 删除成功。记得写入。
                // 按照不同类型，分别处理当前树枝和儿子。
                if(ch.isLeaf) {
                    // 如果不太空，直接返回。
                    if(ch.keys.size() >= (short) ceil(leaf_degree / 2.0)) {
                        bm.write_block(ch);
                        cnt--;
                        return retT(true, false);
                    }
                    // 如果太空。
                    else {
                        Node &sbl = pool[cnt++];
                        short sbl_pos;
                        short key_pos;
                        KeyType K_bt;
                        Node *l_node, *r_node;

                        // 找到兄弟。
                        if(ch_pos == 0) {
                            bm.get_block_by_offset(cur.childs[1], sbl);
                            sbl_pos = 1;
                        }
                        else if(ch_pos == cur.childs.size()-1) {
                            bm.get_block_by_offset(cur.childs[cur.childs.size()-2], sbl);
                            sbl_pos = cur.childs.size() - (short) 2;
                        }
                        else {
                            bm.get_block_by_offset(cur.childs[ch_pos + sbl_off], sbl);
                            sbl_pos = ch_pos + sbl_off;
                            sbl_off *= -1;
                        }

                        // 找到要在当前结点中删除儿子们信息的位置。
                        key_pos = std::min(ch_pos, sbl_pos);
                        K_bt = cur.keys[key_pos];

                        // 如果可以放到一起。
                        if(ch.keys.size() + sbl.keys.size() <= leaf_degree) {
                            // 对称化儿子兄弟和儿子结点。
                            if(ch_pos < sbl_pos) {
                                l_node = &ch;
                                r_node = &sbl;
                            }
                            else {
                                l_node = &sbl;
                                r_node = &ch;
                            }

                            // maintain basic info.
                            l_node->next = r_node->next;
                            if (bm.tail_off == r_node->addr)
                                bm.tail_off = l_node->addr;

                            // 处理儿子。
                            for (short i = 0; i < r_node->keys.size(); ++i) {
                                l_node->keys.push_back(r_node->keys[i]);
                                l_node->vals.push_back(r_node->vals[i]);
                            }

                            // 处理父亲。
                            cur.keys.erase(key_pos);
                            cur.childs.erase(key_pos + (short) 1);

                            // 写入儿子，即使右儿子没什么卵用也暂时写入它。
                            bm.write_block(*l_node);
                            bm.write_block(*r_node);

                            // 删除成功。
                            cnt -= 2;
                            return retT(true, true);
                        }
                        // 借用一个，重新分配。
                        else {
                            if(sbl_pos < ch_pos) {
                                // borrow last key from sibling, record key position in father.
                                KeyType key_shift = sbl.keys.back();
                                ValType val_shift = sbl.vals.back();

                                // remove them.
                                sbl.keys.pop_back();
                                sbl.vals.pop_back();

                                // insert shifted key in front of leaf.
                                ch.keys.insert(0, key_shift);
                                ch.vals.insert(0, val_shift);

                                // change key in father node.
                                cur.keys[key_pos] = ch.keys[0];

                                // 写入儿子。
                                bm.write_block(ch);
                                bm.write_block(sbl);

                                // 删除成功。
                                cnt -= 2;
                                return retT(true, true);
                            }
                            // symmetric situation where sibling is to the right of p.
                            else {
                                // borrow first key from sibling.
                                KeyType key_shift = sbl.keys.front();
                                ValType val_shift = sbl.vals.front();

                                // remove them.
                                sbl.keys.erase(0);
                                sbl.vals.erase(0);

                                // append shifted key.
                                ch.keys.push_back(key_shift);
                                ch.vals.push_back(val_shift);

                                // change key in father node.
                                cur.keys[key_pos] = sbl.keys[0];

                                // 写入儿子。
                                bm.write_block(ch);
                                bm.write_block(sbl);

                                // 删除成功。
                                cnt -= 2;
                                return retT(true, true);
                            }
                        }
                    }
                }
                // 当子结点是树枝的时候。
                else {
                    // 如果不太空，直接返回。
                    if(ch.childs.size() >= (short) ceil(branch_degree / 2.0)) {
                        if(ret_info.modified)
                            bm.write_block(ch);
                        --cnt;
                        return retT(true, false);
                    }
                    // 如果太空。
                    else {
                        // 找到兄弟。
                        Node &sbl = pool[cnt++];
                        short sbl_pos;
                        short key_pos;
                        KeyType K_bt;
                        Node *l_node, *r_node;

                        // 找到兄弟。
                        if(ch_pos == 0) {
                            bm.get_block_by_offset(cur.childs[1], sbl);
                            sbl_pos = 1;
                        }
                        else if(ch_pos == cur.childs.size()-1) {
                            bm.get_block_by_offset(cur.childs[cur.childs.size()-2], sbl);
                            sbl_pos = cur.childs.size() - (short) 2;
                        }
                        else {
                            bm.get_block_by_offset(cur.childs[ch_pos + sbl_off], sbl);
                            sbl_pos = ch_pos + sbl_off;
                            sbl_off *= -1;
                        }

                        key_pos = std::min(ch_pos, sbl_pos);
                        K_bt = cur.keys[key_pos];

                        // 如果可以放到一起。
                        if(ch.childs.size() + sbl.keys.size() <= branch_degree) {
                            // 对称化儿子兄弟和儿子结点。
                            if(ch_pos < sbl_pos) {
                                l_node = &ch;
                                r_node = &sbl;
                            }
                            else {
                                l_node = &sbl;
                                r_node = &ch;
                            }

                            // 处理儿子。在左儿子上append在当前结点的key。
                            l_node->keys.push_back(K_bt);
                            for (short i = 0; i < r_node->keys.size(); ++i)
                                l_node->keys.push_back(r_node->keys[i]);
                            for(short i = 0; i < r_node->childs.size(); ++i)
                                l_node->childs.push_back(r_node->childs[i]);

                            // 处理父亲。
                            cur.keys.erase(key_pos);
                            cur.childs.erase(key_pos + (short) 1);

                            // 写入儿子，即使右儿子没什么卵用也暂时写入它。
                            bm.write_block(*l_node);
                            bm.write_block(*r_node);

                            // 删除成功。
                            cnt -= 2;
                            return retT(true, true);
                        }
                            // 借用一个，重新分配。
                        else {
                            if(sbl_pos < ch_pos) {
                                // borrow last key from sibling, record key position in father.
                                KeyType key_shift = sbl.keys.back();
                                offsetNumber child_shift = sbl.childs.back();

                                // remove them.
                                sbl.keys.pop_back();
                                sbl.childs.pop_back();

                                // insert shifted key in front of leaf.
                                ch.keys.insert(0, K_bt);
                                ch.childs.insert(0, child_shift);

                                // change key in father node.
                                cur.keys[key_pos] = key_shift;

                                // 写入儿子。
                                bm.write_block(ch);
                                bm.write_block(sbl);

                                // 删除成功。
                                cnt -= 2;
                                return retT(true, true);
                            }
                                // symmetric situation where sibling is to the right of p.
                            else {
                                // borrow first key from sibling.
                                KeyType key_shift = sbl.keys.front();
                                offsetNumber child_shift = sbl.childs.front();

                                // remove them.
                                sbl.keys.erase(0);
                                sbl.childs.erase(0);

                                // append shifted key.
                                ch.keys.push_back(key_shift);
                                ch.childs.push_back(child_shift);

                                // change key in father node.
                                cur.keys[key_pos] = key_shift;

                                // 写入儿子。
                                bm.write_block(ch);
                                bm.write_block(sbl);

                                // 删除成功。
                                cnt -= 2;
                                return retT(true, true);
                            }
                        }
                    }
                }
            }
        }

        KeyType split_branch(Node &B, Node &B_next) {
     #ifdef LOG
         std::cout << "split_branch" << std::endl;
     #endif
         short mid = (short) ceil(branch_degree / 2.0);
         KeyType mid_key = B.keys[mid - 1];

         B_next.childs.assign(B.childs, mid, B.childs.size());
         B_next.keys.assign(B.keys, mid, B.keys.size());

         B.childs.shorten_len(mid);
         B.keys.shorten_len(mid - (short)1);
         return mid_key;
     }

     /** sub-function used by insert_in_leaf()
     *
     * when leaf key number exceeds degree, it has to be split into two,
     * in which case key number will be exactly 'degree + 1'.
     *
     * the first node receives ceil((degree) / 2) keys
     * the second node receives the rest of them.
     * note that second node's key number always lies in BPlusTree key number range.
     *
     * example: degree = 5; range = [2, 4]; first = 2; second = 3.
     *          degree = 6; range = [3, 5]; first = 3; second = 3. */
     void split_leaf(Node &L, Node &L_next) {
     #ifdef LOG
         std::cout << "split_leaf" << std::endl;
     #endif
         short mid = (short) std::ceil((leaf_degree) / (2.0));

         L_next.keys.assign(L.keys, mid, L.keys.size());
         L_next.vals.assign(L.vals, mid, L.vals.size());

         L.keys.shorten_len(mid);
         L.vals.shorten_len(mid);
     }

        /** sub-function used by insert_node()
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
            short i = cur.search_sup(K);

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

        retT insert_node(Node &cur, const KeyType &K, const ValType &V) {
            if (cur.isLeaf) {
                bool success = insert_in_leaf(cur, K, V);
                return retT(success, success);        // return false is exists.
            } else {
                Node &ch = pool[cnt++];
                retT ret_info;

                // 向下找子。
                short ch_pos = find_child(cur, K, ch);

                // 递归进去。
                ret_info = insert_node(ch, K, V);

                // 处理儿子和自己。
                // 1. 插入未成功。
                if (!ret_info.success) {
                    cnt--;
                    return retT(false, false);
                }
                // 2. 插入成功。
                // （按照不同的类型，分别处理儿子）
                if (ch.isLeaf) {
                    // 如果不需要分裂，直接返回。
                    if (ch.keys.size() <= leaf_degree) {
                        bm.write_block(ch);
                        cnt--;
                        return retT(true, false);
                    }
                        // 如果需要分裂，处理儿子和自己。
                    else {
                        Node &newLeaf = pool[cnt++];
                        bm.append_block(newLeaf, true); // get a new leaf node to the right of cur.

                        newLeaf.next = ch.next;    // maintain address info.
                        ch.next = newLeaf.addr;
                        if (bm.tail_off == ch.addr)
                            bm.tail_off = newLeaf.addr;

                        split_leaf(ch, newLeaf);
                        cur.childs.insert(ch_pos + (short) 1, newLeaf.addr);
                        cur.keys.insert(ch_pos, newLeaf.keys[0]);

                        bm.write_block(ch);
                        bm.write_block(newLeaf);

                        cnt -= 2;
                        return retT(true, true);
                    }
                }
                    // 儿子结点为树枝。
                else {
                    // 如果不需要分裂，直接返回。
                    if (ch.childs.size() <= branch_degree) {
                        bm.write_block(ch);
                        cnt--;
                        return retT(true, false);
                    }
                        // 如果需要分裂，处理儿子和自己。
                    else {
                        KeyType mid_key;
                        Node &newBranch = pool[cnt++];
                        bm.append_block(newBranch, false);

                        mid_key = split_branch(ch, newBranch);
                        cur.childs.insert(ch_pos + (short) 1, newBranch.addr);
                        cur.keys.insert(ch_pos, mid_key);

                        bm.write_block(ch);
                        bm.write_block(newBranch);
                        cnt -= 2;
                        return retT(true, true);
                    }
                }

            }
        }

        /**
         * cur: branch node; K: key
         * return child: child of cur. return index in childs array. */
        short find_child(Node &cur, const KeyType K, Node &child) {
            short i = cur.search_sup(K);
            // fail to find, means K is larger than max key.
            if (i == -1) {
                bm.get_block_by_offset(cur.childs.back(), child);
                return cur.childs.size() - (short) 1;
            } else {
                bm.get_block_by_offset(cur.childs[i], child);
                return i;
            }
        }

        /**
         * return the leaf node K belongs.
         * Note that I'm always able to land in a leaf node. */
        void search_to_leaf(KeyType K, Node &ret) {
            bm.get_root(ret);

            while (!ret.isLeaf) {
                find_child(ret, K, ret);
            }
        }

    public:
        explicit UniqueBPlusTree(const char *fname) {
            strcpy(filename, fname);
            bm.set_fileName(fname);

            K_byte = sizeof(KeyType), V_byte = sizeof(ValType);
            cnt = 0;

            /**
             * tree utility is stored in file head, which is followed by a leaf node (the first node in an file is always leaf node).
             * Note key and child number differ by 1 in branch. */
            leaf_degree = (blockSize - tree_utility_byte - node_utility_byte) / (K_byte + V_byte);
            branch_degree = (blockSize - node_utility_byte + K_byte) / (sizeof(offsetNumber) + K_byte);

/*        #ifdef LOG
            std::cout << "leaf degree: " << leaf_degree << " branch degree: " << branch_degree << std::endl;
        #endif*/

/*            if (leaf_degree == 0)
                std::cerr << "leaf degree is 0" << std::endl;*/
        }

        ~UniqueBPlusTree() {
            if (bm.is_opened())
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
//            if(cnt >= 20)
//                std::cerr << "pool is full" << std::endl;

            Node &ret = pool[cnt++];
            search_to_leaf(K, ret);

            int i = ret.search_exact(K);

            --cnt;              // 手动析构。
            if (i == -1)     /// return default when unfind.
                return ValType();
            else
                return ret.vals[i];
        }

        /** get data lies between low key and high key, excluding two key values.
         * return value array and data length. */
        void get_range(KeyType low, KeyType high, ValType arr[], int &len) {
            len = 0;
            Node &lowLeaf = pool[cnt++];
            search_to_leaf(low, lowLeaf);

            // find Key: low.
            short pos = 0;
            while(pos < lowLeaf.keys.size() && lowLeaf.keys[pos] <= low)
                ++pos;
            if(pos == lowLeaf.keys.size()) {
                if(lowLeaf.next == -1) {
                    fprintf(stderr, "nothing greater than low\n");
                    len = 0;
                    --cnt;
                    return;
                }
                bm.get_block_by_offset(lowLeaf.next, lowLeaf);
                pos = 0;
            }
            if(lowLeaf.keys[pos] >= high) {
                fprintf(stderr, "nothing between low and high\n");
                len = 0;
                --cnt;
                return;
            }

            // traverse and fill arr.
            while(lowLeaf.keys[pos] < high) {
                arr[len++] = lowLeaf.vals[pos];
                // update leaf and pos for the next elem.

                // jump to next leaf.
                if(pos == lowLeaf.keys.size() - 1) {
                    if(lowLeaf.next == -1) {
                        --cnt;
                        return;
                    }
                    else {
                        bm.get_block_by_offset(lowLeaf.next, lowLeaf);
                        pos = 0;
                    }
                }
                else
                    ++pos;
            }
            --cnt;
        }
        /**
         * overwrite existing key, return false if key isn't found. */
        bool modify(KeyType K, ValType V) {
            Node &leafNode = pool[cnt++];
            short keyPos;

            search_to_leaf(K, leafNode);
            keyPos = leafNode.search_exact(K);
            // fail to find.
            if (keyPos == -1) {
                cnt--;
                return false;
            }
            else {
                leafNode.vals[keyPos] = V;  // modify and write back.
                bm.write_block(leafNode);
                cnt--;
                return true;
            }
        }

        /**
         * return false if key has been inserted. */
        bool insert(const KeyType &K, const ValType &V) {
        #ifdef LOG
            static int cnt = 0;
            cnt++;
            std::cout << std::endl << cnt << " insert log starts: " << std::endl;
        #endif
            Node &root = pool[cnt++];

            if (bm.root_off == -1) {
                bm.append_block(root, true);     // append a new block.

                root.keys.push_back(K);
                root.vals.push_back(V);

                bm.root_off = root.addr;  // update root.
                bm.write_block(root);

                // maintain a list.
                bm.head_off = root.addr;
                bm.tail_off = root.addr;
                cnt--;

                bm.logicSize++;
                return true;
            }

            bm.get_root(root);
            retT ret_info = insert_node(root, K, V);

            if (!ret_info.success) {
                cnt--;
                return false;
            }
                // insert successfully, may need split and increase tree height.
            else {
                if (root.isLeaf) {
                    // 如果出事了，增加树高。更新一些东西。
                    if (root.keys.size() > leaf_degree) {
                        Node &newLeaf = pool[cnt++];
                        Node &newRoot = pool[cnt++];

                        bm.append_block(newLeaf, true);
                        split_leaf(root, newLeaf);

                        newLeaf.next = root.next;    // maintain address info.
                        root.next = newLeaf.addr;
                        if (bm.tail_off == root.addr)
                            bm.tail_off = newLeaf.addr;


                        bm.append_block(newRoot, false);
                        newRoot.childs.push_back(root.addr);
                        newRoot.childs.push_back(newLeaf.addr);
                        newRoot.keys.push_back(newLeaf.keys[0]);
                        bm.root_off = newRoot.addr;

                        bm.write_block(root);
                        bm.write_block(newLeaf);
                        bm.write_block(newRoot);
                        cnt -= 3;

                        bm.logicSize++;
                        return true;
                    }
                    // 如果没出事，写入根节点。
                    else {
                        if(ret_info.modified)
                            bm.write_block(root);
                        cnt--;

                        bm.logicSize++;
                        return true;
                    }
                } else {
                    // 如果出事了，增加树高。
                    if (root.childs.size() > branch_degree) {
                        Node &newBranch = pool[cnt++];
                        Node &newRoot = pool[cnt++];
                        KeyType mid_key;

                        bm.append_block(newBranch, false);
                        mid_key = split_branch(root, newBranch);

                        bm.append_block(newRoot, false);
                        newRoot.childs.push_back(root.addr);
                        newRoot.childs.push_back(newBranch.addr);
                        newRoot.keys.push_back(mid_key);
                        bm.root_off = newRoot.addr;

                        bm.write_block(root);
                        bm.write_block(newBranch);
                        bm.write_block(newRoot);
                        cnt -= 3;

                        bm.logicSize++;
                        return true;
                    }
                    else {
                        // 如果没出事，写入根节点。
                        if(ret_info.modified)
                            bm.write_block(root);
                        cnt--;

                        bm.logicSize++;
                        return true;
                    }

                }
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
            if (bm.root_off == -1) {
                // std::cerr << "erase empty B plus tree" << std::endl;
                return false;
            }

            Node &root = pool[cnt++];
            retT ret_info;
            bm.get_root(root);
            ret_info = erase_node(root, K);

            if(!ret_info.success) {
                cnt--;
                return false;
            }
            // 看看root要不要更新呢？
            else {
                // 根节点——叶结点为空时，空置B+树。
                if(root.isLeaf) {
                    if(root.keys.size() == 0) {
                        bm.root_off = bm.head_off = bm.tail_off = -1;
                    }
                    if(ret_info.modified)
                        bm.write_block(root);
                }
                // 根节点——树枝结点只有一个儿子时，减小树高。
                else {
//                    #ifdef DEBUG
//                    if(root.childs.size() < 1)
//                        std::cerr << "root child number invalid!" << std::endl;
//                    #endif
                    if(root.childs.size() == 1) {
                        bm.root_off = root.childs[0];
                    }
                    else
                        if(ret_info.modified)
                            bm.write_block(root);
                }
                cnt--;
                // 删除成功。
                bm.logicSize--;
                return true;
            }
        }

        int size() {
            return bm.logicSize;
        }

        /** perform a level order traversal for B tree. */
        #ifdef LOG
        struct qElem {
            offsetNumber addr;
            int depth;

            qElem(offsetNumber ad, int dpt) : addr(ad), depth(dpt) {}
        };
        void view() {
            if (bm.root_off == -1) {
                std::cout << "tree is empty" << std::endl;
                return;
            }

            printf("root_offset: %d; head_offset: %d; tail_offset: %d, append_offset: %d\n\n",
                   bm.root_off, bm.head_off, bm.tail_off, bm.append_off);
            std::queue<qElem> q;
            q.emplace(bm.root_off, 1);

            while (!q.empty()) {
                qElem tmpQ = q.front();
                q.pop();

                Node tmpN;
                bm.get_block_by_offset(tmpQ.addr, tmpN);
                printf("address: %d; depth: %d", tmpQ.addr, tmpQ.depth);
                if (tmpN.isLeaf) {
                    printf("; next: %d; leafNode\nkeys: ", tmpN.next);
                    for (int i = 0; i < tmpN.keys.size(); ++i) {
                        std::cout << tmpN.keys[i] << ' ';
                    }

                    printf("\nvals: ");
                    for (int i = 0; i < tmpN.vals.size(); ++i) {
                        std::cout << tmpN.vals[i] << ' ';
                    }

                    printf("\n\n");
                } else {
                    printf("; branchNode\nkeys: ");
                    for (int i = 0; i < tmpN.keys.size(); ++i) {
                        std::cout << tmpN.keys[i] << ' ';
                    }

                    printf("\nchildren: ");
                    for (int i = 0; i < tmpN.childs.size(); ++i) {
                        std::cout << tmpN.childs[i] << ' ';
                    }
                    printf("\n\n");

                    for (int i = 0; i < tmpN.childs.size(); ++i)
                        q.emplace(tmpN.childs[i], tmpQ.depth + 1);
                }
            }
        }
        #else
        void view() {}
        #endif
    };
};
#endif //DATABASE_BPLUSTREE_H
