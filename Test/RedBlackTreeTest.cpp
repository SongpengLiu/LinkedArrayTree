#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include <map>
#include "../tlx/tlx/container/btree_map.hpp"
using namespace std;

template <class T>
unordered_set<T> getRandom(T size)
{
    std::unordered_set<T> randomSet;
    std::random_device rd;

    // size is small, generate directly
    if (size < 10000)
    {
        while (randomSet.size() < size)
        {
            randomSet.insert(rd() % size);
        }
        return randomSet;
    }

    // size is big compared with max
    while (randomSet.size() < size - size/100)
    {
        randomSet.insert(rd() % size);
    }

    for (int i = 0; i < size; i++)
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
void RBTreeTest(unordered_set<T> set)
{
    string message = "--------------Red-black Tree--------------";
    cout<<message<<endl;

    typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : set)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / set.size());
    cout<<message<<endl;

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        cout<<message<<endl;
    }

    map<uint64_t,uint64_t> m; //C++

    timerStart = clock::now();
    for (auto i : set)
    {
        m.insert(std::make_pair(i, i));
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        cout<<message<<endl;
    }

    timerStart = clock::now();
    for (auto i : set)
    {
        m.find(i);
    }

    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "random search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    cout<<message<<endl;

    timerStart = clock::now();
    for (auto i =0;i< set.size();i++)
    {
        m.find(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    cout<<message<<endl;

    timerStart = clock::now();
    for (auto i : set)
    {
        m.erase(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    cout<<message<<endl;
}
int main(){
    RBTreeTest<uint64_t>(getRandom<uint64_t>(1000000));
}