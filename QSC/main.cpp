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

    timer t;
    double st, et;

    ///st = t.get_time();
    sjtu::UniqueBPlusTree<int, sjtu::Train_id_Key> bp("test6");
    bp.open_file();
    for(int i = 0; i < 100000; ++i) {
        bp.insert(i, sjtu::Train_id_Key("test file"));
        ///cout << bp.find(i).get_train_id() << '\n';
    }
    for (int i = 0; i < 100000; i++)
    {
        bp.erase(i);
    }
    bp.close_file();
    //et = t.get_time();
    //cout << et - st << endl;
    //注意一下下，注释尽量写英文，因为中文注释在mac和linux下互通，但是在win下是不能看的，因为编码问题。
    //另外，测试时间可用命令 time ./****来写， mac下应该也有类似的命令
    return 0;
}
