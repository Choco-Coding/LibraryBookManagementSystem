#pragma once
#include "SelectionTreeNode.h"

#include <fstream>
#include <iostream>
#include <string>

class SelectionTree
{
private:
    SelectionTreeNode* root;
    ofstream* fout;
    LoanBookHeap* run[8];   //run
    Compare c;
    int i;  //index of run

public:
    SelectionTree(ofstream* fout) {
        this->root = NULL;
        this->fout = fout;
        i = 0;

        //construct selection tree
        root = new SelectionTreeNode;
        construct(root, 1);
    }
    ~SelectionTree() {
        //free memory allocated with LoanBookHeap
        for (i = 0; i < 8; i++)
        {
            delete run[i];
        }

        //free memory allocated with selection tree node
        if (root != NULL)
        {
            delTree(root);
        }
    }

    void setRoot(SelectionTreeNode* pN) { this->root = pN; }
    SelectionTreeNode* getRoot() { return root; }
    LoanBookHeap* getRun(int index) //return run according to parameter(index / 100)
    {
        return run[index / 100];
    }  

    bool Insert(LoanBookData* newData);
    void SortStree(SelectionTreeNode* p);   //function to sort selection tree
    bool Delete();
    bool printBookData(int bookCode);
    
    void construct(SelectionTreeNode* p, int height);   //construct selection tree

    void delTree(SelectionTreeNode* p);			//recursive function that delete all nodes in Selection tree called by destructor

};