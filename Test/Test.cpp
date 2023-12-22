#include <iostream>
#include <chrono>
#include <unordered_set>
#include <random>
#include <windows.h>
#include <psapi.h>
#include "../RedBlackTree/RBTree.cpp"
#include "../BTree/BTree.cpp"
#include "../LinkedRemainderTree/LinkedRemainderTree.cpp"
#include "../ArrayRemainderTree/ArrayRemainderTree.cpp"
#include "../LinkedRemainderList/LinkedRemainderList.cpp"
#include "../ArrayRemainderList/ArrayRemainderList.cpp"
#include "./TestLog.cpp"
using namespace std;
using namespace chrono;


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
    if (number * 11 > maxValue * 10)
    {
        throw std::invalid_argument("maxValue too small, hard to generate so many random number");
        return randomSet;
    }
    while (randomSet.size() < number)
    {
        randomSet.insert(rd() % maxValue);
    }
    return randomSet;
}

template <class T>
void BRTreeTest(unordered_set<T> dataSet)
{
    string message = "--------------Red Black Tree--------------";
    TestLog::appendLog(message);

    typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : dataSet)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / dataSet.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    RBTree<T> *redBlackTree = new RBTree<T>();
    timerStart = clock::now();
    for (auto i : dataSet)
    {
        redBlackTree->insert(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        redBlackTree->search(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        redBlackTree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    delete redBlackTree;
}

template <class T>
void LinkedRemainderTreeTest(unordered_set<T> dataSet,unsigned int maxValue)
{
    string message = "--------------Linked Remainder Tree--------------";
    TestLog::appendLog(message);

    typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : dataSet)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / dataSet.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    LinkedRemainderTree<unsigned int> *tree = new LinkedRemainderTree<unsigned int>(maxValue);
    message = tree->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->insert(i, i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    delete tree;
}

template <class T>
void BTreeTest(unordered_set<T> dataSet)
{
    string message = "--------------B Tree--------------";
    TestLog::appendLog(message);

        typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : dataSet)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / dataSet.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    BTree *tree = new BTree(0);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->insert(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->search(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);
    delete tree;
}

template <class T>
void ArrayRemainderTreeTest(unordered_set<T> dataSet,unsigned int maxValue, unsigned short radix)
{
    string message = "--------------Array Remainder Tree--------------";
    TestLog::appendLog(message);

        typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : dataSet)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / dataSet.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    ArrayRemainderTree<unsigned int> *tree;
    if (radix > 1)
    {
        tree = new ArrayRemainderTree<unsigned int>(maxValue, radix);
    }
    else
    {
        tree = new ArrayRemainderTree<unsigned int>(maxValue);
    }
    message = tree->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->insert(i, i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    delete tree;
}

template <class T>
void LinkedRemainderListTest(unordered_set<T> dataSet,unsigned int maxValue)
{
    string message = "--------------Linked Remainder List--------------";
    TestLog::appendLog(message);

        typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : dataSet)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / dataSet.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    LinkedRemainderList<unsigned int> *skipList = new LinkedRemainderList<unsigned int>(maxValue);
    message = skipList->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        skipList->insert(i, i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        skipList->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        skipList->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    delete skipList;
}

template <class T>
void ArrayRemainderListTest(unordered_set<T> dataSet,unsigned int maxValue, unsigned short radix)
{
    string message = "--------------Array Remainder List--------------";
    TestLog::appendLog(message);

        typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    for (auto i : dataSet)
    {
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    auto getTime = spentTime;
    message = "get from set time:" + to_string(getTime) + " average:" + to_string((double)getTime / dataSet.size());
    TestLog::appendLog(message);

    PROCESS_MEMORY_COUNTERS pmc;
    int memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
        message = "memory take(before): " + to_string(memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    ArrayRemainderList<unsigned int> *list;
    if (radix > 1)
    {
        list = new ArrayRemainderList<unsigned int>(maxValue, radix);
    }
    else
    {
        list = new ArrayRemainderList<unsigned int>(maxValue);
    }
    message = list->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        list->insert(i, i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time: " + to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        list->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : dataSet)
    {
        list->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / dataSet.size());
    TestLog::appendLog(message);

    delete list;
}

int main()
{
    TestLog::appendLog("");
    TestLog::appendLog("--------------------------------------------------");
    TestLog::appendLog("-----------------new statistic--------------------");
    TestLog::appendLog("--------------------------------------------------");
    unsigned int size =     10000;
    unsigned int maxValue = 11000;
    unordered_set<unsigned int> dataSet = getRandom(size, maxValue);
    string message = "Data size: " + std::to_string(dataSet.size());
    TestLog::appendLog(message);

    BRTreeTest(dataSet);
    ArrayRemainderListTest(dataSet, maxValue, 100);
    ArrayRemainderTreeTest(dataSet, maxValue, 100);
    LinkedRemainderTreeTest(dataSet, maxValue);
    LinkedRemainderListTest(dataSet, maxValue);
    BTreeTest(dataSet);

    TestLog::printLog();
    TestLog::writeLogToFile();

    TestLog::appendLog("");
    TestLog::appendLog("--------------------------------------------------");
    TestLog::appendLog("-----------------new statistic--------------------");
    TestLog::appendLog("--------------------------------------------------");
    size =     1000000;
    maxValue = 1100000;
    dataSet = getRandom(size, maxValue);
    message = "Data size: " + std::to_string(dataSet.size());
    TestLog::appendLog(message);

    BRTreeTest(dataSet);
    ArrayRemainderListTest(dataSet, maxValue, 100);
    ArrayRemainderTreeTest(dataSet, maxValue, 100);
    LinkedRemainderTreeTest(dataSet, maxValue);
    LinkedRemainderListTest(dataSet, maxValue);
    BTreeTest(dataSet);

    TestLog::printLog();
    TestLog::writeLogToFile();

    TestLog::appendLog("");
    TestLog::appendLog("--------------------------------------------------");
    TestLog::appendLog("-----------------new statistic--------------------");
    TestLog::appendLog("--------------------------------------------------");
    size =     10000000;
    maxValue = 11000000;
    dataSet = getRandom(size, maxValue);
    message = "Data size: " + std::to_string(dataSet.size());
    TestLog::appendLog(message);

    BRTreeTest(dataSet);
    ArrayRemainderListTest(dataSet, maxValue, 100);
    ArrayRemainderTreeTest(dataSet, maxValue, 100);
    LinkedRemainderTreeTest(dataSet, maxValue);
    LinkedRemainderListTest(dataSet, maxValue);
    BTreeTest(dataSet);

    TestLog::printLog();
    TestLog::writeLogToFile();

    TestLog::appendLog("");
    TestLog::appendLog("--------------------------------------------------");
    TestLog::appendLog("-----------------new statistic--------------------");
    TestLog::appendLog("--------------------------------------------------");
    size =     100000000;
    maxValue = 110000000;
    dataSet = getRandom(size, maxValue);
    message = "Data size: " + std::to_string(dataSet.size());
    TestLog::appendLog(message);

    BRTreeTest(dataSet);
    ArrayRemainderListTest(dataSet, maxValue, 100);
    ArrayRemainderTreeTest(dataSet, maxValue, 100);
    LinkedRemainderTreeTest(dataSet, maxValue);
    LinkedRemainderListTest(dataSet, maxValue);
    BTreeTest(dataSet);

    TestLog::printLog();
    TestLog::writeLogToFile();

    return 0;
}
