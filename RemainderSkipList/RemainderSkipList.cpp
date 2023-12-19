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
struct RSLNode
{
public:
    unsigned int index;
    T element;
    RSLNode *next;
};

template <class T>
class RemainderSkipList
{
private:
    unsigned short radix; // array size
    unsigned int max;     // max value
    unsigned short level;
    void *root; // root array
    RSLNode<T> *head;


public:
    RemainderSkipList();
    RemainderSkipList(unsigned int);
    RemainderSkipList(unsigned int, unsigned short);
    ~RemainderSkipList();
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
    RSLNode<T>* findLeftNode(unsigned int);
    void* findLeftNode(unsigned short,void*,unsigned int,unsigned short[], unsigned int);
    void destroy(void *, unsigned short);

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
RemainderSkipList<T>::~RemainderSkipList(){
    destroy();
}

template <class T>
void RemainderSkipList<T>::destroy(){

    //iterately remove data node;
    RSLNode<T>* nodePointer = head;
    RSLNode<T>* nodePointer2 = nullptr;
    while(nodePointer != nullptr){
        nodePointer2 = nodePointer->next;
        free(nodePointer);
        nodePointer=nodePointer2;
    }
    destroy(root,0);
    root=nullptr;
    head = nullptr;
}

template <class T>
void RemainderSkipList<T>::destroy(void* pointer, unsigned short currentLevel){
    //clear index level
    if(currentLevel<level-2){
        for(int i =0;i<radix;i++){
            if(*(long*)(pointer+ i * sizeof(void *)) != 0){
                destroy(*(void**)(pointer+ i * sizeof(void *)),currentLevel+1);
            }
        }
        free(pointer);
    }
    if(currentLevel>= level-1){
        free(pointer);
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
string RemainderSkipList<T>::printInfo()
{
    string s = "max value: " + to_string(max)+" radix: " + to_string(radix)+" level: " + to_string(level);
    cout << s<< endl;
    return s;
}


template <class T>
void RemainderSkipList<T>::printMemory(void *pBuff, unsigned int nLen)
{
    if (NULL == pBuff || 0 == nLen)
    {
        return;
    }

    for (unsigned int i = 0; i < nLen; ++i)
    {
        cout<<hex<<"0x"<<(*(long long*)(pBuff+i*8))<<" ";
    }

    cout<<dec<<endl;

}

template <class T>
void *RemainderSkipList<T>::findLeftNode(unsigned short currentLevel, void *arrayPointer, unsigned int arraySequence, unsigned short indexArray[], unsigned int index)
{
    if (arrayPointer == nullptr)
    {
        return nullptr;
    }
    // reached the last index level
    if (currentLevel >= level-1)
    {
        RSLNode<T> *RSLNodePointer = (RSLNode<T> *)arrayPointer;
        //travel to left biggest
        while (RSLNodePointer->next != nullptr && RSLNodePointer->next->index<index)
        {
            RSLNodePointer = RSLNodePointer->next;
        }
        if (RSLNodePointer->index < index)
        {
            return RSLNodePointer;
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
            void *returnPointer =nullptr;
            if((arrayPointer + i * sizeof(void *))!=nullptr){
                returnPointer =findLeftNode(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, indexArray, index);
            }
            if (returnPointer != nullptr)
            {
                return returnPointer;
            }
        }
    }
    else
    {
        for (int i = radix-1; i >= 0; i--)
        {
            void *returnPointer =nullptr;
            if((arrayPointer + i * sizeof(void *))!=nullptr){
                returnPointer =findLeftNode(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), arraySequence * radix + i, indexArray, index);
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
 * @date 2023-11-27
 * @brief find an index's closest left Node
 * @return the pointer of left Node, return nullptr if not found
 ********************************************************/
template <class T>
RSLNode<T>* RemainderSkipList<T>::findLeftNode(unsigned int index){
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

    return (RSLNode<T>*)findLeftNode(0,root,0,indexArray,index);

}

template <class T>
void RemainderSkipList<T>::insert(unsigned int index, T element)
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
            pointer = allocatePointer + (unsigned int)s.top() * sizeof(void *);
            s.pop();
        }
        else
        {
            pointer = (*(void **)pointer + (int)s.top() * sizeof(void *));
            s.pop();
        }
    }

    //travel to the node position
    bool needFindLeft = true;
    RSLNode<T> *nodePointer = nullptr;
    if (*(long *)pointer != 0)
    {
        nodePointer = (*(RSLNode<T> **)pointer);

        while (nodePointer->next != nullptr && nodePointer->index < index)
        {
            pointer = &(nodePointer->next);
            nodePointer = nodePointer->next;
            needFindLeft = false;
        }
        if (nodePointer->index == index)
        {
            nodePointer->element = element;
            return;
        }
        if(nodePointer->index<index){
            pointer= &(nodePointer->next);
            needFindLeft = false;
        }
    }

    //alloc and insert the node
    RSLNode<T> *newPointer = (RSLNode<T> *)malloc(sizeof(RSLNode<T>));
    memset(newPointer, 0, sizeof(RSLNode<T>));
    newPointer->index = index;
    newPointer->element = element;

    if(nodePointer!=nullptr && nodePointer->index>index){
        newPointer->next = nodePointer;
    }
    memcpy_s(pointer, sizeof(void *), &newPointer, sizeof(void *));

    if (needFindLeft)
    {
        RSLNode<T> *leftRSLNode = findLeftNode(index);
        RSLNode<T> *rightRSLNode;
        if (leftRSLNode == nullptr)
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
                newPointer->next = leftRSLNode->next;
            }
            leftRSLNode->next = newPointer;
        }
    }
}

template <class T>
T RemainderSkipList<T>::get(unsigned int index){
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
    RSLNode<T> *nodePointer = (*(RSLNode<T>**)pointer);

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
void RemainderSkipList<T>::remove(unsigned int index){
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
            return;
        }else{
            pointer = (*(void **)pointer + (int)remainderStack.top() * sizeof(void *));
            remainderStack.pop();
        }
    }

