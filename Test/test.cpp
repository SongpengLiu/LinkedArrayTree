#include <iostream>
#include <chrono>
#include <unordered_set>
#include <set>
#include <random>
#include <windows.h>
#include <psapi.h>
#include <bitset>
#include <map>
#include "./TestLog.cpp"
#include "../LinkedArrayTree/LinkedArrayTree.cpp"
#include "../LinkedArrayTree/SparseLinkedArrayTree.cpp"
#include "../LinkedArrayTree/RadixLinkedArrayTree.cpp"
#include "../LinkedArrayTree/UnlinkedArrayTree.cpp"
#include "../tlx/tlx/container/btree_map.hpp"
using namespace std;
using namespace chrono;

enum Algorithm
{
    LAT,
    RBT,
    BT,
    UAT
};
class Test
{
public:
    void dataIntensityTest(Algorithm type, u_int64 size);
    void dataSparseTest(Algorithm type, u_int64 size);
    void dataIntensityIteration(Algorithm type, u_int64 size);
    void dataSparseIteration(Algorithm type, u_int64 size);
    void dataSparseUnlinkedArrayTreeTest(u_int64 size);
    void dataSparseMemoryTest(Algorithm type, u_int64 size);
    void dataIntensityMemoryTest(Algorithm type, u_int64 size);
};

void Test::dataIntensityTest(Algorithm type, u_int64 size)
{
    string message = "--------------dataIntensityTest--------------";
    TestLog::appendLog(message);
    message = "--------------Data set size: " + to_string(size) + "--------------";
    TestLog::appendLog(message);
    LinkedArrayTree<uint64_t, uint64_t> lat;
    map<uint64_t, uint64_t> rbt;
    tlx::btree_map<uint64_t, uint64_t> bt;

    if (type == Algorithm::LAT)
    {
        message = "--------------Linked Array Tree--------------";
    }
    else if (type == Algorithm::RBT)
    {
        message = "--------------Red Black Tree--------------";
    }
    else
    {
        message = "--------------B+-Tree--------------";
    }
    TestLog::appendLog(message);

    typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            lat.insert(i, i);
        }
    }
    else if (type == Algorithm::RBT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            rbt.insert(make_pair(i, i));
        }
    }
    else
    {
        for (uint64_t i = 0; i < size; i++)
        {
            bt.insert(make_pair(i, i));
        }
    }
    auto timerEnd = clock::now();
    auto timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Average insert time:" + to_string((double)timerSpend / size);
    TestLog::appendLog(message);

    timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        for (uint64_t i = 0; i < size; i++)
        {

            lat.get(i);
        }
    }
    else if (type == Algorithm::RBT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            rbt.find(i);
        }
    }
    else
    {
        for (uint64_t i = 0; i < size; i++)
        {
            bt.find(i);
        }
    }
    timerEnd = clock::now();
    timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Average search time:" + to_string((double)timerSpend / size);
    TestLog::appendLog(message);

    timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            lat.remove(i);
        }
    }
    else if (type == Algorithm::RBT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            rbt.erase(i);
        }
    }
    else
    {
        for (uint64_t i = 0; i < size; i++)
        {
            bt.erase(i);
        }
    }
    timerEnd = clock::now();
    timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Average deletion time:" + to_string((double)timerSpend / size);
    TestLog::appendLog(message);
    message = "--------------End--------------";
    TestLog::appendLog(message);
}

