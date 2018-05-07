#ifndef MYDATABASETHIRDVERSION_BPLUSTREE_H
#define MYDATABASETHIRDVERSION_BPLUSTREE_H

#include <cstdio>
#include <vector>
#include "TreeNode.h"

/**
 * assume there are no duplicated key, since we use vector, forget about unnecessary field.
 *
 * try to make it compatible to duplicated key now.
 * */

using namespace std;

typedef int offsetNumber;

/**
 * initially, I wanna design LeafNode and BranchNode separately.
 * degree is stored in B-tree class instead of node class,
 * because node class is adaptable to B-tree with different degrees,
 * which means degree is BPlustree's feature.
 *
 * if we do this, data type will be heterogeneous, and hard to manage.
 * information about keys, vals, and childs are well encapsulated inside <vector> class.
 * we omit unnecessary fields. */

template <class KeyType>
class BPlusTree {
private:
    typedef TreeNode <KeyType> Node;

    // a struct containing the node containing a specific key
    struct searchNodeParse {
        Node *pNode;
        size_t index;   // position of key
        bool ifFound;   // the flag of whether the key is found
    };
private:
    Node *root;
    /**
     * leaf node key number range: [ceil((degree - 1) / 2), degree - 1]
     * branch node childs range: [ceil(degree / 2), degree]
     * branch node key number = childs number - 1 */
    int degree;

    /**
     * a variant version of find, adaptive to duplicated keys situations
     *
     * whether it's reliable is doubtful, because of its way of dealing with
     * node with equal key (search the previous node all the way down to leaf,
     * and, if not found yet, turn to its next brother)
     *
     * no need to concern about duplicated keys
     * we only need to find a 'proper' position to insert, rather than find exactly
     * the first or last leaf element with this key. */
    searchNodeParse find_first(Node *N, KeyType K);

    /**
     * return the leaf node K belongs (not guarantee to be the first one) */
    Node* search_to_leaf(KeyType K);

     /** sub-function used by insert()
      *
      * insert key and value in node pointed by p
      * insert in the first position of the same key value, if duplicated key is allowed,
      * adapting to search_sup method of Node class */
     void insert_in_leaf(Node *p, KeyType K, offsetNumber val);

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
     void split_leaf(Node *L, Node *L_next, Node *tmp);

     /** sub-function used by insert_in_parent()
      *
      * when branch's childs exceeds degree, it has to be split into two,
      * in which case childs number will be exactly 'degree + 1'.
      *
      * the first node receives ceil(degree / 2) childs.
      * the second node receives the rest of them.
      * we regard childs (pointers) as primary concern. Keys between them are sort of like bi-products
      *
      * note that the very key between
      * "the right most pointer in first node" and "the left most pointer in second node"
      * can't be assigned in either of B or B_next.
      * it's actually inserted into further ancestor node. The whole procedure is recursive, to be precise.
      *
      * The awkward extra key is provided in return value */
     KeyType split_branch(Node *B, Node *B_next, Node *tmp);

    /**
     * p is old node which has been split into two
     * p_next is the new one
     * K is the smallest key in p_next (in leaf to branch case)
     *
     * Note that branch nodes' key is always present in the front of leaves,
     * which is sadly not true for branch-branch case */
     void insert_in_parent(Node *p, KeyType K, Node *p_next);

     void erase_branch(Node *p, KeyType K, Node *child);
     void erase_leaf(Node *p, KeyType K);

     /**
      * sub-function sued by erase_branch and erase_leaf, get one of
      * the two siblings of p which has more pointers
      *
      * p is assured to have siblings because of the condition given by B-Plus Tree*/
     Node *getSibling(Node *p);
public:
    explicit BPlusTree(int degree);

    /// find: unique key, find_first: duplicated key
    searchNodeParse find(KeyType K);
    searchNodeParse find_first(KeyType K) {
        return find_first(root, K);
    }

    void insert(KeyType K, offsetNumber val);

    /**
     * this erase operation can only adapt to unique key situation
     *
     * erase is an iterative process which goes bottom up, and uses erase_leaf first,
     * which then invokes erase_branch to do back track. */
    void erase(KeyType K);
};

#include "BPlusTree.cpp"

#endif //MYDATABASETHIRDVERSION_BPLUSTREE_H
