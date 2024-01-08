#include <iomanip>
#include <iostream>
#include <cmath>
#include <string>
#include <cstring>
#include <stack>
#include <memory>
using namespace std;

#define MAX 10000000 // default max vaule
#define DEBUG true

template <class T>
class ArrayRemainderTree
{
private:
    unsigned short radix; // array size
    unsigned int max;     // max value
    unsigned short level;
    void *root; // root array
    stack<unsigned short> remainderStack; //remainder stack

public:
    ArrayRemainderTree();
    ArrayRemainderTree(unsigned int);
    ArrayRemainderTree(unsigned int, unsigned short);
    ~ArrayRemainderTree();
    string printInfo();
    void printPath(unsigned int);
    void insert(unsigned int, T);
    void remove(unsigned int);
    T get(unsigned int);
    void destroy();

private:
    unsigned short findBestRadix(unsigned int);
    unsigned short findLevel(unsigned int, unsigned short);
    void printMemory(void *, unsigned int);
    void remove(unsigned int, unsigned short, void*);
};

template <class T>
ArrayRemainderTree<T>::ArrayRemainderTree()
{
    max = MAX;
    radix = findBestRadix(max);
    level = findLevel(max, radix);

    root = NULL;
}

template <class T>
ArrayRemainderTree<T>::ArrayRemainderTree(unsigned int maxValue)
{
    if (maxValue <= 4)
    {
        throw new exception();
        return;
    }

    max = maxValue;
    radix = findBestRadix(maxValue);
    level = findLevel(max, radix);
    root = NULL;
}

template <class T>
ArrayRemainderTree<T>::ArrayRemainderTree(unsigned int maxValue, unsigned short inputRadix)
{
    if (maxValue <= 4 || inputRadix < 2 || inputRadix >= maxValue)
    {
        throw std::invalid_argument("wrong max value or radix");
        return;
    }
    else
    {
        max = maxValue;
        radix = inputRadix;
        level = findLevel(max, radix);
        root = NULL;
    }
}

template <class T>
ArrayRemainderTree<T>::~ArrayRemainderTree()
{
    destroy();
}

