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
class ArrayRemainderList
{
private:
    unsigned short radix; // array size
    unsigned int max;     // max value
    unsigned short level;
    void *root; // root array
    void *head;
    stack<unsigned short> remainderStack; //remainder stack

public:
    ArrayRemainderList();
    ArrayRemainderList(unsigned int);
    ArrayRemainderList(unsigned int, unsigned short);
    ~ArrayRemainderList();
    string printInfo();
    void printPath(unsigned int);
    void insert(unsigned int, T);
    void remove(unsigned int);
    T get(unsigned int);
    void destroy();
    void printAllData();

private:
    unsigned short findBestRadix(unsigned int);
    unsigned short findLevel(unsigned int, unsigned short);
    void printMemory(void *, unsigned int);
    void *findLeft(unsigned int);
    void *findLeft(unsigned short, void *, unsigned int, unsigned short[], unsigned int);
    void remove(unsigned int, unsigned short, void*);
};

template <class T>
ArrayRemainderList<T>::ArrayRemainderList()
{
    max = MAX;
    radix = findBestRadix(max);
    level = findLevel(max, radix);
    root = NULL;
    head = nullptr;
}

template <class T>
ArrayRemainderList<T>::ArrayRemainderList(unsigned int maxValue)
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
    head = nullptr;
}

template <class T>
ArrayRemainderList<T>::ArrayRemainderList(unsigned int maxValue, unsigned short inputRadix)
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
        head = nullptr;
    }
}

template <class T>
ArrayRemainderList<T>::~ArrayRemainderList()
{
    destroy();
}

template <class T>
void ArrayRemainderList<T>::destroy()
{
    for(unsigned int i=0;i<max;i++){
        remove(i);
    }
    root = nullptr;
    head == nullptr;
}


/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief find a radix for the array ramiander list
 * @return radix
 ********************************************************/
template <class T>
unsigned short ArrayRemainderList<T>::findBestRadix(unsigned int maxValue)
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
unsigned short ArrayRemainderList<T>::findLevel(unsigned int inputMax, unsigned short inputRadix)
{
    return (unsigned short)(ceil(log(inputMax) / log(inputRadix)));
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print the info of the list
 * @return info
 ********************************************************/
template <class T>
string ArrayRemainderList<T>::printInfo()
{
    string s = "max value: " + to_string(max) + " radix: " + to_string(radix) + " level: " + to_string(level);
    cout << s << endl;
    return s;
}

template <class T>
void ArrayRemainderList<T>::printMemory(void *pBuff, unsigned int nLen)
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
 * @brief find the left array of the input key
 * @return array pointer
 ********************************************************/
template <class T>
void *ArrayRemainderList<T>::findLeft(unsigned int key)
{
    if (key < 0 || key >= max)
    {
        throw std::invalid_argument("invalid key");
    }

    if (key >= radix)
    {
        key = key / radix*radix - 1; // make sure start find from the left array
    }
    else
    {
        return nullptr;
    }

    unsigned short keyArray[level];
    unsigned int key2 = key;
    for (int i = 0; i < level; i++)
    {
        keyArray[level - i - 1] = key2 % radix;
        key2 = key2 / radix;
    }

    return findLeft(0, root, 0, keyArray, key);
}

template <class T>
void *ArrayRemainderList<T>::findLeft(unsigned short currentLevel, void *arrayPointer, unsigned int arraySequence, unsigned short keyArray[], unsigned int key)
{
    if (arrayPointer == nullptr)
    {
        return nullptr;
    }

    // reached the last index level
    if (currentLevel >= level - 1)
    {
        return arrayPointer;
    }

    // if not reached the last index level, will recursively search
    unsigned int keySequence = key;
    for (int i = currentLevel; i < level; i++)
    {
        keySequence = keySequence / radix;
    }
    if (arraySequence >= keySequence)
    {
        for (int i = keyArray[currentLevel]; i >= 0; i--)
        {
            void *returnPointer = nullptr;
            if ((arrayPointer + i * sizeof(void *)) != nullptr)
            {
                returnPointer = findLeft(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, keyArray, key);
            }
            if (returnPointer != nullptr)
            {
                return returnPointer;
            }
        }
    }
    else
    {
        for (int i = radix - 1; i >= 0; i--)
        {
            void *returnPointer = nullptr;
            if ((arrayPointer + i * sizeof(void *)) != nullptr)
            {
                returnPointer = findLeft(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, keyArray, key);
            }
            if (returnPointer != nullptr)
            {
                return returnPointer;
            }
        }
    }

    // not find, return null
    return nullptr;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief insert an element to the list
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderList<T>::insert(unsigned int key, T element)
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
        allocatePointer = malloc(sizeof(T) * radix + sizeof(void *));
        memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
        memset(allocatePointer, 0, sizeof(T) * radix + sizeof(void *));
        pointer = allocatePointer + (unsigned int)remainderStack.top() * sizeof(T);

        // new allocated array needs to find left
        void *leftPointer = findLeft(key);


        if (leftPointer == nullptr)
        {
            leftPointer = &head;
        }
        else
        {
            leftPointer = leftPointer + radix * sizeof(T);
        }
        memcpy_s(allocatePointer + radix * sizeof(T), sizeof(void *), leftPointer, sizeof(void *));
        memcpy_s(leftPointer, sizeof(void *), &allocatePointer, sizeof(void *));
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
 * @brief search an element from the list by key
 * @return element
 ********************************************************/
template <class T>
T ArrayRemainderList<T>::get(unsigned int key)
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
 * @brief remove the element from the list based on the key
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderList<T>::remove(unsigned int key)
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

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief remove the element from the list, and then check the aray
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderList<T>::remove(unsigned int key, unsigned short currentLevel, void *pointer)
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
    else // data level
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

        // find left array
        void *leftPointer = findLeft(key);

        if (leftPointer == nullptr)
        {
            leftPointer = &head;
        }
        else
        {
            leftPointer = leftPointer + radix * sizeof(T);
        }

        memcpy_s(leftPointer, sizeof(void *), *(void **)arrayPointer + radix * sizeof(T), sizeof(void *));

        free(*(void **)arrayPointer); // no other value, free the data array.
        memset(arrayPointer, 0, sizeof(void *));
    }
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print search path or an key
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderList<T>::printPath(unsigned int key)
{
    if (key < 0 || key >= max)
    {
        throw std::invalid_argument("invalid key");
        return;
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

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief iterately print all data
 * @return void
 ********************************************************/
template <class T>
void ArrayRemainderList<T>::printAllData()
{
    if (head == nullptr)
    {
        cout << "----------no data-----------" << endl;
        return;
    }
    void *arrayPointer = head;
    cout << "------iterate start-------" << endl;
    while (arrayPointer != nullptr)
    {
        cout << "->";
        for (int i = 0; i < radix; i++)
        {
            cout << " " << *(T *)arrayPointer;
            arrayPointer = arrayPointer + sizeof(T);
        }
        arrayPointer = *(void **)arrayPointer;
        cout << endl;
    }
    cout << "--------iterate end-------" << endl;
}