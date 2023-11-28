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
class RemainderTree
{
private:
    unsigned short radix; // array size
    unsigned int max;     // max value
    unsigned short level;
    void *root; // root array

public:
    RemainderTree();
    RemainderTree(unsigned int);
    RemainderTree(unsigned int, unsigned short);
    // ~RemainderTree();
    void printInfo();
    void printPath(unsigned int);
    void add(unsigned int, T);
    void remove(unsigned int);
    T get(unsigned int);


protected:
    unsigned short findBestRadix(unsigned int);
    unsigned short findLevel(unsigned int, unsigned short);
    void printBuffer(void *, unsigned int);
};

template <class T>
RemainderTree<T>::RemainderTree()
{
        max = MAX;
        radix = findBestRadix(max);
        level = findLevel(max, radix);

        root = NULL;
}

template <class T>
RemainderTree<T>::RemainderTree(unsigned int maxValue)
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
RemainderTree<T>::RemainderTree(unsigned int maxValue, unsigned short inputRadix)
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
unsigned short RemainderTree<T>::findBestRadix(unsigned int maxValue)
{
    unsigned short radix=0;
    int presentSearchTime=0;
    int lastSearchTime=0;
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
unsigned short RemainderTree<T>::findLevel(unsigned int inputMax, unsigned short inputRadix)
{
    return (unsigned short)(ceil(log(inputMax + 1) / log(inputRadix)));
}

template <class T>
void RemainderTree<T>::printInfo()
{
    cout << "max value: " + to_string(max)
         << " radix: " + to_string(radix)
         << " level: " + to_string(level)
         << endl;
}


template <class T>
void RemainderTree<T>::printBuffer(void *pBuff, unsigned int nLen)
{
    if (NULL == pBuff || 0 == nLen)
    {
        return;
    }

    const int nBytePerLine = 16;
    unsigned char *p = (unsigned char *)pBuff;
    char szHex[3 * nBytePerLine + 1] = {0};

    for (unsigned int i = 0; i < nLen; ++i)
    {
        int idx = 3 * (i % nBytePerLine);
        if (0 == idx)
        {
            memset(szHex, 0, sizeof(szHex));
        }
#ifdef WIN32
        sprintf_s(&szHex[idx], 4, "%02x ", p[i]);
#else
        snprintf(&szHex[idx], 4, "%02x ", p[i]);
#endif

        if (0 == ((i + 1) % nBytePerLine))
        {
            printf("%s\n", szHex);
        }
    }

    if (0 != (nLen % nBytePerLine))
    {
        printf("%s\n", szHex);
    }
}

template <class T>
void RemainderTree<T>::add(unsigned int index, T element)
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
        if(nodePointer->index<index){
            pointer= &(nodePointer->next);
        }
        while (nodePointer->next != nullptr)
        {
            if(nodePointer->index<index){
            pointer= &(nodePointer->next);
            nodePointer = nodePointer->next;
            }
            else{
                break;
            }
        }
        if(nodePointer->index==index){
        nodePointer->element=element;
        return;
        }
        if(nodePointer->index<index){
            pointer= &(nodePointer->next);
        }
    }

    TreeNode<T> *newPointer = (TreeNode<T> *)malloc(sizeof(TreeNode<T>));
    memset(newPointer, 0, sizeof(TreeNode<T>));
    newPointer->index = index;
    newPointer->element = element;

    if(nodePointer!=nullptr && nodePointer->index>index){
        newPointer->next = nodePointer;
    }
    memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));
}

template <class T>
T RemainderTree<T>::get(unsigned int index){
    if(index<0 ||index >max){
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> remainderStack;
    unsigned int index2 = index;
    for (int i = 0; i < level; i++)
    {
        remainderStack.push(index2 % radix);
        index2 = index2 / radix;
    }

    void* pointer =&root;
    for(int i=0;i<level-1;i++){
        if (*(long *)pointer == 0){
            return 0;
        }else{
            pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }


    if(*(long *)pointer == 0){
        return 0;
    }
    TreeNode<T> *nodePointer = (*(TreeNode<T>**)pointer);

    while(nodePointer!= nullptr ){
        if(nodePointer->index == index){
            return nodePointer->element;
        }
        else if(nodePointer->index>index){
            return NULL;
        }
        else{
            nodePointer=nodePointer->next;
        }
    }
    return NULL;
}

template <class T>
void RemainderTree<T>::printPath(unsigned int index)
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

    cout<<"---------trace start---------"<<endl;
    cout << "print path of index: " << index << endl;
    stack s2 = s;
    cout << "remainder stack: ";
    while (!s2.empty())
    {
        cout << s2.top() << " ";
        s2.pop();
    }
    cout << endl;

    void *pointer = &root;
    cout<<"root: "<<endl;
    printBuffer(pointer,sizeof(void *));
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long *)pointer == 0)
        {
            cout << "---------end----------" << endl;
            return;
        }
        else
        {
            cout << "level: " << i << " index: " << s.top() <<" address: "<<*(void **)pointer<< endl;
            printBuffer(*(void **)pointer, sizeof(void *) * radix);
            pointer = (*(void **)pointer + (int)s.top() * sizeof(void *));
            s.pop();
        }
    }

    if(*(long *)pointer == 0){
        cout << "---------end----------" << endl;
        return;
    }
    TreeNode<T> *nodePointer =(*(TreeNode<T>**)pointer);
    if(*(long *)nodePointer == 0){
        cout << "---------end----------" << endl;
        return;
    }
    else{
        cout<<"node index: " <<nodePointer->index<<" element: "<<nodePointer->element<<" next: "<<nodePointer->next <<endl;
    }
    while (nodePointer->next != nullptr)
    {
            nodePointer = nodePointer->next;
            cout<<"node index: " <<nodePointer->index<<" element: "<<nodePointer->element<<" next: "<<nodePointer->next <<endl;
    }
    cout << "---------end----------" << endl;
}