void Test::dataSparseTest(Algorithm type, u_int64 size)
{
    string message = "--------------Data Sparse Test--------------";
    TestLog::appendLog(message);
    message = "--------------Data set size: " + to_string(size) + "--------------";
    TestLog::appendLog(message);
    SparseLinkedArrayTree<uint64_t, uint64_t> lat;
    map<uint64_t, uint64_t> rbt;
    tlx::btree_map<uint64_t, uint64_t> bt;
    UnlinkedArrayTree<uint64_t, uint64_t> uat;

    if (type == Algorithm::LAT)
    {
        message = "--------------Linked Array Tree--------------";
    }
    else if (type == Algorithm::RBT)
    {
        message = "--------------Red Black Tree--------------";
    }
    else if (type == Algorithm::BT)
    {
        message = "--------------B+-Tree--------------";
    }
    else if (type == Algorithm::UAT)
    {
        message = "--------------Unlinked Array Tree--------------";
    }
    TestLog::appendLog(message);
    random_device rd;
    u_int statisticSize = 100;
    uint64_t storedData[statisticSize];
    uint64_t i;
    for (u_int j = 0; j < statisticSize; j++)
    {
        storedData[j] = ((uint64_t)rd() << 32) | rd();
    }

    if (type == Algorithm::LAT)
    {
        while (lat.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            lat.insert(i, i);
        }
    }
    else if (type == Algorithm::RBT)
    {
        while (rbt.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            rbt.insert(make_pair(i, i));
        }
    }
    else if (type == Algorithm::BT)
    {
        while (bt.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            bt.insert(make_pair(i, i));
        }
    }
    else
    {
        while (uat.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            uat.insert(i, i);
        }
    }

    typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            lat.insert(storedData[j], storedData[j]);
        }
    }
    else if (type == Algorithm::RBT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            rbt.insert(make_pair(storedData[j], storedData[j]));
        }
    }
    else if (type == Algorithm::BT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            bt.insert(make_pair(storedData[j], storedData[j]));
        }
    }
    else
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            uat.insert(storedData[j], storedData[j]);
        }
    }
    auto timerEnd = clock::now();
    auto timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Average insert time:" + to_string((double)timerSpend / statisticSize);
    TestLog::appendLog(message);

    timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            lat.get(storedData[j]);
        }
    }
    else if (type == Algorithm::RBT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            rbt.find(storedData[j]);
        }
    }
    else if (type == Algorithm::BT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            bt.find(storedData[j]);
        }
    }
    else
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            uat.get(storedData[j]);
        }
    }
    timerEnd = clock::now();
    timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Average search time:" + to_string((double)timerSpend / statisticSize);
    TestLog::appendLog(message);

    timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            lat.remove(storedData[j]);
        }
    }
    else if (type == Algorithm::RBT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            rbt.erase(storedData[j]);
        }
    }
    else if (type == Algorithm::BT)
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            bt.erase(storedData[j]);
        }
    }
    else
    {
        for (u_int j = 0; j < statisticSize; j++)
        {
            uat.remove(storedData[j]);
        }
    }
    timerEnd = clock::now();
    timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Average deletion time:" + to_string((double)timerSpend / statisticSize);
    TestLog::appendLog(message);
    message = "--------------End--------------";
    TestLog::appendLog(message);
}

void Test::dataIntensityIteration(Algorithm type, u_int64 size)
{
    string message = "--------------Data Intensity Iteration Test--------------";
    TestLog::appendLog(message);
    message = "--------------Data set size: " + to_string(size) + "--------------";
    TestLog::appendLog(message);
    LinkedArrayTree<uint64_t, uint64_t> lat;
    map<uint64_t, uint64_t> rbt;
    tlx::btree_map<uint64_t, uint64_t> bt;
    UnlinkedArrayTree<uint64_t, uint64_t> uat;

    if (type == Algorithm::LAT)
    {
        message = "--------------Linked Array Tree--------------";
    }
    else if (type == Algorithm::RBT)
    {
        message = "--------------Red Black Tree--------------";
    }
    else if (type == Algorithm::BT)
    {
        message = "--------------B+-Tree--------------";
    }
    else
    {
        message = "--------------Unlinked Array Tree--------------";
    }
    TestLog::appendLog(message);

    if (type == Algorithm::LAT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            lat.insert(i, i);
        }
    }
    else if (type == Algorithm::RBT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            rbt.insert(make_pair(i, i));
        }
    }
    else if (type == Algorithm::BT)
    {
        for (uint64_t i = 0; i < size; i++)
        {
            bt.insert(make_pair(i, i));
        }
    }
    else
    {
        for (uint64_t i = 0; i < size; i++)
        {
            uat.insert(i, i);
        }
    }
    uint64_t count = 0;
    typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        lat.setBegin();
        uint64_t *iterater;
        while ((iterater = lat.nextValue()) != nullptr)
        {
            if (*iterater % 2 == 0)
            {
                count++; // how many even number
            }
        }
    }
    else if (type == Algorithm::RBT)
    {
        auto iterater = rbt.begin();
        while (iterater != rbt.end())
        {
            if (iterater->second % 2 == 0)
            {
                count++; // how many even number
            }
            iterater++;
        }
    }
    else if (type == Algorithm::BT)
    {
        auto iterater = bt.begin();
        while (iterater != bt.end())
        {
            if (iterater->second % 2 == 0)
            {
                count++; // how many even number
            }
            iterater++;
        }
    }
    else
    {
        uint64_t *iterater;
        iterater = uat.begin();
        while (iterater != nullptr)
        {
            if (*iterater % 2 == 0)
            {
                count++; // how many even number
            }
            iterater = uat.nextValue();
        }
    }
    auto timerEnd = clock::now();
    auto timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Iteration time:" + to_string((double)timerSpend);
    TestLog::appendLog(message);
}

