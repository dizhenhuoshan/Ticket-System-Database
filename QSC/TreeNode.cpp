#include "TreeNode.h"

template <class KeyType>
TreeNode<KeyType>::TreeNode(bool leaf) {
    parent = nullptr;
    keys.clear();
    vals.clear();
    childs.clear();
    next = nullptr;

    isLeaf = leaf;
}

template <class KeyType>
TreeNode<KeyType>::TreeNode(const TreeNode &other) {
    parent = other.parent;
    keys = other.keys;
    vals = other.vals;
    childs = other.childs;
    next = other.next;
    isLeaf = other.isLeaf;
}

template <class KeyType>
int TreeNode<KeyType>::search_sup(KeyType K) {
    int pos = 0;
    while(pos < keys.size() && keys[pos] < K) ++pos;

    if(pos == keys.size()) return -1;
    else return pos;
}

template <class KeyType>
int TreeNode<KeyType>::search_exact(KeyType K) {
    int pos = 0;
    while(pos < keys.size() && keys[pos] != K) ++pos;

    if(pos == keys.size()) return -1;
    else return pos;
}

template <class KeyType>
void TreeNode<KeyType>::clear() {
    keys.clear();
    vals.clear();
    childs.clear();
}

template <class KeyType>
size_t TreeNode<KeyType>::search_child(TreeNode *child) {
    size_t i = 0;
    while(i < childs.size() && childs[i] != child) ++i;

    if(i == childs.size()) throw "unmatched father and childs";
    else return i;
}
