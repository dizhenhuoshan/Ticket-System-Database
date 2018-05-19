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
    /*
    sjtu::BufferManager<int, double> bm;
    bm.set_file("test");

    sjtu::TreeNode<int, double> node, node1;
    bm.append_block(node, true);

    for (int i = 0; i < 10; ++i) {
        node.keys.push_back(i);
        node.vals.push_back((double) i / 2.0);
    }
    bm.write_block(node);

    bm.get_block_by_offset(3, node1);

    std::cout << node1.vals.size() << ' ' << node1.keys.size() << std::endl;

    for(int i = 0; i < 10; ++i) {
        std::cout << node1.vals[i] << ' ' ;
    }
    std::cout << std::endl;
    */

    //timer t;
    double st, et;

    ///st = t.get_time();
    sjtu::UniqueBPlusTree<double, double> bp("test6");
    bp.open_file();

    for(int i = 0; i < 30; ++i)
        bp.insert(i, i);

    while(true) {
        double l, h;
        cout << "low and high: ";
        cin >> l >> h;

        if(l == -2)
            break;
        int len;
        double arr[100];

        bp.get_range(l, h, arr, len);
        for(int i = 0; i < len; ++i)
            cout << arr[i] << ' ';
        cout << endl;
    }
    bp.close_file();
    return 0;
}
