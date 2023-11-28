/**
 * C++ 语言: 二叉查找树
 *
 * @author skywang
 * @date 2013/11/07
 */

#include <iostream>
#include <chrono>
#include <set>
#include <random>
#include "../RedBlackTree/RBTree.cpp"
using namespace std;
using namespace chrono;

template <class T>
set<T> getRandom(T number, T maxValue){
    random_device rd;
    // uniform_int_distribution<T> distribute(0, maxValue);
    std::set<T> randomSet;
    while(number < randomSet.size()){
        cout<<rd()<<endl;
        // randomSet.insert((T)rd());
    }
}

int main()
{
    // int a[]= {10, 40, 30, 60, 90, 70, 20, 50, 80, 90};
    int a[100];
    int check_insert=0;    // "插入"动作的检测开关(0，关闭；1，打开)
    int check_remove=0;    // "删除"动作的检测开关(0，关闭；1，打开)
    int i;
    int ilen = (sizeof(a)) / (sizeof(a[0])) ;
    RBTree<int>* tree=new RBTree<int>();
    typedef std::chrono::high_resolution_clock clock;

    set<unsigned long long int> random = getRandom<unsigned long long int>(100, 100000);
    for(unsigned long long int i: random){
        cout<< " "<<i<<" " <<endl;
    }
    


    srand(time(0));
    for(i=0; i<ilen; i++){
        a[i] = rand()%1000000;
    }
    cout << "== 原始数据: ";
    for(i=0; i<ilen; i++)
        cout << a[i] <<" ";
    cout << endl;

    for(i=0; i<ilen; i++)
    {
        auto timerStart = clock::now();
        tree->insert(a[i]);
        // 设置check_insert=1,测试"添加函数"
        // if(check_insert)
        // {
        //     cout << "== 添加节点: " << a[i] << endl;
        //     cout << "== 树的详细信息: " << endl;
        //     tree->print();
        //     cout << endl;
        // }
        auto timerEnd = clock::now();
        auto insertTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
        // cout<<"insert time is "<<insertTime<<endl;
    }
    RBTNode<int> *result;
    
    
    for(i=0; i<ilen; i++)
    {
        auto timerStart = clock::now();
        result= tree->search(a[i]);
        // 设置check_insert=1,测试"添加函数"
        // if(check_insert)
        // {
        //     cout << "== 添加节点: " << a[i] << endl;
        //     cout << "== 树的详细信息: " << endl;
        //     tree->print();
        //     cout << endl;
        // }
        auto timerEnd = clock::now();
        auto insertTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
        // cout<<result->key<<" search time is "<<insertTime<<endl;
    }
        for(i=0; i<ilen; i++)
    {
        auto timerStart = clock::now();
        result= tree->iterativeSearch(a[i]);
        // 设置check_insert=1,测试"添加函数"
        // if(check_insert)
        // {
        //     cout << "== 添加节点: " << a[i] << endl;
        //     cout << "== 树的详细信息: " << endl;
        //     tree->print();
        //     cout << endl;
        // }
        auto timerEnd = clock::now();
        auto insertTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
        cout<<result->key<<" iterate search time is "<<insertTime<<endl;
    }
    // tree->insert(90);
    // cout << "== 前序遍历: ";
    // tree->preOrder();

    // cout << "\n== 中序遍历: ";
    // tree->inOrder();

    // cout << "\n== 后序遍历: ";
    // tree->postOrder();
    // cout << endl;

    // cout << "== 最小值: " << tree->minimum() << endl;
    // cout << "== 最大值: " << tree->maximum() << endl;
    // cout << "== 树的详细信息: " << endl;
    // tree->print();

    // 设置check_remove=1,测试"删除函数"
    // if(check_remove)
    // {
    //     for(i=0; i<ilen; i++)
    //     {
    //         tree->remove(a[i]);

    //         cout << "== 删除节点: " << a[i] << endl;
    //         cout << "== 树的详细信息: " << endl;
    //         tree->print();
    //         cout << endl;
    //     }
    // }

    // 销毁红黑树
    tree->destroy();

    return 0;
}