void Test::dataSparseIteration(Algorithm type, u_int64 size)
{
    string message = "--------------Data Sparse Iteration Test--------------";
    TestLog::appendLog(message);
    message = "--------------Data set size: " + to_string(size) + "--------------";
    TestLog::appendLog(message);
    SparseLinkedArrayTree<uint64_t, uint64_t> lat;
    map<uint64_t, uint64_t> rbt;
    tlx::btree_map<uint64_t, uint64_t> bt;
    UnlinkedArrayTree<uint64_t, uint64_t> uat;

    if (type == Algorithm::LAT)
    {
        message = "--------------Linked Array Tree--------------";
    }
    else if (type == Algorithm::RBT)
    {
        message = "--------------Red Black Tree--------------";
    }
    else if (type == Algorithm::BT)
    {
        message = "--------------B+-Tree--------------";
    }
    else
    {
        message = "--------------Unlinked Array Tree--------------";
    }
    TestLog::appendLog(message);

    uint64_t i;
    random_device rd;

    if (type == Algorithm::LAT)
    {
        while (lat.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            lat.insert(i, i);
        }
    }
    else if (type == Algorithm::RBT)
    {
        while (rbt.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            rbt.insert(make_pair(i, i));
        }
    }
    else if (type == Algorithm::BT)
    {
        while (bt.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            bt.insert(make_pair(i, i));
        }
    }
    else
    {
        while (uat.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            uat.insert(i, i);
        }
    }
    uint64_t count = 0;
    typedef std::chrono::high_resolution_clock clock;
    auto timerStart = clock::now();
    if (type == Algorithm::LAT)
    {
        uint64_t *iterater;
        iterater = lat.begin();
        while (iterater != nullptr)
        {
            if (*iterater % 2 == 0)
            {
                count++; // how many even number
            }
            iterater = lat.nextValue();
        }
    }
    else if (type == Algorithm::RBT)
    {
        auto iterater = rbt.begin();
        while (iterater != rbt.end())
        {
            if (iterater->second % 2 == 0)
            {
                count++; // how many even number
            }
            iterater++;
        }
    }
    else if (type == Algorithm::BT)
    {
        auto iterater = bt.begin();
        while (iterater != bt.end())
        {
            if (iterater->second % 2 == 0)
            {
                count++; // how many even number
            }
            iterater++;
        }
    }
    else
    {
        uint64_t *iterater;
        iterater = uat.begin();
        while (iterater != nullptr)
        {
            if (*iterater % 2 == 0)
            {
                count++; // how many even number
            }
            iterater = uat.nextValue();
        }
    }
    auto timerEnd = clock::now();
    auto timerSpend = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Iteration time:" + to_string((double)timerSpend);
    TestLog::appendLog(message);
}

void Test::dataSparseMemoryTest(Algorithm type, u_int64 size)
{
    string message = "--------------Data Sparse Memory Test--------------";
    TestLog::appendLog(message);
    message = "--------------Data set size: " + to_string(size) + "--------------";
    TestLog::appendLog(message);

    uint64_t i;
    random_device rd;
    PROCESS_MEMORY_COUNTERS pmc;
    uint32_t memoryAfter;
    uint32_t memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
    }

    if (type == Algorithm::LAT)
    {
        message = "--------------Linked Array Tree--------------";
        SparseLinkedArrayTree<uint64_t, uint64_t> lat;
        while (lat.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            lat.insert(i, i);
        }
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            memoryAfter = pmc.WorkingSetSize / 1024;
        }
    }
    else if (type == Algorithm::RBT)
    {
        message = "--------------Red Black Tree--------------";
        map<uint64_t, uint64_t> rbt;
        while (rbt.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            rbt.insert(make_pair(i, i));
        }
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            memoryAfter = pmc.WorkingSetSize / 1024;
        }
    }
    else if (type == Algorithm::BT)
    {
        message = "--------------B+-Tree--------------";
        tlx::btree_map<uint64_t, uint64_t> bt;
        while (bt.size() < size)
        {
            i = ((uint64_t)rd() << 32) | rd();
            bt.insert(make_pair(i, i));
        }
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            memoryAfter = pmc.WorkingSetSize / 1024;
        }
    }

    TestLog::appendLog(message);
    message = "Increased memeory (KB):" + to_string(memoryAfter - memoryBefore);
    TestLog::appendLog(message);
}

