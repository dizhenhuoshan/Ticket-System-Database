#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;
    for(int i = 0; i < 10; ++i) v.push_back(i);

    v.insert(v.begin() + 3, -1);
    vector<int> v1;
    v1.assign(v.begin(), v.begin() + 2);
    for(int i : v1) {
        cout << i << ' ';
    }
    return 0;
}