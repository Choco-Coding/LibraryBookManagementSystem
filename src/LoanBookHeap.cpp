#include "LoanBookHeap.h"

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {
    if (pN == root)
    {
        return;
    }

    if (c.compareName(pN->getParent()->getBookData()->getName(), pN->getBookData()->getName()) <= 0)
    {
        //parent node's key is smaller than pN's key or equal to pN's key
        //finish sorting
        return;
    }
    else
    {
        //parent node's key is larger than pN's key
        //exchange parent node data and pN data
        LoanBookData* temp = pN->getParent()->getBookData();
        pN->getParent()->setBookData(pN->getBookData());
        pN->setBookData(temp);

        //move to parent node and recursive call
        heapifyUp(pN->getParent());
    }
}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {
    if (pN->getLeftChild() == NULL && pN->getRightChild() == NULL)
    {
        //leaf node
        return;
    }
    
    LoanBookHeapNode* smallerChild;     //The smaller of key of children nodes
    if (pN->getRightChild() == NULL)
    {
        smallerChild = pN->getLeftChild();
    }
    else
    {
        if (pN->getLeftChild() == NULL)
        {
            smallerChild = pN->getRightChild();
        }
        else
        {
             if (c.compareName(pN->getLeftChild()->getBookData()->getName(), pN->getRightChild()->getBookData()->getName()) <= 0)
            {
                //Left child node's key is smaller
                smallerChild = pN->getLeftChild();
            }
            else
            {
                //right child node's key is smaller
                smallerChild = pN->getRightChild();
            }
        }
    }
    
    if (c.compareName(pN->getBookData()->getName(), smallerChild->getBookData()->getName()) <= 0)
    {
        //pN's key is smaller than its child node's key or equal to its child node's key
        //finish sorting
        return;
    }
    else
    {
        //pN's key is larger than its child node's key
        //exchange pN data and its child node data
        LoanBookData* temp = pN->getBookData();
        pN->setBookData(smallerChild->getBookData());
        smallerChild->setBookData(temp);

        //move to child node and recursive call
        heapifyDown(smallerChild);
    }
}

bool LoanBookHeap::Insert(LoanBookData* data) {
    LoanBookHeapNode* pNew = new LoanBookHeapNode;
    pNew->setBookData(data);

    LoanBookData* temp = root->getBookData();

    //Find the position to be inserted new node (last position) and insert new node
    int num = componentNumber + 1;
    LoanBookHeapNode* pCur = root;
    _Stack* s = new _Stack;

    //Calculate direction
    while (true)
    {
        s->push(short(num % 2));
        num /= 2;

        if (num == 1)
        {
            break;
        }
    }

    //Find the next node position of last node
    while (s->getTop() != NULL)      //while not empty
    {
        if (s->pop() == 0)
        {
            if (pCur->getLeftChild() == NULL)
            {
                //insert new node
                pCur->setLeftChild(pNew);
                pNew->setParent(pCur);
                break;
            }
            else
            {
                //move to left child node
                pCur = pCur->getLeftChild();
            }
        }
        else
        {
            if (pCur->getRightChild() == NULL)
            {
                //insert new node
                pCur->setRightChild(pNew);
                pNew->setParent(pCur);
                break;
            }
            else
            {
                //move to right child node
                pCur = pCur->getRightChild();
            }
        }
    }

    componentNumber++;
    delete s;

    //bubbling up
    heapifyUp(pNew);

    if (temp != root->getBookData())   //root is changed -> replay selection tree
    {
        return true;
    }
    else
    {
        return false;
    }
}

//delete root and reinsert
bool LoanBookHeap::Delete()
{
    if (root->getLeftChild() == NULL && root->getRightChild() == NULL)
    {
        //root is last node
        delete root;
        root = NULL;
        componentNumber = 0;
        return false;
    }

    //remove root
    //Find the last node and reinsert into root
    int num = componentNumber;
    short direction;
    LoanBookHeapNode* pCur = root;
    _Stack* s = new _Stack;

    //Calculate direction
    while (true)
    {
        s->push(short(num % 2));
        num /= 2;

        if (num == 1)
        {
            break;
        }
    }

    //Find the next node position of last node
    while (s->getTop() != NULL)      //while not empty
    {
        direction = s->pop();
        if (direction == 0)
        {
            //move to left child node
            pCur = pCur->getLeftChild();
        }
        else
        {
            //move to right child node
            pCur = pCur->getRightChild();
        }
    }

    //exchange data and reinsert
    root->setBookData(pCur->getBookData());
    if (direction == 0)
    {
        pCur->getParent()->setLeftChild(NULL);
    }
    else
    {
        pCur->getParent()->setRightChild(NULL);
    }
    delete pCur;

    componentNumber--;
    delete s;

    //bubbling down
    heapifyDown(root);

    return true;
}

//recursive function that delete all nodes in heap called by destructor
void LoanBookHeap::delTree(LoanBookHeapNode* p)
{
    if (p == NULL)
    {
        //reached to leaf node
        return;
    }
    else
    {
        //recursive call
        delTree(p->getLeftChild());
        delTree(p->getRightChild());
        delete p;
    }
}