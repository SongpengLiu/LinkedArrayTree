#include <iomanip>
#include <iostream>
#include <cmath>
#include <string>
#include <cstring>
#include <stack>
#include <memory>
using namespace std;


#define RADIX pow(2,8);

template <class KeyType, class ValueType>
class LinkedArrayTree
{
private:
    uint16_t radix; //array size
    uint16_t level;
    void *root; // root array
    void *head;


public:
    LinkedArrayTree();
    // ~LinkedArrayTree();
    string printStructureInfo();
    void printPath(const KeyType);
    void insert(const KeyType, const ValueType);
    void remove(const KeyType);
    ValueType* get(const KeyType);
    void destroy();
    void printAllData();

private:
    void printMemory(void *, unsigned int);
    void *findLeft(const KeyType);
    void *findLeft(unsigned short, void *, unsigned int, uint8_t*, const KeyType);
    void remove(const KeyType, uint8_t*, unsigned short, void*);
};

template <class KeyType, class ValueType>
LinkedArrayTree<KeyType, ValueType>::LinkedArrayTree()
{
    radix =(KeyType)RADIX;
    level=sizeof(KeyType);
    root = NULL;
    head = nullptr;
}



// template <class KeyType, class ValueType>
// LinkedArrayTree<KeyType, ValueType>::~LinkedArrayTree()
// {
//     destroy();
// }

// template <class KeyType, class ValueType>
// void LinkedArrayTree<KeyType, ValueType>::destroy()
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
string LinkedArrayTree<KeyType, ValueType>::printStructureInfo()
{
    string s = "Structure Information: max value: 2^" + to_string(sizeof(KeyType)*8) + ", radix: " + to_string(radix) + ", level: " + to_string(level)
                +", Key length (Bytes):" +to_string(sizeof(KeyType)) +", Value length (Bytes): "+to_string(sizeof(ValueType));
    cout << s << endl;
    return s;
}

template <class KeyType, class ValueType>
void LinkedArrayTree<KeyType, ValueType>::printMemory(void *pBuff, unsigned int nLen)
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
void *LinkedArrayTree<KeyType, ValueType>::findLeft(const KeyType key)
{
    KeyType key2 = key;
    if (key2 >= radix)
    {
        key2 = key2 / radix*radix - 1; // make sure start find from the left array
    }
    else
    {
        return nullptr;
    }

    uint8_t* keyArray = (uint8_t*)&key2;

    return findLeft(0, root, 0, keyArray, key);
}

template <class KeyType, class ValueType>
void *LinkedArrayTree<KeyType, ValueType>::findLeft(unsigned short currentLevel, void *arrayPointer, unsigned int arraySequence, uint8_t* keyArray, const KeyType key)
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
    unsigned int keySequence =0;
    if(currentLevel>0){
        keySequence = keyArray[level-1-currentLevel+1]; //sequence of the index array where key is
    }

    if (arraySequence >= keySequence)
    {
        for (int i = keyArray[level-1-currentLevel]; i >= 0; i--)
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
template <class KeyType, class ValueType>
void LinkedArrayTree<KeyType, ValueType>::insert(const KeyType key, const ValueType value)
{
    uint8_t* remainders = (uint8_t*)&key; // mind that data in memeory is stored in a reversed order

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
    *(ValueType *)pointer = value;
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief search the value by key
 * @return pointer of the value
 ********************************************************/
template <class KeyType, class ValueType>
ValueType* LinkedArrayTree<KeyType, ValueType>::get(KeyType key)
{
    uint8_t* remainders = (uint8_t*)&key; // mind that data in memeory is stored in a reversed order
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
void LinkedArrayTree<KeyType, ValueType>::remove(const KeyType key)
{
    uint8_t* remainders = (uint8_t*)&key;
    remove(key, remainders, 0, &root);
}

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief remove the element from the list, and then check the aray
 * @return void
 ********************************************************/
template <class KeyType, class ValueType>
void LinkedArrayTree<KeyType, ValueType>::remove(const KeyType key, uint8_t* remainders, unsigned short currentLevel, void *pointer)
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

/********************************************************
 * @author Songpeng Liu
 * @date 2023-12-22
 * @brief print search path or an key
 * @return void
 ********************************************************/
template <class KeyType, class ValueType>
void LinkedArrayTree<KeyType, ValueType>::printPath(const KeyType key)
{
    uint8_t* remainders = (uint8_t*)&key;
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
void LinkedArrayTree<KeyType, ValueType>::printAllData()
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