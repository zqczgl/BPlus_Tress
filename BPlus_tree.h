#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H
#include "BPlus_node.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "queue"
using namespace std;
class BPlusTree{
public:
    BPlusTree();
    bool insert(int key, book* data);//���뺯��
    bool remove(int key);//����keyֵɾ��Ԫ��
    bool search(int key); // �����Ƿ����
    book* find(int key);
    void ascend()const;    // �������
    void show();
private:
    void recursive_insert(Node* parentNode, int key, book* data);
    void recursive_remove(Node* parentNode, int key);
    bool recursive_search(Node *pNode, int key)const;
    void changeKey(Node *pNode, int oldKey, int newKey);
    book* recursive_find(Node* pNode, int key);
    void recursive_show(queue<Node*> q);
private:
    Node* Root;
    LeafNode* m_DataHead;
};
#endif