void Test::dataIntensityMemoryTest(Algorithm type, u_int64 size)
{
    string message = "--------------Data Intensity Memory Test--------------";
    TestLog::appendLog(message);
    message = "--------------Data set size: " + to_string(size) + "--------------";
    TestLog::appendLog(message);

    uint64_t i;
    random_device rd;
    PROCESS_MEMORY_COUNTERS pmc;
    uint32_t memoryAfter;
    uint32_t memoryBefore;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        memoryBefore = pmc.WorkingSetSize / 1024;
    }

    if (type == Algorithm::LAT)
    {
        message = "--------------Linked Array Tree--------------";
        SparseLinkedArrayTree<uint64_t, uint64_t> lat;
        for (uint64_t i = 0; i < size; i++)
        {
            lat.insert(i, i);
        }
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            memoryAfter = pmc.WorkingSetSize / 1024;
        }
    }
    else if (type == Algorithm::RBT)
    {
        message = "--------------Red Black Tree--------------";
        map<uint64_t, uint64_t> rbt;
        for (uint64_t i = 0; i < size; i++)
        {
            rbt.insert(make_pair(i, i));
        }
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            memoryAfter = pmc.WorkingSetSize / 1024;
        }
    }
    else if (type == Algorithm::BT)
    {
        message = "--------------B+-Tree--------------";
        tlx::btree_map<uint64_t, uint64_t> bt;

        for (uint64_t i = 0; i < size; i++)
        {
            bt.insert(make_pair(i, i));
        }
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            memoryAfter = pmc.WorkingSetSize / 1024;
        }
    }

    TestLog::appendLog(message);
    message = "Increased memeory (KB):" + to_string(memoryAfter - memoryBefore);
    TestLog::appendLog(message);
}


int main()
{
    // random_device rd;
    // // uint64_t n= ((uint64_t)rd() << 32) | rd();
    // uint64_t n=(uint64_t)rd();
    // cout<<std::bitset<64>(n);
    Test test;
    // for(uint64_t i =100;i<=10000000;i=i*10){// data intensity test
    // test.dataIntensityTest(Algorithm::LAT,i);
    // test.dataIntensityTest(Algorithm::RBT,i);
    // test.dataIntensityTest(Algorithm::BT,i);
    // TestLog::printLog();
    // TestLog::writeLogToFile();
    // }

    // for (uint64_t i = 10000000; i <= 10000000; i = i * 10)
    // { // data sparse test
    //     test.dataSparseTest(Algorithm::LAT, i);
    //     test.dataSparseTest(Algorithm::RBT, i);
    //     test.dataSparseTest(Algorithm::BT, i);
    //     test.dataSparseTest(Algorithm::UAT, i);
    //     TestLog::printLog();
    //     TestLog::writeLogToFile();
    // }

    // for (uint64_t i = 100; i <= 10000000; i = i * 10)
    // { // data sparse test
    //     test.dataIntensityIteration(Algorithm::LAT, i);
    //     test.dataIntensityIteration(Algorithm::RBT, i);
    //     test.dataIntensityIteration(Algorithm::BT, i);
    //     test.dataIntensityIteration(Algorithm::UAT, i);
    //     TestLog::printLog();
    //     TestLog::writeLogToFile();
    // }

    // for (uint64_t i = 100; i <= 10000000; i = i * 10)
    // { // data sparse test
    //     test.dataSparseIteration(Algorithm::LAT, i);
    //     test.dataSparseIteration(Algorithm::RBT, i);
    //     test.dataSparseIteration(Algorithm::BT, i);
    //     test.dataSparseIteration(Algorithm::UAT, i);
    //     TestLog::printLog();
    //     TestLog::writeLogToFile();
    // }

    for (uint64_t i = 10000000; i <= 10000000; i = i * 10)
    { // data sparse test
        // test.dataSparseMemoryTest(Algorithm::BT, i);
        // test.dataSparseMemoryTest(Algorithm::RBT, i);
        test.dataSparseMemoryTest(Algorithm::LAT, i);


        TestLog::printLog();
        // TestLog::writeLogToFile();
    }
    //     for (uint64_t i = 1000000; i <= 1000000; i = i * 10)
    // { // data sparse test
    //     test.dataIntensityMemoryTest(Algorithm::BT, i);
    //     test.dataIntensityMemoryTest(Algorithm::RBT, i);
    //     test.dataIntensityMemoryTest(Algorithm::LAT, i);


    //     TestLog::printLog();
    //     // TestLog::writeLogToFile();
    // }
    return 0;
}