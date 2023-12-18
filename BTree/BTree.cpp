#include <iostream>
#include <vector>
#include <utility>
using namespace std;

class BTree {
private :
    struct Node {
        //int n; //the number of the keys in this node
        vector<int> key; //key.size() return n
        vector<Node*> pToChild; //the pointer to the children,p.empty() return isLeaf
        //bool isLeaf;
    };
    using PlaceOfChild = pair<Node*, int>;
private :
    Node * root;
    int t; //the minimum degree, more than or equal to 2
private :
    void SplitChild(Node * x, const int i);
    void Insert_notfull(Node * x, const int k);
    PlaceOfChild search(Node * p, const int k);
    PlaceOfChild predecessor(Node * x, int i);
    PlaceOfChild successor(Node * x, int i);
    Node * minimum(Node * p);
    Node * maximum(Node * p);
    void combine(Node * x, Node * y, PlaceOfChild z);
    void remove(Node * x, const int k);
    void inorderWalk(Node * p);
public :
    BTree(int deg) : root(new Node), t(deg >= 2 ? deg : 2) {}
    ~BTree() {delete root;}
    void insert(const int k);
    void remove(const int k) {remove(root, k);}
    bool search(const int k) {return (search(root, k).first ? true : false);}
    int minimum() {return minimum(root)->key[0];} //can't be empty
    int maximum() {return *(maximum(root)->key.end() - 1);} //can't be empty
    void inorderWalk() {inorderWalk(root);}
};

//if child is full and the parent is not full, split the child.
void BTree::SplitChild(Node * x, const int i) { //O(t)
    Node * y = x->pToChild[i];
    Node * z = new Node;

    for (int j = 0; j < t - 1; ++j) //right half side of key
        z->key.push_back(y->key[j+t]);

    if (!y->pToChild.empty()) {//y is not a leaf
        for (int j = 0; j < t; ++j) //right half side of pointer
            z->pToChild.push_back(y->pToChild[j+t]);
        for (int j = 0; j < t; ++j)
            y->pToChild.pop_back();
    }

    x->key.insert(x->key.begin() + i, y->key[t-1]);
    x->pToChild.insert(x->pToChild.begin() + i + 1, z);
    for (int j = 0; j < t; ++j)
        y->key.pop_back();
}

void BTree::Insert_notfull(Node * x, const int k) {
    int i = x->key.size() - 1;
    while (i >= 0 && k < x->key[i])  //find insertion place
            --i;
    if (x->pToChild.empty()) {
        x->key.insert(x->key.begin() + i + 1, k);
    }
    else {
        ++i;
        if (x->pToChild[i]->key.size() == 2 * t - 1) {
            SplitChild(x, i);
            if (k >= x->key[i])
                ++i;
        }
        Insert_notfull(x->pToChild[i], k);
    }
}

void BTree::insert(const int k) { //O(t*(logn to t))
    Node * r = root;
    if (r->key.size() == 2 * t - 1) { //root is full
        Node * s = new Node;
        root = s;
        s->pToChild.push_back(r);
        SplitChild(s, 0);
        Insert_notfull(s, k);
    }
    else
        Insert_notfull(r, k);
}

BTree::PlaceOfChild BTree::search(Node * p, const int k) {
    int i = 0;
    while (i < p->key.size() && k > p->key[i])
        ++i;
    if (i < p->key.size() && k == p->key[i])
        return make_pair(p, i);
    else if (p->pToChild.empty())
        return make_pair(nullptr, 0);
    else
        return search(p->pToChild[i], k);
}

BTree::Node * BTree::minimum(Node * p) {
    while (!p->pToChild.empty())
        p = p->pToChild[0];
    return p;
}

BTree::Node * BTree::maximum(Node * p) {
    while (!p->pToChild.empty())
        p = p->pToChild[p->pToChild.size()-1];
    return p;
}

BTree::PlaceOfChild BTree::predecessor(Node * x, int i) {
    if (!x->pToChild.empty()) {
        x = maximum(x->pToChild[i]);
        return make_pair(x, x->key.size() - 1);
    }
    else if (i != 0) {
        return make_pair(x, i - 1);
    }
    int key = x->key[i];
    Node * y = root;
    vector<PlaceOfChild> stk;
    while (1) {
        if (y->key[0] == key)
            break;
        for (i = 0; i < y->key.size() && key > y->key[i]; ++i)
            ;
        stk.push_back(make_pair(y, i));
        y = y->pToChild[i];
    }
    PlaceOfChild p;
    while (!stk.empty()) {
        p = stk.back();
        stk.pop_back();
        if (p.second != 0)
            return p;
    }
    return make_pair(nullptr, 0);
}

