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
    void destroy(void *, unsigned short);
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
    if (maxValue <= 1)
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
    if (maxValue <= 1 || inputRadix < 2 || inputRadix >= maxValue)
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
    destroy(root, 0);
    root = nullptr;
}

template <class T>
void ArrayRemainderTree<T>::destroy(void *pointer, unsigned short currentLevel)
{
    // index level
    if (currentLevel < level - 1)
    {
        for (int i = 0; i < radix; i++)
        {
            if (*(long *)(pointer + i * sizeof(void *)) != 0)
            {
                destroy(*(void **)(pointer + i * sizeof(void *)), currentLevel + 1);
            }
        }
        free(pointer);
    }

    // date level
    if (currentLevel >= level - 1)
    {
        free(pointer);
    }
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
    if (maxValue <= 100)
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
void ArrayRemainderTree<T>::insert(unsigned int index, T element)
{
    if (index < 0 || index >= max)
    {
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> ramainderStack;
    unsigned int index2 = index;
    for (int i = 0; i < level; i++)
    {
        ramainderStack.push(index2 % radix);
        index2 = index2 / radix;
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
            pointer = allocatePointer + (unsigned int)ramainderStack.top() * sizeof(void *);
            ramainderStack.pop();
        }
        else
        {
            pointer = (*(void **)pointer + (unsigned int)ramainderStack.top() * sizeof(void *));
            ramainderStack.pop();
        }
    }

    void *indexPointer = nullptr;
    if (*(long *)pointer == 0)
    {
        allocatePointer = malloc(sizeof(T) * radix);
        memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
        memset(allocatePointer, 0, sizeof(T) * radix);
        pointer = allocatePointer + (unsigned int)ramainderStack.top() * sizeof(T);
    }
    else
    {
        pointer = (*(void **)pointer + (unsigned int)ramainderStack.top() * sizeof(T));
    }
    ramainderStack.pop();
    *(T *)pointer = element;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief search an element in the tree based on the index
 * @return element
 ********************************************************/
template <class T>
T ArrayRemainderTree<T>::get(unsigned int index)
{
    if (index < 0 || index >= max)
    {
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> remainderStack;
    unsigned int index2 = index;
    for (int i = 0; i < level; i++)
    {
        remainderStack.push(index2 % radix);
        index2 = index2 / radix;
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
 * @brief remove an element from the tree based on the index
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderTree<T>::remove(unsigned int index)
{
    if (index < 0 || index >= max)
    {
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> remainderStack;
    unsigned int index2 = index;
    for (int i = 0; i < level; i++)
    {
        remainderStack.push(index2 % radix);
        index2 = index2 / radix;
    }

    void *pointer = &root;
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long *)pointer == 0)
        {
            return;
        }
        else
        {
            pointer = (*(void **)pointer + (unsigned int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    if (*(long *)pointer == 0)
    {
        return;
    }
    void *clearPointer = pointer;
    pointer = (*(void **)pointer + (unsigned int)remainderStack.top() * sizeof(T));
    memset(pointer, 0, sizeof(T));

    pointer = *(void **)clearPointer;

    // check if have other value;
    for (int i = 0; i < radix; i++)
    {
        if (*(T *)pointer != 0)
        {
            return; // return when there is other value;
        }
        pointer = pointer + sizeof(T);
    }
    free(*(void **)clearPointer); // no other value, free the data array.
    memset(clearPointer, 0, sizeof(void *));
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print search path of an index
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderTree<T>::printPath(unsigned int index)
{
    if (index < 0 || index >= max)
    {
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> s;
    unsigned int index2 = index;
    for (int i = 0; i < level; i++)
    {
        s.push(index2 % radix);
        index2 = index2 / radix;
    }

    cout << "---------trace start---------" << endl;
    cout << "print path of index: " << index << "; remainder stack: ";
    stack s2 = s;
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
            cout << "level: " << i << " index: " << s.top() << " address: " << *(void **)pointer << endl;
            printMemory(*(void **)pointer, radix);
            pointer = (*(void **)pointer + (int)s.top() * sizeof(void *));
            s.pop();
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
