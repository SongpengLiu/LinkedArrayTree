#include <iomanip>
#include <iostream>
#include <cmath>
#include <string>
#include <cstring>
#include <stack>
#include <memory>
using namespace std;


#define POWER 4
#define RADIX pow(2,POWER);


template <class KeyType, class ValueType>
class SparseLinkedArrayTree
{
private:
    uint16_t radix; //array size
    uint16_t level;
    void *root; // root array
    void *head;
    uint64_t currentSize;
        void* iterator;
    int iteratorCount;


public:
    SparseLinkedArrayTree();
    // ~SparseLinkedArrayTree();
    string printStructureInfo();
    void printPath(const KeyType);
    void insert(const KeyType, const ValueType);
    void remove(const KeyType);
    ValueType* get(const KeyType);
    void destroy();
    void printAllData();
    uint64_t size();
        ValueType* nextValue();
    ValueType* begin();

private:
    void printMemory(void *, unsigned int);
    void *findLeft(const KeyType);
    void *findLeft(unsigned short, void *, uint8_t*, bool);
    void remove(const KeyType, uint8_t*, unsigned short, void*);
};

template <class KeyType, class ValueType>
SparseLinkedArrayTree<KeyType, ValueType>::SparseLinkedArrayTree()
{
    radix =(KeyType)RADIX;
    level=ceil(sizeof(KeyType)*8/POWER);
    currentSize =0;
    root = NULL;
    head = nullptr;
        iterator = &head;
    iteratorCount=-1;
}



// template <class KeyType, class ValueType>
// SparseLinkedArrayTree<KeyType, ValueType>::~SparseLinkedArrayTree()
// {
//     destroy();
// }

// template <class KeyType, class ValueType>
// void SparseLinkedArrayTree<KeyType, ValueType>::destroy()
// {
//     for(unsigned int i=0;i<max;i++){
//         remove(i);
//     }
//     root = nullptr;
//     head == nullptr;
// }

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print the info of the list
 * @return info
 ********************************************************/
template <class KeyType, class ValueType>
string SparseLinkedArrayTree<KeyType, ValueType>::printStructureInfo()
{
    string s = "Structure Information: max value: 2^" + to_string(sizeof(KeyType)*8) + ", radix: " + to_string(radix) + ", level: " + to_string(level)
                +", Key length (Bytes):" +to_string(sizeof(KeyType)) +", Value length (Bytes): "+to_string(sizeof(ValueType));
    cout << s << endl;
    return s;
}

template <class KeyType, class ValueType>
void SparseLinkedArrayTree<KeyType, ValueType>::printMemory(void *pBuff, unsigned int nLen)
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
template <class KeyType, class ValueType>
void *SparseLinkedArrayTree<KeyType, ValueType>::findLeft(const KeyType key)
{
    KeyType key2 = key;
    if (key2 >= radix)
    {
        key2 = key2 -(key2%radix) - 1; // make sure start find from the left array
    }
    else
    {
        return nullptr;
    }

    // uint8_t* keyArray = (uint8_t*)&key2;
    uint8_t keyArray[level];

    for(int i=0;i<level;i++){
        keyArray[i]=key2 &(radix-1);
        key2 = key2>>POWER;
    }

    return findLeft(0, root, keyArray, true);
}

