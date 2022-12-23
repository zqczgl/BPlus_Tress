#ifndef BPLUS_NODE
#define BPLUS_NODE
#include "math.h"
#include "book.h"
enum NODE_TYPE{INTERNAL, LEAF};        // ������ͣ��ڽ�㡢Ҷ�ӽ��
enum SIBLING_DIRECTION{LEFT, RIGHT};   // �ֵܽ�㷽�����ֵܽ�㡢���ֵܽ��
const int M = 7;                   // ��
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
    int getKeyIndex(int key)const;  // �ҵ���ֵ�ڽ���д洢���±�
    virtual void removeKey(int keyIndex, int childIndex)=0;  // �ӽ�����Ƴ���ֵ
    virtual void split(Node* parentNode, int childIndex)=0; // ���ѽ��
    virtual void mergeChild(Node* parentNode, Node* childNode, int keyIndex,SIBLING_DIRECTION d)=0;  // �ϲ����
    virtual void borrowFrom(Node* destNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d)=0; // ���ֵܽ���н�һ����ֵ
    virtual int getChildIndex(int key, int keyIndex)const=0;  // ���ݼ�ֵ��ȡ���ӽ��ָ���±�
protected:
    NODE_TYPE m_Type;
    int m_KeyNum;
    int m_KeyValues[MAXNUM_KEY+1];
};
// �ڲ����
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
    Node* m_Childs[MAXNUM_KEY + 2];//���������һ����Ϊ����ʱ�����Խ��з��ѡ�
};
// Ҷ�ӽ��
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