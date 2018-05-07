#include "BPlusTree.h"
#include <cmath>

template <class T>
void swap(T &a, T &b) {
    T tmp(a);
    a = b;
    b = tmp;
}


template <class KeyType>
BPlusTree<KeyType>::BPlusTree(int degree) {
    root = nullptr;
    this -> degree = degree;
}

template <class KeyType>
typename BPlusTree<KeyType>::Node* BPlusTree<KeyType>::search_to_leaf(KeyType K) {
    Node *cur = root;

    while(!cur->isLeaf) {
        int i = cur->search_sup(K);

        // fail to find, means K is larger than max
        if(i == -1) cur = cur->childs.back();
            // have found
            /* We do things in this manner because the same scheme can be generalized
             * to cases with duplicated keys. */
        else {
            if(K == cur->keys[i]) cur = cur->childs[i + 1];
            else cur = cur->childs[i];
        }
    }

    return cur;
}

template <class KeyType>
typename BPlusTree<KeyType>::searchNodeParse BPlusTree<KeyType>::find(KeyType K) {
    searchNodeParse ret;
    Node *cur = search_to_leaf(K);

    int i = cur->search_exact(K);
    if(i == -1) {
        ret.ifFound = false;
        ret.pNode = nullptr;
        ret.index = cur->keys.size();
    }
    else {
        ret.ifFound = false;
        ret.pNode = cur;
        ret.index = (size_t) i;
    }
    return ret;
}

template <class KeyType>
typename BPlusTree<KeyType>::searchNodeParse BPlusTree<KeyType>::find_first(BPlusTree<KeyType>::Node *N, KeyType K) {
    Node *deeper;

    // N is branch
    if(!N->isLeaf) {
        // 让 i 成为满足查找K小于等于当前结点中key值的最小index
        int i = N->search_sup(K);

        // search fails
        if(i == -1) {
            deeper = N->childs.back();
            return find_first(deeper, K);
        }

        // now K <= N->keys[i], but we aren't sure which pointer K belongs it belongs to.
        else {
            if(K == N->keys[i]) {
                searchNodeParse ret;
                deeper = N->childs[i];

                ret = find_first(deeper, K);
                if(!ret.ifFound) {
                    deeper = N->childs[i + 1];
                    return find_first(deeper, K);
                }
                else return ret;
            }
            else {
                deeper = N->childs[i];
                return find_first(deeper, K);
            }
        }
    }

    // N is leaf
    searchNodeParse ret;
    int i = N->search_exact(K);
    if(i == -1) {
        ret.ifFound = false;
        ret.pNode = nullptr;
        ret.index = N->keys.size();
    }
    else {
        ret.ifFound = true;
        ret.pNode = N;
        ret.index = (size_t) i;
    }
    return ret;
}

template <class KeyType>
void BPlusTree<KeyType>::insert(KeyType K, offsetNumber val) {
    if(root == nullptr) {
        root = new Node(true);
        return;
    }

    Node *cur = search_to_leaf(K);
    // if leaf node has extra slot
    if(cur->keys.size() < degree - 1) {
        insert_in_leaf(cur, K, val);
    }
    // split node
    else {
        Node *next_leaf = new Node(true);
        next_leaf->next = cur->next;
        cur->next = next_leaf;

        /* we can't do this so far, because cur->parent may not be created yet.
         * next_leaf->parent = cur->parent; */

        // new is slow, should optimize it later
        Node *tmp = new Node(*cur);
        insert_in_leaf(tmp, K, val);

        cur->clear();
        split_leaf(cur, next_leaf, tmp);

        insert_in_parent(cur, next_leaf->keys[0], next_leaf);
        delete tmp;
    }
}

template <class KeyType>
void BPlusTree<KeyType>::insert_in_leaf(BPlusTree<KeyType>::Node *p, KeyType K, offsetNumber val) {
    // find index of K's sup, if exist
    int i = p->search_sup(K);

    // no sup, insert in the back
    if(i == -1) {
        p->keys.push_back(K);
        p->vals.push_back(val);
        return ;
    }
    else {
        p->keys.insert(p->keys.begin() + i, K);
        p->vals.insert(p->vals.begin() + i, val);
    }
}

