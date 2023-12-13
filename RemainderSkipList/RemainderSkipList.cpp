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
struct Node
{
public:
    unsigned int index;
    T element;
    Node *next;
};

template <class T>
class RemainderSkipList
{
private:
    unsigned short radix; // array size
    unsigned int max;     // max value
    unsigned short level;
    void *root; // root array
    Node<T> *head;


public:
    RemainderSkipList();
    RemainderSkipList(unsigned int);
    RemainderSkipList(unsigned int, unsigned short);
    // ~RemainderSkipList();
    void printInfo();
    void printPath(unsigned int);
    void add(unsigned int, T);
    void remove(unsigned int);
    T get(unsigned int);
    void printAllNodes();

protected:
    unsigned short findBestRadix(unsigned int);
    unsigned short findLevel(unsigned int, unsigned short);
    void printBuffer(void *, unsigned int);
    Node<T>* findLeftNode(unsigned int);
    void* findLeftNodeHelper(unsigned short,void*,unsigned int,unsigned short[], unsigned short);

};

template <class T>
RemainderSkipList<T>::RemainderSkipList()
{
        max = MAX;
        radix = findBestRadix(max);
        level = findLevel(max, radix);
        head = nullptr;
        root = NULL;
}

template <class T>
RemainderSkipList<T>::RemainderSkipList(unsigned int maxValue)
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
RemainderSkipList<T>::RemainderSkipList(unsigned int maxValue, unsigned short inputRadix)
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
unsigned short RemainderSkipList<T>::findBestRadix(unsigned int maxValue)
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
unsigned short RemainderSkipList<T>::findLevel(unsigned int inputMax, unsigned short inputRadix)
{
    return (unsigned short)(ceil(log(inputMax + 1) / log(inputRadix)));
}

template <class T>
void RemainderSkipList<T>::printInfo()
{
    cout << "max value: " + to_string(max)
         << " radix: " + to_string(radix)
         << " level: " + to_string(level)
         << endl;
}


template <class T>
void RemainderSkipList<T>::printBuffer(void *pBuff, unsigned int nLen)
{
    if (NULL == pBuff || 0 == nLen)
    {
        return;
    }

    const int nBytePerLine = 8;
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
        sprintf_s(&szHex[idx], 4, "%02x", p[i]);
#else
        snprintf(&szHex[idx], 4, "%02x", p[i]);
#endif

        if (0 == ((i + 1) % nBytePerLine))
        {
            printf("%s ", szHex);
        }
    }

    if (0 != (nLen % nBytePerLine))
    {
        printf("%s ", szHex);
    }
}

