#include "SelectionTree.h"

bool SelectionTree::Insert(LoanBookData* newData) {
    SelectionTreeNode* p = run[newData->getCode() / 100]->getLeaf();    //get leaf node where newData is inserted
    p->setBookData(newData);    //set new Book data
    
    SortStree(p);

    return true;
}

//function to sort selection tree
void SelectionTree::SortStree(SelectionTreeNode* p)
{
    p = p->getParent();         //move to parent node
    //compare name string and determine winner while move from leaf to root
    while (p != NULL)
    {
        if (p->getLeftChild()->getBookData() == NULL)   //left child node is empty
        {
            //setting book data of p according to right child node
            p->setBookData(p->getRightChild()->getBookData());
            p = p->getParent();         //move to parent node
            continue;
        }

        if (p->getRightChild()->getBookData() == NULL)   //right child node is empty
        {
            //setting book data of p according to right child node
            p->setBookData(p->getLeftChild()->getBookData());
            p = p->getParent();         //move to parent node
            continue;
        }

        if (c.compareName(p->getLeftChild()->getBookData()->getName(), p->getRightChild()->getBookData()->getName()) <= 0)
        {
            //Left child node's key is smaller -> Left child node is winner
            p->setBookData(p->getLeftChild()->getBookData());
        }
        else
        {
            //right child node's key is smaller -> right child node is winner
            p->setBookData(p->getRightChild()->getBookData());
        }

        p = p->getParent();         //move to parent node
    }
}

bool SelectionTree::Delete() {
    if (root->getBookData() == NULL)
    {
        //selection tree is empty
        return false;
    }

    SelectionTreeNode* p = root;

    string delName = p->getBookData()->getName();

    //delete data of root and its data of children nodes that have same data from root to leaf
    while (true)
    {
        p->setBookData(NULL);
        
        //Check if p reaches to leaf node
        if (p->getHeap() != NULL)
        {
            break;
        }

        //move
        if (p->getLeftChild()->getBookData() != NULL)
        {
            if (p->getLeftChild()->getBookData()->getName() == delName)
            {
                p = p->getLeftChild();
                continue;
            }
        }
        if (p->getRightChild()->getBookData() != NULL)
        {
            if (p->getRightChild()->getBookData()->getName() == delName)
            {
                p = p->getRightChild();
                continue;
            }
        }
    }

    bool exist = p->getHeap()->Delete(); //delete root of heap
    
    if (exist)
    {
        //heap is not empty
        Insert(p->getHeap()->getRoot()->getBookData()); //replay
    }
    else
    {
        SortStree(p);
    }

    return true;
}

bool SelectionTree::printBookData(int bookCode) {
    if (root == NULL || run[bookCode/100]->getRoot() == NULL)
    {
        //empty
        return false;
    }
    
    LoanBookHeapNode* copyHeapNode = root->deepCopy(run[bookCode/100]->getRoot(), NULL);  //copy heap

    LoanBookHeap* copyHeap = new LoanBookHeap;
    copyHeap->setRoot(copyHeapNode);
    copyHeap->setComponentNumber(run[bookCode/100]->getComponenetNumber());
    
    *fout << "========PRINT_ST========" << endl;

    //print
    while (copyHeap->getRoot() != NULL)
    {
        *fout << copyHeap->getRoot()->getBookData()->getName() << '/'; 
		
		if (copyHeap->getRoot()->getBookData()->getCode() == 0)
		{
			*fout << "000";
		}
		else
		{
			*fout << copyHeap->getRoot()->getBookData()->getCode();
		}

		*fout << '/' << copyHeap->getRoot()->getBookData()->getAuthor() << '/' << copyHeap->getRoot()->getBookData()->getYear() << '/' << copyHeap->getRoot()->getBookData()->getLoanCount() << endl;

        copyHeap->Delete();
    }

    *fout << "==========================" << endl << endl;

    delete copyHeap;
    return true;
}

//construct selection tree of 8 runs
void SelectionTree::construct(SelectionTreeNode* p, int height)
{
    if (height < 4)
    {
        //set left child node
        SelectionTreeNode* newLeft = new SelectionTreeNode;
        p->setLeftChild(newLeft);
        newLeft->setParent(p);

        //set right child node
        SelectionTreeNode* newRight = new SelectionTreeNode;
        p->setRightChild(newRight);
        newRight->setParent(p);
        
        //recursive call
        construct(newLeft, height + 1);
        construct(newRight, height + 1);
    }
    else
    {
        //reach to leaf node
        run[i] = new LoanBookHeap;  //allocate heap
        p->setHeap(run[i]);   //link together
        run[i]->setLeaf(p);
        i++;
    }
}

//recursive function that delete all nodes in Selection tree called by destructor
void SelectionTree::delTree(SelectionTreeNode* p)
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
