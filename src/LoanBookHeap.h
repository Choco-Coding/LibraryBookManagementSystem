#pragma once
#include "LoanBookData.h"
#include "LoanBookHeapNode.h"
#include "_Stack.h"

class SelectionTreeNode;

class LoanBookHeap
{
private:
    LoanBookHeapNode* root;
    SelectionTreeNode* leaf;    //connected leaf node of selection tree
    Compare c;
    int componentNumber;        //number of nodes in heap

public:
    LoanBookHeap() {
        this->root = NULL;
        this->leaf = NULL;
        componentNumber = 0;
    };
    ~LoanBookHeap() {
        //free memory allocated with heap node
        if (root != NULL)
        {
            delTree(root);
        }
    }
    
    void setRoot(LoanBookHeapNode* pN) { this->root = pN; }
    LoanBookHeapNode* getRoot() { return root; }

    void heapifyUp(LoanBookHeapNode* pN);
    void heapifyDown(LoanBookHeapNode* pN);
    
    bool Insert(LoanBookData* data);    //Find the Last node position -> insert
    bool Delete();  //delete root and reinsert

    void setLeaf(SelectionTreeNode* pN) { this->leaf = pN; }
    SelectionTreeNode* getLeaf() { return leaf; }
    void setComponentNumber(int num) { this->componentNumber = num; }
    int getComponenetNumber() { return componentNumber; }

    void delTree(LoanBookHeapNode* p);			//recursive function that delete all nodes in heap called by destructor

};