template <class KeyType, class ValueType>
void *SparseLinkedArrayTree<KeyType, ValueType>::findLeft(unsigned short currentLevel, void *arrayPointer, uint8_t* keyArray, bool isOnPath)
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

    if (isOnPath)
    {
        for (int i = keyArray[level-1-currentLevel]; i >= 0; i--)
        {
            void *returnPointer = nullptr;
            if (*(long long*)(arrayPointer + i * sizeof(void *)) != 0)
            {
                returnPointer = findLeft(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), keyArray, isOnPath);
            }
            if (returnPointer != nullptr)
            {
                return returnPointer;
            }
            if(isOnPath){
                isOnPath =false;
            }
        }
    }
    else
    {
        for (int i = radix - 1; i >= 0; i--)
        {
            void *returnPointer = nullptr;
            if (*(long long*)(arrayPointer + i * sizeof(void *)) !=0)
            {
                returnPointer = findLeft(currentLevel + 1, *(void **)(arrayPointer + i * sizeof(void *)), keyArray, false);
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
template <class KeyType, class ValueType>
void SparseLinkedArrayTree<KeyType, ValueType>::insert(const KeyType key, const ValueType value)
{
    // uint8_t* remainders = (uint8_t*)&key; // mind that data in memeory is stored in a reversed order
    uint8_t remainders[level];
    KeyType key2 =key;
    for(int i=0;i<level;i++){
        remainders[i]=key2 &(radix-1);
        key2 = key2>>POWER;
    }
    void *pointer = &root;
    void *allocatePointer;

    //iterate through index levels, allocate memory if no index array exist.
    for (int i = 0; i <level-1; i++)
    {
        if (*(long long*)pointer == 0)
        {
            allocatePointer = malloc(radix * (sizeof(void *)));
            memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *));
            memset(allocatePointer, 0, radix * (sizeof(void *)));
            pointer = allocatePointer + (unsigned int)remainders[level-1-i] * sizeof(void *);
        }
        else
        {
            pointer = (*(void **)pointer + (unsigned int)remainders[level-1-i] * sizeof(void *));
        }
    }


    void *keyPointer = nullptr;
    if (*(long long *)pointer == 0)//no data array, allocate a new.
    {
        allocatePointer = malloc(sizeof(ValueType) * radix + sizeof(void *)); //allocate data array
        memcpy_s(pointer, sizeof(void *), &allocatePointer, sizeof(void *)); //copy the array pointer to index array
        memset(allocatePointer, 0, sizeof(ValueType) * radix + sizeof(void *)); //initialize the data array.
        pointer = allocatePointer + (unsigned int)remainders[0] * sizeof(ValueType)+ sizeof(void *); //get the pointer of the element

        // new allocated array needs to find left array
        void *leftPointer = findLeft(key);

        if (leftPointer == nullptr) //no left array, I'm the first
        {
            leftPointer = &head;
        }
        memcpy_s(allocatePointer, sizeof(void *), leftPointer, sizeof(void *));// copy the left array's right array pointer to me
        memcpy_s(leftPointer, sizeof(void *), &allocatePointer, sizeof(void *));// copy me to the left
    }
    else
    {
        pointer = (*(void **)pointer + (unsigned int)remainders[0] * sizeof(ValueType))+ sizeof(void *);
    }
    if(!*(ValueType *)pointer){
        currentSize++;
    }
    *(ValueType *)pointer = value;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief search the value by key
 * @return pointer of the value
 ********************************************************/
template <class KeyType, class ValueType>
ValueType* SparseLinkedArrayTree<KeyType, ValueType>::get(KeyType key)
{
    // uint8_t* remainders = (uint8_t*)&key; // mind that data in memeory is stored in a reversed order
        uint8_t remainders[level];
    KeyType key2 =key;
        for(int i=0;i<level;i++){
        remainders[i]=key2 &(radix-1);
        key2 = key2>>POWER;
    }
    void *pointer = &root;
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long long*)pointer == 0)
        {
            return nullptr;
        }
        else
        {
            pointer = (*(void **)pointer + (unsigned int)remainders[level-1-i] * sizeof(void *));
        }
    }

    if (*(long long*)pointer == 0)
    {
        return nullptr;
    }

    pointer = (*(void **)pointer +sizeof(void *)+ (unsigned int)remainders[0] * sizeof(ValueType));
    return (ValueType*)pointer;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief remove the element from the list based on the key
 * @return void
 ********************************************************/
