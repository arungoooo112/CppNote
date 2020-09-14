#include <thread>
#include <iostream>
using namespace std;
int tag = 0;
int cnt = 0;

void add()
{
    while (cnt++ < 100){
        cout << "add() " << tag << endl;
        tag++;
    }
}

void sub()
{
    while (cnt++ < 100){
        cout << "sub()" << tag << endl;
        tag--;
    }
}

int main()
{
    thread th1(add);
    thread th2(sub);
    th1.join();
    th2.join();
}