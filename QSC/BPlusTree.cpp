#include "BPlusTree.h"
#include <cmath>

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
        if(i == -1) cur = cur->children.back();
            // have found
            /* We do things in this manner because the same scheme can be generalized
             * to cases with duplicated keys. */
        else {
            if(K == cur->keys[i]) cur = cur->children[i + 1];
            else cur = cur->children[i];
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
            deeper = N->children.back();
            return find_first(deeper, K);
        }

        // now K <= N->keys[i], but we aren't sure which pointer K belongs it belongs to.
        else {
            if(K == N->keys[i]) {
                searchNodeParse ret;
                deeper = N->children[i];

                ret = find_first(deeper, K);
                if(!ret.ifFound) {
                    deeper = N->children[i + 1];
                    return find_first(deeper, K);
                }
                else return ret;
            }
            else {
                deeper = N->children[i];
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

    B->children.assign(tmp->children.begin(), tmp->children.begin() + mid);
    B->keys.assign(tmp->keys.begin(), tmp->keys.begin() + mid - 1);

    B_next->children.assign(tmp->children.begin() + mid, tmp->children.end());
    B_next->keys.assign(tmp->keys.begin() + mid, tmp->keys.end());

    return mid_key;
}

template <class KeyType>
void BPlusTree<KeyType>::insert_in_parent(BPlusTree<KeyType>::Node *p, KeyType K, BPlusTree<KeyType>::Node *p_next) {
    // new root is needed
    if(root == p) {
        root = new Node(false);
        root->children.push_back(p);
        root->children.push_back(p_next);
        root->keys.push_back(K);
        p->parent = p_next->parent = root;
        return;
    }
    // pick out p's parent, update parent field.
    Node *fa = p->parent;
    p_next->parent = fa;
    /* insert into parent first, regardless of whether there's empty slot available */
    size_t p_idx = fa->search_child(p);
    fa->children.insert(fa->children.begin() + p_idx + 1, p_next);      // insert p_next after p's index
    fa->keys.insert(fa->keys.begin() + p_idx, K);                       // there's one slot shift in keys

    // check whether split is needed, distribute keys and children
    if(fa->children.size() > degree) {
        Node *tmp = new Node(*fa);
        Node *fa_next(false);
        KeyType mid_key;

        fa->clear();
        mid_key = split_branch(fa, fa_next, tmp);
        insert_in_parent(fa, mid_key, fa_next);

        delete tmp;
    }
}