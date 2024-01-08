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
struct ListNode
{
public:
    unsigned int key;
    T element;
    ListNode *next;
};

template <class T>
class LinkedRemainderList
{
private:
    unsigned short radix; // array size
    unsigned int max;     // max value
    unsigned short level;
    void *root; // root array
    ListNode<T> *head;
    stack<unsigned short> remainderStack;

public:
    LinkedRemainderList();
    LinkedRemainderList(unsigned int);
    LinkedRemainderList(unsigned int, unsigned short);
    ~LinkedRemainderList();
    string printInfo();
    void printPath(unsigned int);
    void insert(unsigned int, T);
    void remove(unsigned int);
    T get(unsigned int);
    void printAllNodes();
    void destroy();

private:
    unsigned short findBestRadix(unsigned int);
    unsigned short findLevel(unsigned int, unsigned short);
    void printMemory(void *, unsigned int);
    ListNode<T> *findLeftNode(unsigned int);
    void *findLeftNode(unsigned short, void *, unsigned int, unsigned short[], unsigned int);
    void remove(unsigned int, unsigned short, void*);
};

template <class T>
LinkedRemainderList<T>::LinkedRemainderList()
{
    max = MAX;
    radix = findBestRadix(max);
    level = findLevel(max, radix);
    head = nullptr;
    root = NULL;
}

template <class T>
LinkedRemainderList<T>::LinkedRemainderList(unsigned int maxValue)
{
    if (maxValue <= 1)
    {
        throw new exception();
        return;
    }

    max = maxValue;
    radix = findBestRadix(maxValue);
    level = findLevel(max, radix);
    head = nullptr;
    root = NULL;
}

template <class T>
LinkedRemainderList<T>::LinkedRemainderList(unsigned int maxValue, unsigned short inputRadix)
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
        head = nullptr;
        root = NULL;
    }
}

template <class T>
LinkedRemainderList<T>::~LinkedRemainderList()
{
    destroy();
}

template <class T>
void LinkedRemainderList<T>::destroy()
{
    for(unsigned int i =0;i<max;i++){
        remove(i);
    }
    root = nullptr;
    head = nullptr;
}


/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief find the radix that can minimize average search time based on the max value
 * @return radix
 ********************************************************/
template <class T>
unsigned short LinkedRemainderList<T>::findBestRadix(unsigned int maxValue)
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
unsigned short LinkedRemainderList<T>::findLevel(unsigned int inputMax, unsigned short inputRadix)
{
    return (unsigned short)(ceil(log(inputMax) / log(inputRadix)));
}

template <class T>
string LinkedRemainderList<T>::printInfo()
{
    string s = "max value: " + to_string(max) + " radix: " + to_string(radix) + " level: " + to_string(level);
    cout << s << endl;
    return s;
}