template <class T>
void *RemainderSkipList<T>::findLeftNodeHelper(unsigned short currentLevel, void *arrayPointer, unsigned int arraySequence, unsigned short indexArray[], unsigned short index)
{
    if (arrayPointer == nullptr || *(long *)arrayPointer == 0)
    {
        return nullptr;
    }
    // reached the last index level
    if (currentLevel >= level-1)
    {
        Node<T> *nodePointer = (Node<T> *)arrayPointer;
        //travel to left biggest
        while (nodePointer->next != nullptr && nodePointer->next->index<index)
        {
            nodePointer = nodePointer->next;
        }
        if (nodePointer->index < index)
        {
            return nodePointer;
        }
        else
        {
            return nullptr;
        }
    }

    // if not reached the last index level, will recursively search
    unsigned int indexSequence=index;
    for(int i=currentLevel;i<level;i++){
        indexSequence = indexSequence/radix;
    }
    if (currentLevel == 0 || arraySequence >= indexSequence)
    {
        for (int i = indexArray[currentLevel]; i >= 0; i--)
        {
            void *returnPointer = findLeftNodeHelper(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, indexArray, index);
            if (returnPointer != nullptr && *(long *)returnPointer != 0)
            {
                return returnPointer;
            }
        }
    }
    else
    {
        for (int i = radix-1; i >= 0; i--)
        {
            void *returnPointer = findLeftNodeHelper(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, indexArray, index);
            if (returnPointer != nullptr && *(long *)returnPointer != 0)
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
 * @date 2023-11-27
 * @brief find an index's left existing node
 * @return the pointer of left node, return nullptr if not found
 ********************************************************/
template <class T>
Node<T>* RemainderSkipList<T>::findLeftNode(unsigned int index){
    if(index<0 ||index >max){
        throw std::invalid_argument("invalid index");
    }

    unsigned short indexArray[level];
    unsigned int index2 =index;
    for (int i = 0; i < level; i++)
    {
        indexArray[level-i-1]=index2 % radix;
        index2 = index2 / radix;
    }

    return (Node<T>*)findLeftNodeHelper(0,root,0,indexArray,index);

}

template <class T>
void RemainderSkipList<T>::add(unsigned int index, T element)
{
    if (index < 0 || index > max)
    {
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> s;
    unsigned short index2 = index;
    for (int i = 0; i < level; i++)
    {
        s.push(index2 % radix);
        index2 = index2 / radix;
    }

    // Node<T>* node;
    void *pointer = &root;
    void *allocatePointer;
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long *)pointer == 0)
        {
            allocatePointer = malloc(radix * (sizeof(void *)));
            memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
            memset(allocatePointer, 0, radix * (sizeof(void *)));
            pointer = allocatePointer + (unsigned int)s.top() * sizeof(void *);
            s.pop();
        }
        else
        {
            pointer = (*(void **)pointer + (int)s.top() * sizeof(void *));
            s.pop();
        }
    }

    bool needFindLeft = true;
    Node<T> *nodePointer = nullptr;
    if (*(long *)pointer != 0)
    {
        nodePointer = (*(Node<T> **)pointer);
        if(nodePointer->index<index){
            pointer= &(nodePointer->next);
        }
        while (nodePointer->next != NULL && nodePointer->next->index <= index)
        {
            nodePointer = nodePointer->next;
            pointer= &(nodePointer->next);
        }
    }
    if(nodePointer!= nullptr && nodePointer->index == index ){
        nodePointer->element = element;
        return;
    }

    Node<T> *newPointer = (Node<T> *)malloc(sizeof(Node<T>));
    memset(newPointer, 0, sizeof(Node<T>));
    newPointer->index = index;
    newPointer->element = element;
    if(nodePointer==nullptr){
        memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));
    }
    else{
        if (pointer== &(nodePointer->next)){
            newPointer->next=nodePointer->next;
            memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));
        }
        else{
            newPointer->next=nodePointer;
            memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));
        }
    }


    if (nodePointer==nullptr || pointer!= &(nodePointer->next))
    {
        Node<T> *leftNode = findLeftNode(index);
        Node<T> *rightNode;
        if (leftNode == nullptr || *(long *)leftNode == NULL)
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
T RemainderSkipList<T>::get(unsigned int index){
    if(index<0 ||index >max){
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> s;
    unsigned short index2 = index;
    for (int i = 0; i < level; i++)
    {
        s.push(index2 % radix);
        index2 = index2 / radix;
    }

    void* pointer =&root;
    for(int i=0;i<level-1;i++){
        if (*(long *)pointer == 0){
            return 0;
        }else{
            pointer = (*(void **)pointer + (int)s.top() * sizeof(void *));
            s.pop();
        }
    }

    Node<T> *nodePointer = (*(Node<T>**)pointer);
    if(*(long *)nodePointer == 0){
        return 0;
    }

    while (nodePointer->index < index)
    {
        if (nodePointer->next != nullptr)
            nodePointer = nodePointer->next;
        else
            break;
    }
    if (nodePointer->index == index)
    {
        return nodePointer->element;
    }
    else
    {
        return 0;
    }
}

template <class T>
void RemainderSkipList<T>::printPath(unsigned int index)
{
    if (index < 0 || index > max)
    {
        throw std::invalid_argument("invalid index");
    }

    stack<unsigned short> s;
    unsigned short index2 = index;
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
    Node<T> *nodePointer =(*(Node<T>**)pointer);
    if(*(long *)nodePointer == 0){
        cout << "---------end----------" << endl;
        return;
    }
    else{
        cout<<"node index: " <<nodePointer->index<<" element: "<<nodePointer->element<<" next: "<<nodePointer->next <<endl;
    }
    while (nodePointer->index < index)
    {
        if (nodePointer->next != nullptr){
            nodePointer = nodePointer->next;
            cout<<"node index: " <<nodePointer->index<<" element: "<<nodePointer->element<<" next: "<<nodePointer->next <<endl;
        }
        else
            break;
    }
    cout << "---------end----------" << endl;
}

template <class T>
void RemainderSkipList<T>::printAllNodes(){
    if(head ==nullptr){
        cout<<"----------no node-----------"<<endl;
        return;
    }
    Node<T>* node=(Node<T>*)head;
    cout<<"------iterate start-------"<<endl;
    while(node!=nullptr){
        cout<<"node index: "<<node->index<<" element: "<<node->element<<endl;
        node=node->next;
    }
    cout<<"--------iterate end-------"<<endl;
}