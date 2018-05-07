#ifndef MYDATABASETHIRDVERSION_TREENODE_H
#define MYDATABASETHIRDVERSION_TREENODE_H

#include <vector>
using namespace std;

typedef int offsetNumber;

template <class KeyType>
class TreeNode {
public:
    TreeNode <KeyType> *parent;
    vector <KeyType> keys;      // keys
    vector <offsetNumber> vals; // offsetNumber leads to record
    vector <TreeNode *> childs;
    TreeNode <KeyType> *next;

    bool isLeaf;

    explicit TreeNode(bool leaf);
    TreeNode(const TreeNode &other);

    /**
     * search for i, where i is the smallest value with key[i] >= K. if search fails, return -1
     * it may be regarded as finding sup of K in set of Kis */
    int search_sup(KeyType K);
    /**
     * find exactly key[i] == K. if search fails, return -1
     * in the case of duplicated key, return the left-most one */
    int search_exact(KeyType K);

    /**
     * reset node content, parent, next, and isLeaf remains */
    void clear();

    /**
     * Node must be branch node, search the position of the specific child in this node.
     * I must be able to find it, otherwise throw exception */
    size_t search_child(TreeNode *child);
};

#include "TreeNode.cpp"

#endif //MYDATABASETHIRDVERSION_TREENODE_H
