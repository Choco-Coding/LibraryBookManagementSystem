#pragma once
#include "LoanBookData.h"
#include "LoanBookHeap.h"

class SelectionTreeNode 
{
private:
    LoanBookData* pData;
    SelectionTreeNode* pLeft;
    SelectionTreeNode* pRight;
    SelectionTreeNode* pParent;
    LoanBookHeap* hRoot;

public:
    SelectionTreeNode() {
        this->pData = NULL;
        this->pLeft = NULL;
        this->pRight = NULL;
        this->pParent = NULL;
        this->hRoot = NULL;
    }
    ~SelectionTreeNode() {

    }

    void setBookData(LoanBookData* data) { this->pData = data; }
    void setLeftChild(SelectionTreeNode* pL) { this->pLeft = pL; }
    void setRightChild(SelectionTreeNode* pR) { this->pRight = pR; }
    void setParent(SelectionTreeNode* pP) { this->pParent = pP; }
    void setHeap(LoanBookHeap* pHR) { this->hRoot = pHR; }

    // function to output heap data stored in LoanBookHeap according to book classification code
    LoanBookHeapNode* deepCopy(LoanBookHeapNode* root, LoanBookHeapNode* copyParent) {
        if (root == NULL) return NULL;

        LoanBookHeapNode* copy = new LoanBookHeapNode();
        copy->setBookData(root->getBookData());
        copy->setParent(copyParent);
        
        //recursive call
        copy->setLeftChild(deepCopy(root->getLeftChild(), copy));
        copy->setRightChild(deepCopy(root->getRightChild(), copy));

        return copy;
    } 

    LoanBookData* getBookData() { return pData; }
    SelectionTreeNode* getLeftChild() { return pLeft; }
    SelectionTreeNode* getRightChild() { return pRight; }
    SelectionTreeNode* getParent() { return pParent; }
    LoanBookHeap* getHeap() { return hRoot; }
};