template <class KeyType, class ValueType>
void SparseLinkedArrayTree<KeyType, ValueType>::remove(const KeyType key)
{
    // uint8_t* remainders = (uint8_t*)&key;
        uint8_t remainders[level];
    KeyType key2 =key;
        for(int i=0;i<level;i++){
        remainders[i]=key2 &(radix-1);
        key2 = key2>>POWER;
    }
    remove(key, remainders, 0, &root);
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief remove the element from the list, and then check the aray
 * @return void
 ********************************************************/
template <class KeyType, class ValueType>
void SparseLinkedArrayTree<KeyType, ValueType>::remove(const KeyType key, uint8_t* remainders, unsigned short currentLevel, void *pointer)
{
    if (pointer == nullptr || *(long long*)pointer == 0)
    {
        return;
    }
    if (currentLevel < level - 1) // index level
    {
        void *arrayPointer = pointer;
        pointer = (*(void **)pointer + (unsigned int) remainders[level-1-currentLevel] * sizeof(void *));
        if (*(long long*)pointer == 0)
        {
            return; // no such a key
        }
        else
        {
            remove(key, remainders, currentLevel + 1, pointer); // has valid pointer, travel to next level
        }
        pointer = *(void **)arrayPointer; // need to check if the index array could be freed
        for (int i = 0; i < radix; i++)
        {
            if (*(long long*)pointer != 0)
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
        pointer = (*(void **)pointer + sizeof(void *)+(unsigned int)remainders[0] * sizeof(ValueType));

        memset(pointer, 0, sizeof(ValueType));
        currentSize--;

        pointer = *(void **)arrayPointer+ sizeof(void *);

        // check if have other value;
        for (int i = 0; i < radix; i++)
        {
            if (*(ValueType *)(pointer + i* sizeof(ValueType)) != 0)
            {
                return; // return when there is other value;
            }
        }
        //end loop, all value is 0

        // find left array
        void *leftPointer = findLeft(key);

        if (leftPointer == nullptr)
        {
            leftPointer = &head;
        }

        memcpy_s(leftPointer, sizeof(void *), *(void **)arrayPointer, sizeof(void *));

        free(*(void **)arrayPointer); // no other value, free the data array.
        memset(arrayPointer, 0, sizeof(void *));
    }
}

template <class KeyType, class ValueType>
uint64_t SparseLinkedArrayTree<KeyType, ValueType>::size(){
    return currentSize;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print search path or an key
 * @return void
 ********************************************************/
template <class KeyType, class ValueType>
void SparseLinkedArrayTree<KeyType, ValueType>::printPath(const KeyType key)
{
    // uint8_t* remainders = (uint8_t*)&key;
        uint8_t remainders[level];
    KeyType key2 =key;
        for(int i=0;i<level;i++){
        remainders[i]=key2 &(radix-1);
        key2 = key2>>POWER;
    }
    cout << "---------trace start---------" << endl;
    cout << "print path of key: " << key << "; remainders (top-to-down): ";
    for(int i =level-1;i>=0;i--){
        cout<<(unsigned int)remainders[i]<<" ";
    }

    cout << endl;

    void *pointer = &root;
    cout << "root: ";
    printMemory(pointer, 1);
    for (int i = 0; i < level - 1; i++)
    {
        if (*(long long*)pointer == 0)
        {
            cout << "---------end----------" << endl;
            return;
        }
        else
        {
            cout << "level: " << i << " remainder index: " << remainders[level-1-i] << " address: " << *(void **)pointer << endl;
            printMemory(*(void **)pointer, radix);
            pointer = (*(void **)pointer + (unsigned int)remainders[level-1-i] * sizeof(void *));
        }
    }

    if (*(long long*)pointer == 0)
    {
        cout << "---------end----------" << endl;
        return;
    }

    pointer = *(void **)pointer;
    cout<< "me address: "<<pointer<<", next data array address: "<<*(void **)pointer<<endl;
    pointer = pointer+sizeof(void *);
    cout << "value:";
    for (int i = 0; i < radix; i++)
    {
        cout << " " << *(ValueType *)pointer;
        pointer = pointer + sizeof(ValueType);
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
template <class KeyType, class ValueType>
void SparseLinkedArrayTree<KeyType, ValueType>::printAllData()
{
    if (head == nullptr)
    {
        cout << "----------no data-----------" << endl;
        return;
    }
    void *arrayPointer = head;
    arrayPointer=arrayPointer;
    cout << "------iterate start-------" << endl;
    do
    {
        cout << "->";
        for (int i = 0; i < radix; i++)
        {
            if(*(ValueType *)(arrayPointer+ sizeof(void *) +i*sizeof(ValueType)) !=NULL)
                cout << " " << *(ValueType *)(arrayPointer+ sizeof(void *) +i*sizeof(ValueType));
        }
        arrayPointer = *(void **)arrayPointer;
        cout << endl;
    }
    while (arrayPointer != nullptr);
    cout << "--------iterate end-------" << endl;
}

template <class KeyType, class ValueType>
ValueType* SparseLinkedArrayTree<KeyType, ValueType>::begin(){
    iterator =&head;
    iteratorCount =-1;
    return nextValue();
}
template <class KeyType, class ValueType>
ValueType* SparseLinkedArrayTree<KeyType, ValueType>::nextValue(){
    if(iterator ==nullptr){
        return nullptr;
    }
    while(iterator !=nullptr){
        iteratorCount++;
    if(iteratorCount%radix ==0){
        iterator = *(void**)iterator;
        iteratorCount =0;
        if(iterator == nullptr){
            return nullptr;
        }
    }
    if(*(ValueType*)(iterator+sizeof(void*)+sizeof(ValueType)*iteratorCount) !=0){
        return (ValueType*)(iterator+sizeof(void*)+sizeof(ValueType)*iteratorCount);
    }
    }
    return nullptr;

}