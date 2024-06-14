#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include "LinkedArrayTree.cpp"

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
void TreeTest(unordered_set<T> set)
{
    string message = "--------------Linked Array Tree--------------";
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

    LinkedArrayTree<uint64_t,uint64_t> lat;

    timerStart = clock::now();
    for (auto i : set)
    {
        lat.insert(i,i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    cout<<message<<endl;

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        cout<<message<<endl;
    }

    timerStart = clock::now();
    for (auto i : set)
    {
        lat.get(i);
    }

    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "random search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    cout<<message<<endl;

    timerStart = clock::now();
    for (auto i =0;i< set.size();i++)
    {
        lat.get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    cout<<message<<endl;

    timerStart = clock::now();
    for (auto i : set)
    {
        lat.remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    cout<<message<<endl;
}

int main(){
    // TreeTest<uint64_t>(getRandom<uint64_t>(10000000));
    // the first test result is astonishing. Write in 6/14/2024, the time take for searching
    // in 100 elements and 1000000000 elements are the same. Even the key is 64 bits, one search only takes 10-30 ns.
    // The most performance devour part is the remainder calculation. Since I eliminated it, the result is in expection.

    LinkedArrayTree<uint16_t,uint64_t> lat;
    lat.printStructureInfo();
    lat.insert(1,1);
    lat.insert(5,5);
    lat.insert(266,266);
    lat.insert(300,300);
    lat.insert(600,600);
    lat.insert(900000000,900000000);
    lat.printPath(266);
    lat.printAllData();
    // cout<<*lat.get(266);
    // lat.remove(266);
    // lat.printPath(266);
    // lat.printPath(1);

    // uint64_t i = 2000;
    // uint8_t j= 100;
    // uint8_t* p = (uint8_t*)&i;
    // cout<<(int)p[0]<<" ";
    // cout<<(int)p[1]<<" ";
    // cout<<(int)p[2]<<" ";
    // cout<<(int)p[3]<<" ";
    // cout<<(int)p[4]<<" ";
    // cout<<(int)p[5]<<" ";
    // cout<<(int)p[6]<<" ";
    // cout<<(int)p[7]<<" ";

    // cout<<endl;

}