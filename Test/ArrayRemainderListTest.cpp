#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include "../ArrayRemainderList/ArrayRemainderList.cpp"
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
    int size =20;
    int max = 22;

    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(size, max);
    ArrayRemainderList<unsigned int> *list = new ArrayRemainderList<unsigned int>(max,5);
    for(auto i: randomSet){
        list -> insert(i,i);

    }
    list->printInfo();
    list->printPath(1);
    list->printAllData();
    list->remove(0);
    list->remove(1);
    list->remove(2);
    list->remove(3);
    list->remove(4);
    list->printAllData();
    return 0;
}