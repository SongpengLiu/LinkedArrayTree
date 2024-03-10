#include <iostream>
#include <chrono>
#include <unordered_set>
#include <set>
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
class Test{
    private:
    std::unordered_set<T> set;
    T size;
    T max;
    public:
    Test(T size, T max);
    void setRandomSet();
    void blackRedTreeTest();
    void linkedRemainderTreeTest(unsigned short radix);
    void BTreeTest();
    void arrayRemainderTreeTest(unsigned short radix);
    void linkedRemainderListTest(unsigned short radix);
    void arrayRemainderListTest(unsigned short radix);
    T getSetSize();
};

template <class T>
Test<T>::Test(T inputSize, T inputMax){
    size =inputSize;
    max = inputMax;
}

template <class T>
void Test<T>::setRandomSet()
{
    std::random_device rd;
    if (size > max)
    {
        throw std::invalid_argument("invalid index");
        return;
    }

    // size is small, generate directly
    if (size < 1000)
    {
        while (set.size() < size)
        {
            set.insert(rd() % max);
        }
        return;
    }

    // size is mall compared with max, generate directly
    if (max -size > max / 100)
    {
        while (set.size() < size)
        {
            set.insert(rd() % max);
        }
        return;
    }

    // size is big compared with max
    while (set.size() < size - max/100)
    {
        set.insert(rd() % max);
    }

    for (int i = 0; i < max; i++)
    {
        if (set.find(i) == set.end())
        {
            set.insert(i);
        }
        if (set.size() >= size)
        {
            break;
        }
    }
    return;
}

template <class T>
T Test<T>::getSetSize(){
    return set.size();
}

template <class T>
void Test<T>::blackRedTreeTest()
{
    string message = "--------------Red Black Tree--------------";
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

    RBTree<T> *redBlackTree = new RBTree<T>();
    timerStart = clock::now();
    for (auto i : set)
    {
        redBlackTree->insert(i, i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "insert time " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        message = "memory take(after): " + to_string(pmc.WorkingSetSize / 1024) + "KB; increace memory: " + to_string((pmc.WorkingSetSize / 1024) - memoryBefore) + "KB";
        TestLog::appendLog(message);
    }

    timerStart = clock::now();
    for (auto i : set)
    {
        redBlackTree->search(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "random search time " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i =0; i<set.size();i++)
    {
        redBlackTree->search(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        redBlackTree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time " + to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    delete redBlackTree;
}

template <class T>
void Test<T>::linkedRemainderTreeTest(unsigned short radix)
{
    string message = "--------------Linked Remainder Tree--------------";
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

    LinkedRemainderTree<unsigned int> *tree;
    if(radix>1){
        tree = new LinkedRemainderTree<unsigned int>(max,radix);
    }
    else{
     tree = new LinkedRemainderTree<unsigned int>(max);
    }
    message = tree->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        tree->insert(i, i);
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
        tree->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "random search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i =0 ;i< set.size();i++)
    {
        tree->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        tree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    delete tree;
}

template <class T>
void Test<T>::BTreeTest()
{
    string message = "--------------B Tree--------------";
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

    BTree *tree = new BTree(0);

    timerStart = clock::now();
    for (auto i : set)
    {
        tree->insert(i);
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
        tree->search(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "random search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i =0;i< set.size();i++)
    {
        tree->search(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        tree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);
    delete tree;
}

template <class T>
void Test<T>::arrayRemainderTreeTest(unsigned short radix)
{
    string message = "--------------Array Remainder Tree--------------";
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

    ArrayRemainderTree<unsigned int> *tree;
    if (radix > 1)
    {
        tree = new ArrayRemainderTree<unsigned int>(max, radix);
    }
    else
    {
        tree = new ArrayRemainderTree<unsigned int>(max);
    }
    message = tree->printInfo();
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        tree->insert(i, i);
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
        tree->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "random search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i =0 ;i< set.size();i++)
    {
        tree->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i : set)
    {
        tree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "remove time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    delete tree;
}

template <class T>
void Test<T>::linkedRemainderListTest(unsigned short radix)
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
    message = "random search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
    TestLog::appendLog(message);

    timerStart = clock::now();
    for (auto i =0;i< set.size();i++)
    {
        list->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
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
void Test<T>::arrayRemainderListTest(unsigned short radix)
{
    string message = "--------------Array Remainder List--------------";
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

    ArrayRemainderList<unsigned int> *list;
    if (radix > 1)
    {
        list = new ArrayRemainderList<unsigned int>(max, radix);
    }
    else
    {
        list = new ArrayRemainderList<unsigned int>(max);
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
    for (auto i =0;i< set.size();i++)
    {
        list->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    spentTime = spentTime - getTime;
    message = "ordered search time: " + std::to_string(spentTime) + " average:" + to_string((double)spentTime / set.size());
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

//Warning: because of the substance of process, regarding memeory statistic, only the first memeory summary is accurate in a execution.
int main()
{
    TestLog::appendLog("");
    TestLog::appendLog("--------------------------------------------------");
    TestLog::appendLog("-----------------new statistic--------------------");
    TestLog::appendLog("--------------------------------------------------");
    unsigned int size = 100;
    unsigned int max =  100;

    while (size<=100000000)
    {
    Test<unsigned int> test = Test<unsigned int>(size,max);
    test.setRandomSet();
    string message = "Data size: " + std::to_string(test.getSetSize());
    TestLog::appendLog(message);


    test.blackRedTreeTest();
    test.arrayRemainderTreeTest(0);
    test.arrayRemainderListTest(0);
    test.linkedRemainderTreeTest(0);
    test.linkedRemainderListTest(0);
    test.BTreeTest();
    TestLog::printLog();
    TestLog::writeLogToFile();
    size=size*10;
    max=max*10;
    }



    return 0;
}
