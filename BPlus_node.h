#ifndef BPLUS_NODE
#define BPLUS_NODE
#include "math.h"
#include "book.h"
enum NODE_TYPE{INTERNAL, LEAF};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION{LEFT, RIGHT};   // 兄弟结点方向：左兄弟结点、右兄弟结点
const int M = 7;                   // 阶
const int MINNUM_KEY = ceil( M /2.0);
const int MAXNUM_KEY =  M - 1;
class Node{
public:
    Node();
    NODE_TYPE getType() const {return m_Type;}
    void setType(NODE_TYPE type){m_Type = type;}
    int getKeyNum() const {return m_KeyNum;}
    void setKeyNum(int n){m_KeyNum = n;}
    int getKeyValue(int i) const {return m_KeyValues[i];}
    void setKeyValue(int i, int key){m_KeyValues[i] = key;}
    int getKeyIndex(int key)const;  // 找到键值在结点中存储的下标
    virtual void removeKey(int keyIndex, int childIndex)=0;  // 从结点中移除键值
    virtual void split(Node* parentNode, int childIndex)=0; // 分裂结点
    virtual void mergeChild(Node* parentNode, Node* childNode, int keyIndex,SIBLING_DIRECTION d)=0;  // 合并结点
    virtual void borrowFrom(Node* destNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d)=0; // 从兄弟结点中借一个键值
    virtual int getChildIndex(int key, int keyIndex)const=0;  // 根据键值获取孩子结点指针下标
protected:
    NODE_TYPE m_Type;
    int m_KeyNum;
    int m_KeyValues[MAXNUM_KEY+1];
};
// 内部结点
class InternalNode : public Node{
public:
    InternalNode();
    Node* getChild(int i) const {return m_Childs[i];}
    void setChild(int i, Node* child){ m_Childs[i] = child;}
    void insert(int keyIndex, int childIndex, int key, Node* childNode);
    void split(Node* parentNode, int childIndex);
    void mergeChild(Node* parentNode, Node* childNode, int keyIndex,SIBLING_DIRECTION d);
    void removeKey(int keyIndex, int childIndex);
    void borrowFrom(Node* destNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d);
    int getChildIndex(int key, int keyIndex)const;
private:
    Node* m_Childs[MAXNUM_KEY + 2];//这里数组多一个是为了暂时储存以进行分裂。
};
// 叶子结点
class LeafNode : public Node{
public:
    LeafNode();
    LeafNode* getLeftSibling() const {return m_LeftSibling;}
    void setLeftSibling(LeafNode* node){ m_LeftSibling = node;}
    LeafNode* getRightSibling() const {return m_RightSibling;}
    void setRightSibling(LeafNode* node){ m_RightSibling = node;}
    book* getData(int i) const {return m_Datas[i];}
    void setData(int i,  book* data){m_Datas[i] = data;}
    void insert(int key,  book* data);
    void split(Node* parentNode, int childIndex);
    void mergeChild(Node* parentNode, Node* childNode, int keyIndex,SIBLING_DIRECTION d);
    void removeKey(int keyIndex, int childIndex);
    void borrowFrom(Node* destNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d);
    int getChildIndex(int key, int keyIndex)const;
private:
    LeafNode* m_LeftSibling;
    LeafNode* m_RightSibling;
    book* m_Datas[MAXNUM_KEY];
};
Node::Node(){
    setType(LEAF);
    setKeyNum(0);
}
#endif