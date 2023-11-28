#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include "../RemainderTree/RemainderTree.cpp"
using namespace std;

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

void testConstructor(){
    RemainderTree<int>* tree = new RemainderTree<int>(7,2);
    tree->printInfo();
}

template <class T>
void testAdd(unordered_set<T> dataSet, unsigned int max)
{
    typedef std::chrono::high_resolution_clock clock;


    RemainderTree<unsigned int> *tree = new RemainderTree<unsigned int>(max);
    tree->printInfo();
    auto timerStart = clock::now();
    for (auto i : dataSet)
    {
        tree->add(i, i + 1);
    }
    for (auto i : dataSet)
    {
        tree->printPath(i);
        cout<<"iiii:" <<i<<" "<<tree->get(i)<<endl;
        break;
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    cout<< "time 1: "<<spentTime;
    // tree->add(38764,38764);
    // tree->add(38761,38764);
    // tree->add(38762,38764);
    // tree->add(38765,38764);
    // tree->printPath(38764);

}

int main(){
    unsigned int maxValue = 200000;
    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(100000, maxValue);
    string message = "Set size: "+std::to_string(randomSet.size());
    cout<<message<<endl;
    testAdd(randomSet,maxValue);
}