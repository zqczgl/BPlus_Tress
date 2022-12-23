#include "BPlus_tree.h"
BPlusTree::BPlusTree(){
    Root = NULL;
    m_DataHead = NULL;
}
bool BPlusTree::insert(int key,  book* data){
    // 是否已经存在
    if (search(key))
    {
        return false;
    }
    // 找到可以插入的叶子结点，否则创建新的叶子结点
    if(Root == NULL)
    {
        Root = new LeafNode();
        m_DataHead = (LeafNode*)Root;
    }
    if (Root->getKeyNum() >= MAXNUM_KEY&&Root->getType()==LEAF) // 若根节点为叶节点且结点已满，分裂
    {
        InternalNode* newNode = new InternalNode();  //创建新的内部节点
        newNode->setChild(0, Root);
        Root->split(newNode, 0);    // 叶子结点分裂
        Root = newNode;  //更新根节点指针
    }
    recursive_insert(Root, key, data);
    if (Root->getKeyNum() > MAXNUM_KEY) // 若根节点为内部节点且结点已满，分裂
    {
        InternalNode* newNode = new InternalNode();  //创建新的内部节点
        newNode->setChild(0, Root);
        Root->split(newNode, 0);    // 叶子结点分裂
        Root = newNode;  //更新根节点指针
    }
    return true;
}

void BPlusTree::recursive_insert(Node* parentNode, int key, book* data)
{
    if (parentNode->getType()==LEAF)  // 叶子结点，直接插入
    {
        ((LeafNode*)parentNode)->insert(key, data);
    }
    else
    {
        // 找到子结点
        int childIndex = parentNode->getKeyIndex(key);
        Node* childNode = ((InternalNode*)parentNode)->getChild(childIndex);
        recursive_insert(childNode, key, data);
        if (childNode->getKeyNum()>MAXNUM_KEY)  // 子结点已满，需进行分裂
        {
            childNode->split(parentNode, childIndex);
        }
    }
}
bool BPlusTree::search(int key)
{
    return recursive_search(Root, key);
}

bool BPlusTree::recursive_search(Node *pNode, int key)const
{
    if (pNode==NULL)  //检测节点指针是否为空，或该节点是否为叶子节点
    {
        return false;
    }
    else
    {
        int keyIndex = pNode->getKeyIndex(key);
        //int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
        int childIndex =keyIndex;
        if (keyIndex<pNode->getKeyNum() && key==pNode->getKeyValue(keyIndex))
        {
            return true;
        }
        else
        {
            if (pNode->getType()==LEAF)   //检查该节点是否为叶子节点
            {
                return false;
            }
            else
            {
                return recursive_search(((InternalNode*)pNode)->getChild(childIndex), key);
            }
        }
    }
}

book* BPlusTree::find(int key) {
    return recursive_find(Root,key);
}
void BPlusTree::ascend()const
{
    LeafNode* itr = m_DataHead;
    while(itr!=NULL)
    {
        for (int i=0; i<itr->getKeyNum(); ++i)
        {
            cout<<*itr->getData(i)<<endl;
        }
        itr = itr->getRightSibling();
    }
}

bool BPlusTree::remove(int key)
{
    if (!search(key))  //不存在
    {
        return false;
    }
    if (Root->getKeyNum() == 1&&Root->getType() == LEAF)//就一个元素，清空
    {
        Root=NULL;
        m_DataHead=NULL;
        return true;
    }
    recursive_remove(Root, key);
    if(Root->getKeyNum()==0){
        Root=((InternalNode*)Root)->getChild(0);
    }
    return true;
}
void BPlusTree::recursive_remove(Node* parentNode, int key)
{
    if (key==169){
        key=169;
    }
    int keyIndex = parentNode->getKeyIndex(key);
    //二分修正
    int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (parentNode->getType()==LEAF)// 找到目标叶子节点
    {
        parentNode->removeKey(keyIndex, childIndex);  // 直接删除
        // 如果键值在内部结点中存在，也要相应的替换内部结点
        if (childIndex==0 && Root->getType() != LEAF && parentNode != m_DataHead)
        {
            changeKey(Root, key, parentNode->getKeyValue(0));
        }
    }
    else // 内结点
    {
        Node *pChildNode = ((InternalNode*)parentNode)->getChild(childIndex); //包含key的子树根节点
        recursive_remove(pChildNode, key);
        if (pChildNode->getKeyNum()==MINNUM_KEY-2)                       // 包含关键字达到下限值，进行相关操作
        {
            Node *pLeft = childIndex > 0 ? ((InternalNode*)parentNode)->getChild(childIndex - 1) : NULL;                       //左兄弟节点
            Node *pRight = childIndex < parentNode->getKeyNum() ? ((InternalNode*)parentNode)->getChild(childIndex + 1) : NULL;//右兄弟节点
            // 先考虑从兄弟结点中借
            if (pLeft!=NULL && pLeft->getKeyNum()>=MINNUM_KEY)// 左兄弟结点可借
            {
                pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
            }
            else if (pRight!=NULL && pRight->getKeyNum()>=MINNUM_KEY)//右兄弟结点可借
            {
                pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
            }
                //左右兄弟节点都不可借，考虑合并
            else if (pLeft!=NULL)                    //与左兄弟合并
            {
                pLeft->mergeChild(parentNode, pChildNode, childIndex-1,LEFT);
            }
            else if (pRight!=NULL)                   //与右兄弟合并
            {
                pChildNode->mergeChild(parentNode, pRight, childIndex,RIGHT);
            }
        }
    }
}
void BPlusTree::changeKey(Node *pNode, int oldKey, int newKey)
{
    if (pNode!=NULL && pNode->getType()!=LEAF)
    {
        int keyIndex = pNode->getKeyIndex(oldKey);
        if (keyIndex<pNode->getKeyNum() && oldKey==pNode->getKeyValue(keyIndex))  // 找到
        {
            pNode->setKeyValue(keyIndex, newKey);
        }
        else   // 继续找
        {
            changeKey(((InternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
        }
    }
}
book* BPlusTree::recursive_find(Node *pNode, int key) {
    int keyIndex = pNode->getKeyIndex(key);
    int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (pNode->getType()==LEAF)
    {
        return ((LeafNode*)pNode)->getData(keyIndex);
    }
    else
    {
        return recursive_find(((InternalNode*)pNode)->getChild(childIndex), key);
    }
}

void BPlusTree::show() {
    if(Root==NULL){return;}
    queue<Node*> q;
    q.push(Root);
    return recursive_show(q);
}

void BPlusTree::recursive_show(queue<Node *> q) {
    if(q.empty())
        return;
    int size=q.size();
    for (int i = 0; i < size; ++i) {
        Node* pNode=q.front();
        q.pop();
        if (pNode->getType()==LEAF)
        {   cout<<"(";
            for(int i=0;i<pNode->getKeyNum();i++){
                //if(i==0)cout<<"(";
                cout<<pNode->getKeyValue(i)<<" ";
                //if(i==pNode->getKeyNum()-1)cout<<")     ";
            }
            cout<<")     ";
        } else{
            cout<<"(";
            for(int i=0;i<pNode->getKeyNum();i++){
                //if(i==0)cout<<"(";
                cout<<pNode->getKeyValue(i)<<" ";
                //if(i==pNode->getKeyNum()-1)cout<<")     ";
            }
            cout<<")     ";
            for (int j = 0; j <= pNode->getKeyNum(); ++j) {
                q.push(((InternalNode*)pNode)->getChild(j));
            }
            cout<<"    ";
        }
    }
    cout<<endl;
    return recursive_show(q);
}