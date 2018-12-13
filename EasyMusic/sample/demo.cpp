#include <iostream>
#include <queue>

using namespace std;

int main() {
    queue<int*> mQueue;
    mQueue.push(new int(3));
    mQueue.push(new int(2));
    mQueue.push(new int(1));

    int* a = mQueue.front(); mQueue.pop();
    cout << "#1:" << *a << ", size :" << mQueue.size() << endl; 
    delete a;

    int* b = mQueue.front(); mQueue.pop();
    cout << "#2:" << *b << ", size :" << mQueue.size() << endl; 
    delete b;

    int* c = mQueue.front(); mQueue.pop();
    cout << "#3:" << *c << ", size :" << mQueue.size() << endl; 
    delete c;

    return 0;
}