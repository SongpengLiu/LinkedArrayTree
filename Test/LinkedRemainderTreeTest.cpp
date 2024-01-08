#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include "../LinkedRemainderTree/LinkedRemainderTree.cpp"
using namespace std;

template <class T>
unordered_set<T> getRandom(T size, T max)
{
    std::unordered_set<T> randomSet;
    std::random_device rd;

    if (size > max)
    {
        throw std::invalid_argument("invalid index");
        return randomSet;
    }

    // size is small, generate directly
    if (size < 1000)
    {
        while (randomSet.size() < size)
        {
            randomSet.insert(rd() % max);
        }
        return randomSet;
    }

    // size is mall compared with max, generate directly
    if (max -size > max / 100)
    {
        while (randomSet.size() < size)
        {
            randomSet.insert(rd() % max);
        }
        return randomSet;
    }

    // size is big compared with max
    while (randomSet.size() < size - max/100)
    {
        randomSet.insert(rd() % max);
    }

    for (int i = 0; i < max; i++)
    {
        if (randomSet.find(i) == randomSet.end())
        {
            randomSet.insert(i);
        }
        if (randomSet.size() >= size)
        {
            break;
        }
    }
    return randomSet;
}

template <class T>
set<T> getOrdered(T size, T max){
    set<T> set;
    for(int i=0;i<size;i++){
        set.insert(i);
    }
    return set;
}

int main(){
    unsigned int max = 100;
    unsigned int size= 100;
    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(size,max);
    LinkedRemainderTree<unsigned int> *tree = new LinkedRemainderTree<unsigned int>(max);
    for(auto i: randomSet){
        tree->insert(i,i);
    }
    tree->printPath(20);
    tree->remove(20);
    tree->printPath(20);
    tree->destroy();
    tree->printPath(20);
}