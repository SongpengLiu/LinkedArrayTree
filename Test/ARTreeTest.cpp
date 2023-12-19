#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include "../ArrayRemainderTree/ArrayRemainderTree.cpp"
using namespace std;

template <class T>
unordered_set<T> getRandom(T number, T maxValue)
{
    std::unordered_set<T> randomSet;
    std::random_device rd;
    if (number > maxValue)
    {
        throw std::invalid_argument("invalid index");
        return randomSet;
    }
    if(number*11>maxValue*10){
        throw std::invalid_argument("maxValue too small, hard to generate so many random number");
        return randomSet;
    }
    while(randomSet.size()<number){
            randomSet.insert(rd() % maxValue);
    }
    return randomSet;
}

int main(){
    int size =1000000;
    int max = 1100000;

    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(size, max);
    ArrayRemainderTree<unsigned int> *tree = new ArrayRemainderTree<unsigned int>(max);
    for(auto i: randomSet){
        tree -> insert(i,i);

    }
    tree->printInfo();

    cout<<"search: "<<tree->get(*randomSet.begin())<<endl;
    tree->printPath(*randomSet.begin());
    tree->remove(*randomSet.begin());
    // tree->destroy();
    tree->printPath(*randomSet.begin());
    return 0;
}