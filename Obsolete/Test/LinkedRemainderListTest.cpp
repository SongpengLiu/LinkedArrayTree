#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include "../LinkedRemainderList/LinkedRemainderList.cpp"
#include "./TestLog.cpp"
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

template <class T>
void linkedRemainderListTest(unsigned short radix, unsigned int size, unsigned int max, set<T> set)
{
    string message = "--------------Linked Remainder List--------------";
    TestLog::appendLog(message);

        typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : set)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / set.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    LinkedRemainderList<unsigned int> *list;
    if(radix>1){
     list = new LinkedRemainderList<unsigned int>(max,radix);
    }
    else{
        list = new LinkedRemainderList<unsigned int>(max);
    }
    message = list->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        list->insert(i, i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : set)
    {
        list->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        list->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    delete list;
}

template <class T>
void reverseTest(unsigned short radix, unsigned int size, unsigned int max, set<T> set)
{
    string message = "--------------Linked Remainder List--------------";
    TestLog::appendLog(message);

        typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : set)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / set.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    LinkedRemainderList<unsigned int> *list;
    if(radix>1){
     list = new LinkedRemainderList<unsigned int>(max,radix);
    }
    else{
        list = new LinkedRemainderList<unsigned int>(max);
    }
    message = list->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i = set.rbegin();i!=set.rend();++i)
    {
        list->insert(*i, *i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i = set.rbegin();i!=set.rend();++i)
    {
        list->get(*i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i = set.rbegin();i!=set.rend();++i)
    {
        list->remove(*i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    delete list;
}

int main(){
    int size =100;
    int max = 100;

    set<unsigned int> set = getOrdered<unsigned int>(size,max);
    linkedRemainderListTest(0,size,max,set);
    reverseTest(0,size,max,set);
    TestLog::printLog();

    return 0;
}