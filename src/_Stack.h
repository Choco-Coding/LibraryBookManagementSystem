#pragma once
#include <iostream>

//This stack is used to find the last node to insert or delete elements in the LoanBookHeap
//This stack is used to move to the left or right child node depending on the number of element(componentNumber) in the LoanBookHeap

//Stack node class
class _StackNode
{
private:
    short direction;     //0: move to left child node / 1: move to right child node
    _StackNode* next;    //link

public:
    _StackNode()
    {
        direction = 0;
        next = NULL;
    }
    ~_StackNode()
    {
        direction = 0;
        next = NULL;
    }

    void setDirection(short direction) { this->direction = direction; }
    short getDirection() { return direction; }
    void setNext(_StackNode* p) { next = p; }
    _StackNode* getNext() { return next; }
};

//Stack class
class _Stack
{
private:
    _StackNode* top;    //top of stack

public:
    _Stack()
    {
        top = NULL;
    }
    ~_Stack()
    {
        //unallocate
        while (top != NULL)
        {
            _StackNode* temp = top;
            top = top->getNext();
            delete temp;
        }
    }

    void setTop(_StackNode* p) { top = p; }
    _StackNode* getTop() { return top; }

    void push(short direction)
    {
        //create new node
        _StackNode* pNew = new _StackNode;
        pNew->setDirection(direction);

        //insert
        if (top == NULL)
        {
            //first insert
            top = pNew;
        }
        else
        {
            //insert in front of first node
            pNew->setNext(top);
            top = pNew;
        }
    }

    short pop()
    {
        short direction = top->getDirection();

        if (top == NULL)
        {
            //do not exist
            return -1;
        }
        else
        {
            //remove and move ro next node
            _StackNode* temp = top;
            top = top->getNext();
            delete temp;
        }

        return direction;
    }
};
