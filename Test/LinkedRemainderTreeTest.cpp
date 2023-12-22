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

void testConstructor(){
    LinkedRemainderTree<int>* tree = new LinkedRemainderTree<int>(7,2);
    tree->printInfo();
}

template <class T>
void testAdd(unordered_set<T> dataSet, unsigned int max)
{
    LinkedRemainderTree<string> *tree = new LinkedRemainderTree<string>(max);
    tree->printInfo();
    for (auto i : dataSet)
    {
        tree->insert(i, "value="+to_string(i));
    }
    cout<<"element:" <<*dataSet.begin()<<" "<<tree->get(*dataSet.begin())<<endl;
    tree->printPath(*dataSet.begin());
    tree->printPath(0);
    delete tree;
}

template <class T>
void testRemove(unordered_set<T> dataSet, unsigned int max)
{
    LinkedRemainderTree<string> *tree = new LinkedRemainderTree<string>(max);
    tree->printInfo();
    for (auto i : dataSet)
    {
        tree->insert(i, "value="+to_string(i));
    }
    tree->printPath(*dataSet.begin());
    tree->remove(*dataSet.begin());
    tree->printPath(*dataSet.begin());

    delete tree;
}

template <class T>
void testDestroy(unordered_set<T> dataSet, unsigned int max)
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        cout << "memory take(initial): " << pmc.WorkingSetSize / 1024 << "KB" << endl;
    }
    LinkedRemainderTree<unsigned int> *tree = new LinkedRemainderTree<unsigned int>(max);
    tree->printInfo();
    for (auto i : dataSet)
    {
        tree->insert(i, i + 1);
    }
    cout<<"element: "<<tree->get(0)<<endl;
    tree->printPath(0);
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        cout << "memory take(inserted): " << pmc.WorkingSetSize / 1024 << "KB" << endl;
    }
    delete tree;
    tree->printPath(0);
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        cout << "memory take(destroied): " << pmc.WorkingSetSize / 1024 << "KB" << endl;
    }
}

int main(){
    unsigned int maxValue = 11000;
    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(10000, maxValue);
    string message = "Set size: "+std::to_string(randomSet.size());
    cout<<message<<endl;
    // testAdd(randomSet,maxValue);
    testRemove(randomSet,maxValue);
    // testDestroy(randomSet,maxValue);
}