#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include "../RemainderSkipList/RemainderSkipList.cpp"
using namespace std;

void testConstructor(){
    RemainderSkipList<int>* rsl = new RemainderSkipList<int>(3,2);
    rsl->printInfo();

    RemainderSkipList<int>* rsl2 = new RemainderSkipList<int>(7,2);
    rsl2->printInfo();

    RemainderSkipList<int>* rsl3 = new RemainderSkipList<int>(8,2);
    rsl3->printInfo();

    RemainderSkipList<int>* rsl4 = new RemainderSkipList<int>(4,2);
    rsl4->printInfo();
}

void testAdd(){
    RemainderSkipList<int>* rsl = new RemainderSkipList<int>(100);
    rsl->printInfo();
    rsl->add(1,999);
    // rsl->printPath(1);
    rsl->add(5,3555);
    rsl->add(3,449);
    rsl->add(4,45569);
    rsl->add(7,778);
    rsl->add(2,88549);
    rsl->add(10,777);
    rsl->add(8,645654);

    rsl->printAllNodes();
    cout<<"ddff: "<<rsl->get(10);
}


int main(){
    // testConstructor();
    testAdd();
}
