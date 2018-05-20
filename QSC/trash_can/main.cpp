#include <iostream>
#include "TreeNode.h"
#include "vector.h"
#include <vector>
#include "timer.h"
#include "BPlusTree.h"
#include "debug.h"
#include "keys.h"
#include <map>

using namespace std;

int main() {
    sjtu::UniqueBPlusTree<double, double> bp("test6");
    bp.open_file();

    bp.close_file();
    return 0;
}
