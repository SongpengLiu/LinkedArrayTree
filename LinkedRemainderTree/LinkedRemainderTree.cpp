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
    unsigned int index;
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
    void destroy(void *, unsigned short);
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
    destroy(root, 0);
    root = nullptr;
}

template <class T>
void LinkedRemainderTree<T>::destroy(void *pointer, unsigned short currentLevel)
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
        if (((TreeNode<T> *)pointer)->next != nullptr)
        {
            destroy(((TreeNode<T> *)pointer)->next, currentLevel);
        }
        free(pointer);
    }
}

/********************************************************
 * @author Songpeng Liu
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
    return (unsigned short)(ceil(log(inputMax + 1) / log(inputRadix)));
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
void LinkedRemainderTree<T>::insert(unsigned int index, T element)
{
    if (index < 0 || index > max)
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
            // allocatePointer= new void**[radix];
            memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
            memset(allocatePointer, 0, radix * (sizeof(void *)));
            pointer = allocatePointer + (unsigned int)ramainderStack.top() * sizeof(void *);
            ramainderStack.pop();
        }
        else
        {
            pointer = (*(void **)pointer + (int)ramainderStack.top() * sizeof(void *));
            ramainderStack.pop();
        }
    }

    TreeNode<T> *nodePointer = nullptr;
    if (*(long *)pointer != 0)
    {
        nodePointer = (*(TreeNode<T> **)pointer);
        while (nodePointer->next != nullptr && nodePointer->index < index)
        {
            pointer = &(nodePointer->next);
            nodePointer = nodePointer->next;
        }
        if (nodePointer->index == index)
        {
            nodePointer->element = element;
            return;
        }
        if (nodePointer->index < index)
        {
            pointer = &(nodePointer->next);
        }
    }

    TreeNode<T> *newPointer = (TreeNode<T> *)malloc(sizeof(TreeNode<T>));
    // TreeNode<T> *newPointer = new TreeNode<T>;
    memset(newPointer, 0, sizeof(TreeNode<T>));
    newPointer->index = index;
    newPointer->element = element;

    if (nodePointer != nullptr && nodePointer->index > index)
    {
        newPointer->next = nodePointer;
    }
    memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));
}

template <class T>
T LinkedRemainderTree<T>::get(unsigned int index)
{
    if (index < 0 || index > max)
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
        if (nodePointer->index == index)
        {
            return nodePointer->element;
        }
        else if (nodePointer->index > index)
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
void LinkedRemainderTree<T>::remove(unsigned int index)
{
    if (index < 0 || index > max)
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
            pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    if (*(long *)pointer == 0)
    {
        return;
    }
    TreeNode<T> *nodePointer = (*(TreeNode<T> **)pointer);

    while (nodePointer != nullptr)
    {
        if (nodePointer->index == index)
        {
            memcpy_s(pointer, sizeof(void *), &(nodePointer->next), sizeof(void *));
            free(nodePointer);
            // delete nodePointer;
            return;
        }
        else if (nodePointer->index > index)
        {
            return;
        }
        else
        {
            pointer = &(nodePointer->next);
            nodePointer = nodePointer->next;
        }
    }
    return;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print search path of an index
 * @return void
 ********************************************************/
template <class T>
void LinkedRemainderTree<T>::printPath(unsigned int index)
{
    if (index < 0 || index > max)
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
    TreeNode<T> *nodePointer = (*(TreeNode<T> **)pointer);
    cout << "node index: " << nodePointer->index << " element: " << nodePointer->element << " next: " << nodePointer->next << endl;
    while (nodePointer->next != nullptr)
    {
        nodePointer = nodePointer->next;
        cout << "node index: " << nodePointer->index << " element: " << nodePointer->element << " next: " << nodePointer->next << endl;
    }
    cout << "---------end----------" << endl;
}