BTree::PlaceOfChild BTree::successor(Node * x, int i) {
    if (!x->pToChild.empty()) {
        x = minimum(x->pToChild[i+1]);
        return make_pair(x, 0);
    }
    else if (i != x->key.size() - 1) {
        return make_pair(x, i + 1);
    }
    int key = x->key[i];
    Node * y = root;
    vector<PlaceOfChild> stk;
    while (1) {
        if (y->key.back() == key)
            break;
        for (i = 0; i < y->key.size() && key > y->key[i]; ++i)
            ;
        stk.push_back(make_pair(y, i));
        y = y->pToChild[i];
    }
    PlaceOfChild p;
    while (!stk.empty()) {
        p = stk.back();
        stk.pop_back();
        if (p.second != p.first->key.size())
            return p;
    }
    return make_pair(nullptr, 0);
}

void BTree::combine(Node * x, Node  * y, PlaceOfChild z) {
    x->key.push_back(z.first->key[z.second]);
    for (int i = 0; i < t - 1; ++i)
        x->key.push_back(y->key[i]);
    if (!x->pToChild.empty())
        for (int i = 0; i < t; ++i) {
            x->pToChild.push_back(y->pToChild[i]);
        }
    delete y;

    z.first->key.erase(z.first->key.begin() + z.second);
    z.first->pToChild.erase(z.first->pToChild.begin() + z.second + 1);
    if (z.first->key.empty()) {
        root = z.first->pToChild[z.second];
        delete z.first;
    }
}

void BTree::remove(Node * x, const int k) { //This function guarantees x->key.size() >= t,except root
    int i = 0;
    while (i < x->key.size() && x->key[i] < k)
        ++i;
    if (i < x->key.size() && x->key[i] == k) {
        if (x->pToChild.empty())
            x->key.erase(x->key.begin() + i);
        else {
            if (x->pToChild[i]->key.size() >= t) {
                PlaceOfChild preOfk = predecessor(x, i);
                x->key[i] = preOfk.first->key[preOfk.second];
                remove(x->pToChild[i], x->key[i]); //recursive in the child ,not the successor
            }
            else if (x->pToChild[i+1]->key.size() >= t) {
                PlaceOfChild sucOfk = successor(x, i);
                x->key[i] = sucOfk.first->key[sucOfk.second];
                remove(x->pToChild[i+1], x->key[i]); //recursive in the child ,not the successor
            }
            else {
                combine(x->pToChild[i], x->pToChild[i+1], make_pair(x, i));
                remove(x->pToChild[i], k);
            }
        }
    }
    else {
        if (x->pToChild.empty())
            return ;
        else if (x->pToChild[i]->key.size() != t - 1)
            remove(x->pToChild[i], k);
        else {
            Node *y, *z;
            if (i > 0 && x->pToChild[i-1]->key.size() != t - 1) {
                y = x->pToChild[i-1];
                z = x->pToChild[i];
                z->key.insert(z->key.begin(), x->key[i-1]);
                if (!y->pToChild.empty()) {
                    z->pToChild.insert(z->pToChild.begin(), y->pToChild.back());
                    y->pToChild.pop_back();
                }
                x->key[i-1] = y->key.back();
                y->key.pop_back();
                remove(z, k);
            }
            else if (i < x->pToChild.size() - 1 && x->pToChild[i+1]->key.size() != t - 1){
                y = x->pToChild[i+1];
                z = x->pToChild[i];
                z->key.push_back(x->key[i]);
                if (!y->pToChild.empty()) {
                    z->pToChild.push_back(y->pToChild[0]);
                    y->pToChild.erase(y->pToChild.begin());
                }
                x->key[i] = y->key[0];
                y->key.erase(y->key.begin());
                remove(z, k);
            }
            else if (i > 0) {
                y = x->pToChild[i-1];
                z = x->pToChild[i];
                combine(y, z, make_pair(x, i-1));
                remove(y, k);
            }
            else if (i < x->pToChild.size() - 1) {
                y = x->pToChild[i];
                z = x->pToChild[i+1];
                combine(y, z, make_pair(x, i));
                remove(y, k);
            }
        }
    }
}

void BTree::inorderWalk(Node * p) {
    int i;
    if (!p->pToChild.empty()) {
        for (i = 0; i < p->key.size(); ++i) {
            inorderWalk(p->pToChild[i]);
            cout << p->key[i] << ' ';
        }
        inorderWalk(p->pToChild[i]);
    }
    else {
        for (i = 0; i < p->key.size(); ++i)
            cout << p->key[i] << ' ';
    }
}