template <class T>
void ArrayRemainderTree<T>::destroy()
{
    for(unsigned int i=0;i<max;i++){
        remove(i);
    }
    root = nullptr;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief find a radix for the tree
 * @return radix
 ********************************************************/
template <class T>
unsigned short ArrayRemainderTree<T>::findBestRadix(unsigned int maxValue)
{
    if (maxValue <= 10){
        return maxValue -1;
    }
    else if (maxValue <= 100)
    {
        return 10;
    }
    else if(maxValue <=10000)
    {
        return 100;
    }
    else
    {
        return 1000;
    }
}

template <class T>
unsigned short ArrayRemainderTree<T>::findLevel(unsigned int inputMax, unsigned short inputRadix)
{
    return (unsigned short)(ceil(log(inputMax) / log(inputRadix)));
}

template <class T>
string ArrayRemainderTree<T>::printInfo()
{
    string s = "max value: " + to_string(max) + " radix: " + to_string(radix) + " level: " + to_string(level);
    cout << s << endl;
    return s;
}

template <class T>
void ArrayRemainderTree<T>::printMemory(void *pBuff, unsigned int nLen)
{
    if (NULL == pBuff || 0 == nLen)
    {
        return;
    }

    for (unsigned int i = 0; i < nLen; ++i)
    {
        cout << hex << "0x" << (*(long long *)(pBuff + i * 8)) << " ";
    }

    cout << dec << endl;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief insert an element to the tree
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderTree<T>::insert(unsigned int key, T element)
{
    if (key < 0 || key >= max)
    {
        throw std::invalid_argument("invalid key");
    }

    while(!remainderStack.empty()){
        remainderStack.pop();
    }
    unsigned int key2 = key;
    for (int i = 0; i < level; i++)
    {
        remainderStack.push(key2 % radix);
        key2 = key2 / radix;
    }

    // TreeNode<T>* node;
    void *pointer = &root;
    void *allocatePointer;
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long *)pointer == 0)
        {
            allocatePointer = malloc(radix * (sizeof(void *)));
            memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
            memset(allocatePointer, 0, radix * (sizeof(void *)));
            pointer = allocatePointer + (unsigned int)remainderStack.top() * sizeof(void *);
            remainderStack.pop();
        }
        else
        {
            pointer = (*(void **)pointer + (unsigned int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    void *keyPointer = nullptr;
    if (*(long *)pointer == 0)
    {
        allocatePointer = malloc(sizeof(T) * radix);
        memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
        memset(allocatePointer, 0, sizeof(T) * radix);
        pointer = allocatePointer + (unsigned int)remainderStack.top() * sizeof(T);
    }
    else
    {
        pointer = (*(void **)pointer + (unsigned int)remainderStack.top() * sizeof(T));
    }
    remainderStack.pop();
    *(T *)pointer = element;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief search an element in the tree based on the key
 * @return element
 ********************************************************/
template <class T>
T ArrayRemainderTree<T>::get(unsigned int key)
{
    if (key < 0 || key >= max)
    {
        throw std::invalid_argument("invalid key");
    }

    while(!remainderStack.empty()){
        remainderStack.pop();
    }
    unsigned int key2 = key;
    for (int i = 0; i < level; i++)
    {
        remainderStack.push(key2 % radix);
        key2 = key2 / radix;
    }

    void *pointer = &root;
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long *)pointer == 0)
        {
            return 0;
        }
        else
        {
            pointer = (*(void **)pointer + (unsigned int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    if (*(long *)pointer == 0)
    {
        return 0;
    }

    pointer = (*(void **)pointer + (unsigned int)remainderStack.top() * sizeof(T));
    return *(T *)pointer;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief remove an element from the tree based on the key
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderTree<T>::remove(unsigned int key)
{
    if (key < 0 || key >= max)
    {
        throw std::invalid_argument("invalid key");
    }

    while (!remainderStack.empty())
    {
        remainderStack.pop();
    }
    unsigned int key2 = key;
    for (int i = 0; i < level; i++)
    {
        remainderStack.push(key2 % radix);
        key2 = key2 / radix;
    }

    remove(key, 0, &root);
}
template <class T>
void ArrayRemainderTree<T>::remove(unsigned int key, unsigned short currentLevel, void *pointer)
{
    if (pointer == nullptr || *(long *)pointer == 0)
    {
        return;
    }
    if (currentLevel < level - 1) // index level
    {
        void *arrayPointer = pointer;
        pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
        remainderStack.pop();
        if (*(long *)pointer == 0)
        {
            return;
        }
        else
        {
            remove(key, currentLevel + 1, pointer); // has valid pointer, travel to next level
        }
        pointer = *(void **)arrayPointer; // need to check if the index array could be freed
        for (int i = 0; i < radix; i++)
        {
            if (*(long *)pointer != 0)
            {
                return; // return when there is other value;
            }
            pointer = pointer + sizeof(long *);
        }
        free(*(void **)arrayPointer);            // no valid pointer, free the index array
        memset(arrayPointer, 0, sizeof(void *)); // set pointer in above index array to nullptr
    }
    else //data level
    {
        void *arrayPointer = pointer;
        pointer = (*(void **)pointer + (unsigned int)remainderStack.top() * sizeof(T));
        remainderStack.pop();
        memset(pointer, 0, sizeof(T));

        pointer = *(void **)arrayPointer;

        // check if have other value;
        for (int i = 0; i < radix; i++)
        {
            if (*(T *)pointer != 0)
            {
                return; // return when there is other value;
            }
            pointer = pointer + sizeof(T);
        }
        free(*(void **)arrayPointer); // no other value, free the data array.
        memset(arrayPointer, 0, sizeof(void *));
    }
}


/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print search path of an key
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderTree<T>::printPath(unsigned int key)
{
    if (key < 0 || key >= max)
    {
        throw std::invalid_argument("invalid key");
    }

    while(!remainderStack.empty()){
        remainderStack.pop();
    }
    unsigned int key2 = key;
    for (int i = 0; i < level; i++)
    {
        remainderStack.push(key2 % radix);
        key2 = key2 / radix;
    }

    cout << "---------trace start---------" << endl;
    cout << "print path of key: " << key << "; remainder stack: ";
    stack s2 = remainderStack;
    while (!s2.empty())
    {
        cout << s2.top() << " ";
        s2.pop();
    }
    cout << endl;

    void *pointer = &root;
    cout << "root: ";
    printMemory(pointer, 1);
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long *)pointer == 0)
        {
            cout << "---------end----------" << endl;
            return;
        }
        else
        {
            cout << "level: " << i << " remainder index: " << remainderStack.top() << " address: " << *(void **)pointer << endl;
            printMemory(*(void **)pointer, radix);
            pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    if (*(long *)pointer == 0)
    {
        cout << "---------end----------" << endl;
        return;
    }
    pointer = *(void **)pointer;
    cout << "value:";
    for (int i = 0; i < radix; i++)
    {
        cout << " " << *(T *)pointer;
        pointer = pointer + sizeof(T);
    }
    cout << endl;
    cout << "---------end----------" << endl;
}