    if(*(long *)pointer == 0){
        return;
    }
    RSLNode<T> *nodePointer = (*(RSLNode<T>**)pointer);

    //need find left
    if(nodePointer->index == index){
        RSLNode<T> *leftPointer = findLeftNode(index);
        if(leftPointer == nullptr){
            head = nodePointer->next;
        }
        else{
            leftPointer->next=nodePointer->next;
        }
        if(nodePointer->next!= nullptr && nodePointer->next->index <(index/radix*radix +radix)){
        memcpy_s(pointer, sizeof(void *), &(nodePointer->next), sizeof(void *));
        }
        else{
            memcpy_s(pointer, sizeof(void *), 0, sizeof(void *));
        }
        free(nodePointer);
        return;
    }

    while(nodePointer!= nullptr ){
        if(nodePointer->index == index){
            memcpy_s(pointer, sizeof(void *), &(nodePointer->next), sizeof(void *));
            free(nodePointer);
            // delete nodePointer;
            return;
        }
        else if(nodePointer->index>index){
            return;
        }
        else{
            pointer=&(nodePointer->next);
            nodePointer=nodePointer->next;
        }
    }
    return;
}

template <class T>
void RemainderSkipList<T>::printPath(unsigned int index)
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
    cout << "print path of index: " << index << "; remainder stack: ";
    stack s2 = s;
    while (!s2.empty())
    {
        cout << s2.top() << " ";
        s2.pop();
    }
    cout << endl;


    void *pointer = &root;
    cout<<"root: ";
    printMemory(pointer,1);
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
            printMemory(*(void **)pointer, radix);
            pointer = (*(void **)pointer + (int)s.top() * sizeof(void *));
            s.pop();
        }
    }

    if(*(long *)pointer == 0){
        cout << "---------end----------" << endl;
        return;
    }
    RSLNode<T> *nodePointer =(*(RSLNode<T>**)pointer);
    while (nodePointer != nullptr && nodePointer ->index <= index)
    {
            cout<<"node index: " <<nodePointer->index<<" element: "<<nodePointer->element<<" next: "<<nodePointer->next <<endl;
            nodePointer = nodePointer->next;
    }
    if(nodePointer != nullptr){
        cout<<"node index: " <<nodePointer->index<<" element: "<<nodePointer->element<<" next: "<<nodePointer->next <<endl;
    }
    cout << "---------end----------" << endl;
}

template <class T>
void RemainderSkipList<T>::printAllNodes(){
    if(head ==nullptr){
        cout<<"----------no RSLNode-----------"<<endl;
        return;
    }
    RSLNode<T>* RSLNode= head;
    cout<<"------iterate start-------"<<endl;
    while(RSLNode!=nullptr){
        cout<<"RSLNode index: "<<RSLNode->index<<" element: "<<RSLNode->element<<endl;
        RSLNode=RSLNode->next;
    }
    cout<<"--------iterate end-------"<<endl;
}