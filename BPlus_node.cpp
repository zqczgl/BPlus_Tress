#include "BPlus_node.h"
int Node::getKeyIndex(int key) const
{
    if(key==11){
        key=11;
    }
    int low = 0;
    int mid = 0;
    int high = getKeyNum() - 1;
    while(low < high)
    {
        mid = (low + high) /2; // 中点值
        int midKey = getKeyValue(mid);
        if(key > midKey)
        {
            low = mid + 1;
        }
        else if(key < midKey)
        {
            high = mid -1 ;
        }
        else
        {
            return mid ;
        }
    }
    if(key <= getKeyValue(low))
    {
        return low;
    }
    else
    {
        return low + 1;
    }
}

// InternalNode
InternalNode::InternalNode(): Node(){
    setType(INTERNAL);
}
void InternalNode::split(Node* parentNode, int childIndex)
{
    InternalNode* newNode = new InternalNode();//分裂后的右节点
    newNode->setKeyNum(getKeyNum()-MINNUM_KEY);
    int i;
    for (i=0; i<getKeyNum()-MINNUM_KEY; ++i)// 拷贝关键字的值
    {
        newNode->setKeyValue(i, m_KeyValues[i+MINNUM_KEY]);
    }
    for (i=0; i<getKeyNum()-MINNUM_KEY+1; ++i) // 拷贝孩子节点指针
    {
        newNode->setChild(i, m_Childs[i+MINNUM_KEY]);
    }
    setKeyNum(MINNUM_KEY-1);  //更新左子树的关键字个数
    ((InternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MINNUM_KEY - 1], newNode);
}

void InternalNode::insert(int keyIndex, int childIndex, int key, Node* childNode){
    int i;
    for (i=getKeyNum(); i>keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
    {
        setChild(i+1,m_Childs[i]);
        setKeyValue(i,m_KeyValues[i-1]);
    }
    setChild(childIndex, childNode);
    setKeyValue(keyIndex, key);
    setKeyNum(m_KeyNum+1);
}

void InternalNode::mergeChild(Node* parentNode, Node* childNode, int keyIndex,SIBLING_DIRECTION d)
{
    // 合并数据
    if(d==LEFT){
        insert(MINNUM_KEY-1, MINNUM_KEY, parentNode->getKeyValue(keyIndex), ((InternalNode*)childNode)->getChild(0));
        int i;
        for (i=1; i<=childNode->getKeyNum(); ++i)
        {
            insert(MINNUM_KEY+i-1, MINNUM_KEY+i, childNode->getKeyValue(i-1), ((InternalNode*)childNode)->getChild(i));
        }
        //父节点删除index的key
        parentNode->removeKey(keyIndex, keyIndex+1);
    }else{
        insert(MINNUM_KEY-2, MINNUM_KEY-1, parentNode->getKeyValue(keyIndex), ((InternalNode*)childNode)->getChild(0));
        int i;
        for (i=1; i<=childNode->getKeyNum(); ++i)
        {
            insert(MINNUM_KEY+i-2, MINNUM_KEY+i-1, childNode->getKeyValue(i-1), ((InternalNode*)childNode)->getChild(i));
        }
        //父节点删除index的key
        parentNode->removeKey(keyIndex, keyIndex+1);
    }
}

void InternalNode::removeKey(int keyIndex, int childIndex)
{
    int i;
    for ( i=0; i<getKeyNum()-keyIndex-1; ++i)
    {
        setKeyValue(keyIndex+i, getKeyValue(keyIndex+i+1));
        setChild(childIndex+i, getChild(childIndex+i+1));
    }
    //i++;
    setChild(childIndex+i, getChild(childIndex+i+1));
    setKeyNum(getKeyNum()-1);
}

void InternalNode::borrowFrom(Node* siblingNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
    if(d==LEFT)  // 从左兄弟结点借
    {
        insert(0, 0, parentNode->getKeyValue(keyIndex), ((InternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
        parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum()-1));
        siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum());
    }
    else  // 从右兄弟结点借
    {
        insert(getKeyNum(), getKeyNum()+1, parentNode->getKeyValue(keyIndex), ((InternalNode*)siblingNode)->getChild(0));
        parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
        siblingNode->removeKey(0, 0);
    }
}

int InternalNode::getChildIndex(int key, int keyIndex)const
{
    if (keyIndex<getKeyNum()&&key==getKeyValue(keyIndex))
    {
        return keyIndex+1;
    }
    else
    {
        return keyIndex;
    }
}

// LeafNode
LeafNode::LeafNode(): Node(){
    setType(LEAF);
    setLeftSibling(NULL);
    setRightSibling(NULL);
}
void LeafNode::insert(int key, book* data)
{
    int i;
    for (i=m_KeyNum; i>=1 && m_KeyValues[i-1]>key; --i)
    {
        setKeyValue(i, m_KeyValues[i-1]);
        setData(i, m_Datas[i-1]);
    }
    setKeyValue(i, key);
    setData(i, data);
    setKeyNum(m_KeyNum+1);
}

void LeafNode::split(Node* parentNode, int childIndex)
{
    LeafNode* newNode = new LeafNode();//分裂后的右节点
    newNode->setKeyNum(getKeyNum()-MINNUM_KEY+1);
    setKeyNum(MINNUM_KEY-1);
    newNode->setRightSibling(getRightSibling());
    setRightSibling(newNode);
    newNode->setLeftSibling(this);
    int i;
    for (i=0; i<newNode->getKeyNum(); ++i)// 拷贝关键字的值
    {
        newNode->setKeyValue(i, m_KeyValues[i+MINNUM_KEY-1]);
    }
    for (i=0; i<newNode->getKeyNum(); ++i)// 拷贝数据
    {
        newNode->setData(i, m_Datas[i+MINNUM_KEY-1]);
    }
    ((InternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MINNUM_KEY - 1], newNode);
}

void LeafNode::mergeChild(Node* parentNode, Node* childNode, int keyIndex,SIBLING_DIRECTION d)
{
    // 合并数据
    for (int i=0; i<childNode->getKeyNum(); ++i)
    {
        insert(childNode->getKeyValue(i), ((LeafNode*)childNode)->getData(i));
    }
    setRightSibling(((LeafNode*)childNode)->getRightSibling());
    //父节点删除index的key，
    parentNode->removeKey(keyIndex, keyIndex+1);
}

void LeafNode::removeKey(int keyIndex, int childIndex)
{
    for (int i=keyIndex; i<getKeyNum()-1; ++i)
    {
        setKeyValue(i, getKeyValue(i+1));
        setData(i, getData(i+1));
    }
    setKeyNum(getKeyNum()-1);
}

void LeafNode::borrowFrom(Node* siblingNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
    switch(d)
    {
        case LEFT:  // 从左兄弟结点借
        {
            insert(siblingNode->getKeyValue(siblingNode->getKeyNum()-1), ((LeafNode*)siblingNode)->getData(siblingNode->getKeyNum() - 1));
            siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum()-1);
            parentNode->setKeyValue(keyIndex, getKeyValue(0));
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(siblingNode->getKeyValue(0), ((LeafNode*)siblingNode)->getData(0));
            siblingNode->removeKey(0, 0);
            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
        }
            break;
    }
}

int LeafNode::getChildIndex(int key, int keyIndex)const
{
    return keyIndex;
}