template <class T>
void LinkedRemainderList<T>::printMemory(void *pBuff, unsigned int nLen)
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
void *LinkedRemainderList<T>::findLeftNode(unsigned short currentLevel, void *arrayPointer, unsigned int arraySequence, unsigned short keyArray[], unsigned int key)
{
    if (arrayPointer == nullptr)
    {
        return nullptr;
    }
    // reached the last key level
    if (currentLevel >= level - 1)
    {
        ListNode<T> *nodePointer = (ListNode<T> *)arrayPointer;
        // travel to left biggest
        while (nodePointer->next != nullptr && nodePointer->next->key <= key)
        {
            nodePointer = nodePointer->next;
        }
        if (nodePointer->key <= key)
        {
            return nodePointer;
        }
        else
        {
            return nullptr;
        }
    }

    // if not reached the last key level, will recursively search
    unsigned int keySequence = key;
    for (int i = currentLevel; i < level; i++)
    {
        keySequence = keySequence / radix;
    }
    if (currentLevel == 0 || arraySequence >= keySequence)
    {
        for (int i = keyArray[currentLevel]; i >= 0; i--)
        {
            void *returnPointer = nullptr;
            if ((arrayPointer + i * sizeof(void *)) != nullptr)
            {
                returnPointer = findLeftNode(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, keyArray, key);
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
                returnPointer = findLeftNode(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, keyArray, key);
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
 * @brief find the closest left node of an key
 * @return node pointer
 ********************************************************/
template <class T>
ListNode<T> *LinkedRemainderList<T>::findLeftNode(unsigned int key)
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

    return (ListNode<T> *)findLeftNode(0, root, 0, keyArray, key);
}

template <class T>
void LinkedRemainderList<T>::insert(unsigned int key, T element)
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

    // travel to the data level
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
            pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    // travel to the node position
    bool needFindLeft = true;
    ListNode<T> *nodePointer = nullptr;
    if (*(long *)pointer != 0)
    {
        nodePointer = (*(ListNode<T> **)pointer);

        while (nodePointer->next != nullptr && nodePointer->key < key)
        {
            pointer = &(nodePointer->next);
            nodePointer = nodePointer->next;
            needFindLeft = false;
        }
        if (nodePointer->key == key)
        {
            nodePointer->element = element;
            return;
        }
        if (nodePointer->key < key)
        {
            pointer = &(nodePointer->next);
            needFindLeft = false;
        }
    }

    // alloc and insert the node
    ListNode<T> *newPointer = (ListNode<T> *)malloc(sizeof(ListNode<T>));
    memset(newPointer, 0, sizeof(ListNode<T>));
    newPointer->key = key;
    newPointer->element = element;

    if (nodePointer != nullptr && nodePointer->key > key)
    {
        newPointer->next = nodePointer;
    }
    memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));

    if (needFindLeft)
    {
        ListNode<T> *leftNode = findLeftNode(key);
        ListNode<T> *rightNode;
        if (leftNode == nullptr)
        {
            if (newPointer->next == nullptr)
            {
                newPointer->next = head;
            }
            head = newPointer;
        }
        else
        {
            if (newPointer->next == nullptr)
            {
                newPointer->next = leftNode->next;
            }
            leftNode->next = newPointer;
        }
    }
}

template <class T>
T LinkedRemainderList<T>::get(unsigned int key)
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
    ListNode<T> *nodePointer = (*(ListNode<T> **)pointer);

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
void LinkedRemainderList<T>::remove(unsigned int key){
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

    remove(key, 0, &root);

}

template <class T>
void LinkedRemainderList<T>::remove(unsigned int key, unsigned short currentLevel, void *pointer)
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
            remove(key, currentLevel + 1, pointer);
        }
        pointer = *(void**)arrayPointer; //need to check if the index array could be freed
        for (int i = 0; i < radix; i++)
        {
            if (*(long *)pointer != 0)
            {
                return; // return when there is other value;
            }
            pointer = pointer + sizeof(long*);
        }
        free(*(void**)arrayPointer); // no valid pointer, free the index array
        memset(arrayPointer, 0, sizeof(void *)); // set pointer in above index array to nullptr
    }
    else //data level
    {
        ListNode<T> *nodePointer = (*(ListNode<T> **)pointer);

        // need find left
        if (nodePointer->key == key)
        {
            ListNode<T> *leftPointer = findLeftNode(key);
            if (leftPointer == nullptr)
            {
                head = nodePointer->next;
            }
            else
            {
                leftPointer->next = nodePointer->next;
            }
            if (nodePointer->next != nullptr && nodePointer->next->key < (key / radix * radix + radix))
            {
                memcpy_s(pointer, sizeof(void *), &(nodePointer->next), sizeof(void *));
            }
            else
            {
                memcpy_s(pointer, sizeof(void *), 0, sizeof(void *));
            }
            free(nodePointer);
            return;
        }

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

template <class T>
void LinkedRemainderList<T>::printPath(unsigned int key)
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
    ListNode<T> *nodePointer = (*(ListNode<T> **)pointer);
    while (nodePointer != nullptr && nodePointer->key < key/radix* radix+ radix)
    {
        cout << "node key: " << nodePointer->key << " element: " << nodePointer->element << " next: " << nodePointer->next << endl;
        nodePointer = nodePointer->next;
    }
    cout << "---------end----------" << endl;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief iterately print all nodes
 * @return void
 ********************************************************/
template <class T>
void LinkedRemainderList<T>::printAllNodes()
{
    if (head == nullptr)
    {
        cout << "----------no node-----------" << endl;
        return;
    }
    ListNode<T> *node = head;
    cout << "------iterate start-------" << endl;
    while (node != nullptr)
    {
        cout << "node key: " << node->key << " element: " << node->element << endl;
        node = node->next;
    }
    cout << "--------iterate end-------" << endl;
}