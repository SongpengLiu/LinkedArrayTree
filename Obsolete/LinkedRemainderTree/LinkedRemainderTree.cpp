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
struct TreeNode
{
public:
    unsigned int key;
    T element;
    TreeNode *next;
};

template <class T>
class LinkedRemainderTree
{
private:
    unsigned short radix; // array size
    unsigned int max;     // max value
    unsigned short level; // totle level
    void *root;           // root array
    stack<unsigned short> remainderStack; //remainder stack

public:
    LinkedRemainderTree();
    LinkedRemainderTree(unsigned int);
    LinkedRemainderTree(unsigned int, unsigned short);
    ~LinkedRemainderTree();
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
LinkedRemainderTree<T>::LinkedRemainderTree()
{
    max = MAX;
    radix = findBestRadix(max);
    level = findLevel(max, radix);

    root = NULL;
}

template <class T>
LinkedRemainderTree<T>::LinkedRemainderTree(unsigned int maxValue)
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
LinkedRemainderTree<T>::LinkedRemainderTree(unsigned int maxValue, unsigned short inputRadix)
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
LinkedRemainderTree<T>::~LinkedRemainderTree()
{
    destroy();
}

template <class T>
void LinkedRemainderTree<T>::destroy()
{
    for(unsigned int i=0;i<max;i++){
        remove(i);
    }
    root = nullptr;
}

/********************************************************
 * @author Anonymous for Conference
 * @date 2023-12-22
 * @brief find the radix that can minimize average search time based on the max value
 * @return radix
 ********************************************************/
template <class T>
unsigned short LinkedRemainderTree<T>::findBestRadix(unsigned int maxValue)
{
    unsigned short radix = 0;
    int presentSearchTime = 0;
    int lastSearchTime = 0;
    for (int i = 2; i < maxValue; i++)
    {
        presentSearchTime = (int)ceil(log((float)maxValue) / log((float)i)) + (int)ceil((float)i / 2);
        if (radix == 0)
        {
            radix = i;
            lastSearchTime = presentSearchTime;
        }
        else if (presentSearchTime <= lastSearchTime)
        {
            radix = i;
            lastSearchTime = presentSearchTime;
        }
        else
        {
            break;
        }
    }
    return radix;
}

template <class T>
unsigned short LinkedRemainderTree<T>::findLevel(unsigned int inputMax, unsigned short inputRadix)
{
    return (unsigned short)(ceil(log(inputMax) / log(inputRadix)));
}

template <class T>
string LinkedRemainderTree<T>::printInfo()
{
    string s = "max value: " + to_string(max) + " radix: " + to_string(radix) + " level: " + to_string(level);
    cout << s << endl;
    return s;
}

template <class T>
void LinkedRemainderTree<T>::printMemory(void *pBuff, unsigned int nLen)
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

template <class T>
void LinkedRemainderTree<T>::insert(unsigned int key, T element)
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
            // allocatePointer= new void**[radix];
            memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
            memset(allocatePointer, 0, radix * (sizeof(void *)));
            pointer = allocatePointer + (unsigned int)remainderStack.top() * sizeof(void *);
            remainderStack.pop();
        }
        else
        {
            pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    TreeNode<T> *nodePointer = nullptr;
    if (*(long *)pointer != 0)
    {
        nodePointer = (*(TreeNode<T> **)pointer);
        while (nodePointer->next != nullptr && nodePointer->key < key)
        {
            pointer = &(nodePointer->next);
            nodePointer = nodePointer->next;
        }
        if (nodePointer->key == key)
        {
            nodePointer->element = element;
            return;
        }
        if (nodePointer->key < key)
        {
            pointer = &(nodePointer->next);
        }
    }

    TreeNode<T> *newPointer = (TreeNode<T> *)malloc(sizeof(TreeNode<T>));
    // TreeNode<T> *newPointer = new TreeNode<T>;
    memset(newPointer, 0, sizeof(TreeNode<T>));
    newPointer->key = key;
    newPointer->element = element;

    if (nodePointer != nullptr && nodePointer->key > key)
    {
        newPointer->next = nodePointer;
    }
    memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));
}

template <class T>
T LinkedRemainderTree<T>::get(unsigned int key)
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
            pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    if (*(long *)pointer == 0)
    {
        return 0;
    }
    TreeNode<T> *nodePointer = (*(TreeNode<T> **)pointer);

    while (nodePointer != nullptr)
    {
        if (nodePointer->key == key)
        {
            return nodePointer->element;
        }
        else if (nodePointer->key > key)
        {
            return NULL;
        }
        else
        {
            nodePointer = nodePointer->next;
        }
    }
    return NULL;
}

template <class T>
void LinkedRemainderTree<T>::remove(unsigned int key){
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
void LinkedRemainderTree<T>::remove(unsigned int key, unsigned short currentLevel, void *pointer)
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
        TreeNode<T> *nodePointer = (*(TreeNode<T> **)pointer);
        while (nodePointer != nullptr)
        {
            if (nodePointer->key == key)
            {
                memcpy_s(pointer, sizeof(void *), &(nodePointer->next), sizeof(void *));
                free(nodePointer);
                // delete nodePointer;
                return;
            }
            else if (nodePointer->key > key)
            {
                return;
            }
            else
            {
                pointer = &(nodePointer->next);
                nodePointer = nodePointer->next;
            }
        }
    }
}

// template <class T>
// void LinkedRemainderTree<T>::remove(unsigned int key)
// {
//     if (key < 0 || key >= max)
//     {
//         throw std::invalid_argument("invalid key");
//     }

//     while(!remainderStack.empty()){
//         remainderStack.pop();
//     }
//     unsigned int key2 = key;
//     for (int i = 0; i < level; i++)
//     {
//         remainderStack.push(key2 % radix);
//         key2 = key2 / radix;
//     }

//     void *pointer = &root;
//     for (int i = 0; i < level - 1; i++)
//     {
//         if (*(long *)pointer == 0)
//         {
//             return;
//         }
//         else
//         {
//             pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
//             remainderStack.pop();
//         }
//     }

//     if (*(long *)pointer == 0)
//     {
//         return;
//     }
//     TreeNode<T> *nodePointer = (*(TreeNode<T> **)pointer);

//     while (nodePointer != nullptr)
//     {
//         if (nodePointer->key == key)
//         {
//             memcpy_s(pointer, sizeof(void *), &(nodePointer->next), sizeof(void *));
//             free(nodePointer);
//             // delete nodePointer;
//             return;
//         }
//         else if (nodePointer->key > key)
//         {
//             return;
//         }
//         else
//         {
//             pointer = &(nodePointer->next);
//             nodePointer = nodePointer->next;
//         }
//     }
//     return;
// }

/********************************************************
 * @author Anonymous for Conference
 * @date 2023-12-22
 * @brief print search path of an key
 * @return void
 ********************************************************/
template <class T>
void LinkedRemainderTree<T>::printPath(unsigned int key)
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
    TreeNode<T> *nodePointer = (*(TreeNode<T> **)pointer);
    cout << "node key: " << nodePointer->key << " element: " << nodePointer->element << " next: " << nodePointer->next << endl;
    while (nodePointer->next != nullptr)
    {
        nodePointer = nodePointer->next;
        cout << "node key: " << nodePointer->key << " element: " << nodePointer->element << " next: " << nodePointer->next << endl;
    }
    cout << "---------end----------" << endl;
}
