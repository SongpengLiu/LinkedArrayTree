#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include "../LinkedRemainderList/LinkedRemainderList.cpp"
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
    int size =10000;
    int max = 20000;

    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(size, max);
    LinkedRemainderList<unsigned int> *skipList = new LinkedRemainderList<unsigned int>(max);
    for(auto i: randomSet){
        skipList -> insert(i,i);
    }

    skipList->printInfo();
    // skipList -> printAllNodes();
    for(auto i: randomSet){
        // skipList->printPath(i);
        skipList -> remove(i);
    }
    skipList -> printAllNodes();

    delete skipList;
    return 0;
}