template <class KeyType>
void BPlusTree<KeyType>::split_leaf(BPlusTree<KeyType>::Node *L, BPlusTree<KeyType>::Node *L_next,
                               BPlusTree<KeyType>::Node *tmp) {
    size_t mid = (size_t)ceil((degree - 1) / 2);

    L->keys.assign(tmp->keys.begin(), tmp->keys.begin() + mid);
    L->vals.assign(tmp->vals.begin(), tmp->vals.begin() + mid);

    L_next->keys.assign(tmp->keys.begin() + mid, tmp->keys.end());
    L_next->vals.assign(tmp->keys.begin() + mid, tmp->keys.end());
}

template <class KeyType>
KeyType BPlusTree<KeyType>::split_branch(BPlusTree<KeyType>::Node *B, BPlusTree<KeyType>::Node *B_next,
                                      BPlusTree<KeyType>::Node *tmp) {
    size_t mid = (size_t)ceil(degree / 2);
    KeyType mid_key = tmp->keys[mid - 1];

    B->childs.assign(tmp->childs.begin(), tmp->childs.begin() + mid);
    B->keys.assign(tmp->keys.begin(), tmp->keys.begin() + mid - 1);

    B_next->childs.assign(tmp->childs.begin() + mid, tmp->childs.end());
    B_next->keys.assign(tmp->keys.begin() + mid, tmp->keys.end());

    return mid_key;
}

template <class KeyType>
void BPlusTree<KeyType>::insert_in_parent(BPlusTree<KeyType>::Node *p, KeyType K, BPlusTree<KeyType>::Node *p_next) {
    // new root is needed
    if(root == p) {
        root = new Node(false);
        root->childs.push_back(p);
        root->childs.push_back(p_next);
        root->keys.push_back(K);
        p->parent = p_next->parent = root;
        return;
    }
    // pick out p's parent, update parent field.
    Node *fa = p->parent;
    p_next->parent = fa;
    /* insert into parent first, regardless of whether there's empty slot available */
    size_t p_idx = fa->search_child(p);
    fa->childs.insert(fa->childs.begin() + p_idx + 1, p_next);      // insert p_next after p's index
    fa->keys.insert(fa->keys.begin() + p_idx, K);                       // there's one slot shift in keys

    // check whether split is needed, distribute keys and childs
    if(fa->childs.size() > degree) {
        Node *tmp = new Node(*fa);
        Node *fa_next(false);
        KeyType mid_key;

        fa->clear();
        mid_key = split_branch(fa, fa_next, tmp);
        insert_in_parent(fa, mid_key, fa_next);

        delete tmp;
    }
}

template <class KeyType>
void BPlusTree<KeyType>::erase(KeyType K) {
    Node *leaf = search_to_leaf(K);
    erase_leaf(leaf, K);
}

template <class KeyType>
BPlusTree<KeyType>::Node * BPlusTree<KeyType>::getSibling(BPlusTree<KeyType>::Node *p) {
    Node *fa = p->parent;
    size_t child_idx = fa->search_child(p);

    // we are left with no choice
    if(child_idx == 0) return fa->childs[1];
    else if(child_idx == fa->childs.size() - 1) return fa->childs[fa->childs.size() - 2];
    else {
        Node *child1 = fa->childs[child_idx - 1];
        Node *child2 = fa->childs[child_idx + 1];

        /** because this function is generic to both branch and leaf,
         * we compare their key numbers, which is a field held by both of them */
        if(child1->keys.size() > child2->keys.size()) return child1;
        else return child2;
    }
}


