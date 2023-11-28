#include <iostream>
#include <chrono>
#include <unordered_set>
#include <random>
#include "../RedBlackTree/RBTree.cpp"
#include "../RemainderSkipList/RemainderSkipList.cpp"
#include "../RemainderTree/RemainderTree.cpp"
#include "./TestLog.cpp"
using namespace std;
using namespace chrono;


template <class T>
unordered_set<T> getRandom(T number, T maxValue){
        std::unordered_set<T> randomSet;
    if(number>maxValue){
        return randomSet;
    }

    if(number == maxValue){
        for(int i=0;i<number;i++){
            randomSet.insert(i);
        }
        return randomSet;
    }
    std::random_device rd;

    while(number > randomSet.size()){
        randomSet.insert(rd()%maxValue);
    }
    return randomSet;
}

template <class T>
void BRTreeTest(unordered_set<T> dataSet){
    RBTree<T>* redBlacTree=new RBTree<T>();
    typedef std::chrono::high_resolution_clock clock;

    auto timerStart = clock::now();
    for(auto i: dataSet){
        redBlacTree->insert(i);
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    string message = "Red-black tree insert time "+to_string(spentTime);
    TestLog::appendLog(message);

    timerStart = clock::now();
    for(auto i: dataSet){
        redBlacTree->search(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Red-black tree recursive search time "+to_string(spentTime);
    TestLog::appendLog(message);

    timerStart = clock::now();
    for(auto i: dataSet){
        redBlacTree->iterativeSearch(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Red-black treeiterative search time "+to_string(spentTime);
    TestLog::appendLog(message);

    timerStart = clock::now();
    for(auto i: dataSet){
        redBlacTree->remove(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Red-black tree remove time "+to_string(spentTime);
    TestLog::appendLog(message);


    redBlacTree->destroy();
}


template <class T>
void RemainderTreeTest(unordered_set<T> dataSet,unsigned int maxValue){
    RemainderTree<unsigned int>* tree = new RemainderTree<unsigned int>(maxValue);
    tree->printInfo();
    typedef std::chrono::high_resolution_clock clock;

    auto timerStart = clock::now();
        for(auto i: dataSet){
        tree->add(i,i);
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    string message = "Remainder Tree insert time: "+to_string(spentTime);
    TestLog::appendLog(message);

    timerStart = clock::now();
    for(auto i: dataSet){
        tree->get(i);
    }
    timerEnd = clock::now();
    spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    message = "Remainder Tree search time: "+std::to_string(spentTime);
    TestLog::appendLog(message);
}

int main(){
    unsigned int maxValue = 100000000;
    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(10000000, maxValue);
    string message = "Set size: "+std::to_string(randomSet.size());
    TestLog::appendLog(message);
    TestLog::printLog();

    BRTreeTest(randomSet);
    cout<<"---------------"<<endl;
    RemainderTreeTest(randomSet,maxValue);

    TestLog::printLog();
    TestLog::writeLogToFile();

    return 0;
}

