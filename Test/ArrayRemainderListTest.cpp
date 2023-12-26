#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include <unordered_set>
#include <windows.h>
#include <psapi.h>
#include "../ArrayRemainderList/ArrayRemainderList.cpp"
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

int main()
{
    int size = 20;
    int max = 22;

    unordered_set<unsigned int> randomSet = getRandom<unsigned int>(size, max);
    ArrayRemainderList<char *> *list = new ArrayRemainderList<char *>(max, 5);

    string s = "EEEE:" + to_string(*randomSet.begin());
    char c[10];
    strcpy(c, s.c_str());

    list->insert(*randomSet.begin(), c);

    list->printInfo();
    // list->printPath(*randomSet.begin());
    string cc = list->get(*randomSet.begin());
    cout << "eeeee: " << cc << endl;

    list->printAllData();
    return 0;
}