template <class KeyType>
void BPlusTree<KeyType>::erase_leaf(BPlusTree<KeyType>::Node *p, KeyType K) {
    int K_idx = p->search_exact(K);

    p->keys.erase(p->keys.begin() + K_idx);
    p->vals.erase(p->vals.begin() + K_idx);

    // if node is too empty, do something
    if(p->keys.size() < (size_t) ceil((degree - 1) / 2)) {
        Node *sbl = getSibling(p);

        // sbl and p are both children in a branch node, find the key between them.
        size_t p_idx = p->parent->search_child(p);
        size_t sbl_idx = p->parent->search_child(sbl);

        KeyType K_bt = p->parent->keys[min(p_idx, sbl_idx)];    // K_bt means key in between
        int key_pos = p->parent->search_exact(K_bt);

        // p and sibling can be put in one single node
        if(p->keys.size() + sbl->keys.size() < degree) {
            /** make sure that sibling is in front of p, we can do this because
             * merging two nodes makes they symmetric to each other. They are
             * they satisfy commutative law */
            if(p_idx < sbl_idx) swap(sbl, p);

            for(KeyType key : p->keys) sbl->keys.push_back(key);
            for(offsetNumber val : p->vals) sbl->vals.push_back(val);

            sbl->next = p->next;

            erase_branch(p->parent, K_bt, p);   // delete current node p in parent node
            delete p;
        }
        // redistribution, borrow a key from sibling to p.
        else {
            if(sbl_idx < p_idx) {
                // borrow last key from sibling, record key position in father.
                KeyType key_shift = sbl->keys.back();
                offsetNumber val_shift = sbl->vals.back();

                // remove them
                sbl->keys.pop_back(), sbl->vals.pop_back();

                // insert shifted key in front of p
                p->keys.insert(p->keys.begin(), key_shift);
                p->vals.insert(p->vals.begin(), val_shift);

                // change key in father node
                p->parent->keys[key_pos] = p->keys[0];
            }
            // symmetric situation where sibling is to the right of p
            else {
                // borrow first key from sibling
                KeyType key_shift = sbl->keys.front();
                offsetNumber val_shift = sbl->vals.front();

                // remove them
                sbl->keys.erase(sbl->keys.begin());
                sbl->vals.erase(sbl->vals.begin());

                // insert it in back of p
                p->keys.push_back(key_shift);
                p->vals.push_back(val_shift);

                // change key in father node
                p->parent->keys[key_pos] = sbl->keys[0];
            }
        }
    }
}

/// this function is similar to erase_leaf, but we separate it out for clarity.
template <class KeyType>
void BPlusTree<KeyType>::erase_branch(BPlusTree<KeyType>::Node *p, KeyType K, BPlusTree<KeyType>::Node *child) {
    int K_idx = p->search_exact(K);
    size_t child_idx = p->search_child(child);

    p->keys.erase(p->keys.begin() + K_idx);
    p->childs.erase(p->childs.begin() + child_idx);

    // switch root
    if(p == root && p->childs.size() == 1) {
        root = p->childs[0];
        p->childs[0]->parent = nullptr;
        delete p;
    }
    // if p become too empty, note that root node is a special case
    else if(p != root && p->keys.size() < (size_t)ceil(degree / 2)){
        Node *sbl = getSibling(p);

        // sbl and p are both children in a branch node, find the key between them.
        size_t p_idx = p->parent->search_child(p);
        size_t sbl_idx = p->parent->search_child(sbl);

        KeyType K_bt = p->parent->keys[min(p_idx, sbl_idx)];    // K_bt means key in between
        int key_pos = p->parent->search_exact(K_bt);

        // p and sibling can be put in one single node
        if(p->keys.size() + sbl->keys.size() <= degree) {
            /** make sure that sibling is in front of p, we can do this because
             * merging two nodes makes they symmetric to each other. They are
             * they satisfy commutative law */
            if(p_idx < sbl_idx) swap(sbl, p);

            // append
            sbl->keys.push_back(K_bt);
            for(KeyType key : p->keys) sbl->keys.push_back(key);
            for(Node *son : p->childs) sbl->childs.push_back(son);

            erase_branch(p->parent, K_bt, p);   // delete current node p in parent node
            delete p;
        }
        // redistribution, borrow a key from sibling to p.
        else {
            if(sbl_idx < p_idx) {
                // borrow last key from sibling, record key position in father.
                KeyType key_shift = sbl->keys.back();
                Node *child_shift = sbl->childs.back();

                // remove them
                sbl->keys.pop_back(), sbl->childs.pop_back();

                // insert shifted key in front of p
                p->keys.insert(p->keys.begin(), K_bt);
                p->childs.insert(p->childs.begin(), child_shift);

                // change key in father node
                p->parent->keys[key_pos] = key_shift;
            }
            // symmetric situation where sibling is to the right of p
            else {
                // borrow first key from sibling
                KeyType key_shift = sbl->keys.front();
                Node *child_shift = sbl->childs.front();

                // remove them
                sbl->keys.erase(sbl->keys.begin());
                sbl->childs.erase(sbl->childs.begin());

                // insert it in back of p
                p->keys.push_back(K_bt);
                p->childs.push_back(child_shift);

                // change key in father node
                p->parent->keys[key_pos] = key_shift;
            }
        }
    }
}