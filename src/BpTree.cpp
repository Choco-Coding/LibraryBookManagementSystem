#include "BpTree.h"

BpTree::~BpTree()
{
	//free all allocated memory of BpTree
	if (root != NULL)
	{
		delTree(root);
	}
}

bool BpTree::Insert(LoanBookData* newData) {

	BpTreeNode* pInsert;

	//check if all books are loaned
	if (newData->getCode() >= 0 && newData->getCode() <= 200 && newData->getLoanCount() >= 3)
	{
		//false return value means that all books are loaned -> insert into selection tree
		return false;
	}
	if (newData->getCode() >= 300 && newData->getCode() <= 400 && newData->getLoanCount() >= 4)
	{
		//all books are loaned -> insert into selection tree
		return false;
	}
	if (newData->getCode() >= 500 && newData->getCode() <= 700 && newData->getLoanCount() >= 2)
	{
		//all books are loaned -> insert into selection tree
		return false;
	}


	if (root == NULL)
	{
		//first node
		//create new node and set root pointing to new node
		root = new BpTreeDataNode;
		root->insertDataMap(newData->getName(), newData);
	}
	else
	{
		//Search that node to have same key exists in BpTree
		pInsert = searchDataNode(newData->getName());

		if (pInsert == NULL)
		{
			//don't exist in BpTree -> Search position to insert node -> insert new node
			pInsert = root;

			while (pInsert->getMostLeftChild() != NULL)		//while index node
			{
				bool move = false;
				map<string, BpTreeNode*>::iterator i;

				//compare name string to determine which child node to move
				for (i = pInsert->getIndexMap()->begin(); i != pInsert->getIndexMap()->end(); i++)
				{
					//if newData's key is smaller -> move to child node					
					if (c.compareName(newData->getName(), i->first) < 0)
					{
						if (i == pInsert->getIndexMap()->begin())	//if smaller than first key, move to most left child
						{
							pInsert = pInsert->getMostLeftChild();
						}
						else										//move to left child
						{
							i--;
							pInsert = i->second;
						}
						move = true;
						break;
					}
				}

				if (!move)
				{
					//if larger than last key, move to most right child
					i--;
					pInsert = i->second;
				}
			}

			//insert data into map of pInsert
			pInsert->insertDataMap(newData->getName(), newData);
			pInsert->setMostLeftChild(NULL);

			//Check if data node exceed the order
			if (excessDataNode(pInsert))
			{
				//exceed order -> split
				splitDataNode(pInsert);
			}
		}
		else
		{
			//node that has same key exists in BpTree -> increase loan_count
			map<string, LoanBookData*>::iterator it = pInsert->getDataMap()->find(newData->getName());
			it->second->updateCount();
			int pCode = it->second->getCode();
			int pCount = it->second->getLoanCount();

			//check if all books are loaned
			if (pCode >= 0 && pCode <= 200 && pCount >= 3)
			{
				//all books are loaned -> insert into selection tree
				return false;
			}
			if (pCode >= 300 && pCode <= 400 && pCount >= 4)
			{
				//all books are loaned -> insert into selection tree
				return false;
			}
			if (pCode >= 500 && pCode <= 700 && pCount >= 2)
			{
				//all books are loaned -> insert into selection tree
				return false;
			}
		}
	}

	return true;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	BpTreeDataNode* splitNode = new BpTreeDataNode;		//splited node
	map<string, LoanBookData*>::iterator i = pDataNode->getDataMap()->begin();	//first data

	splitNode->insertDataMap(i->first, i->second);
	

	//link together
	if (pDataNode->getPrev() != NULL)
	{
		pDataNode->getPrev()->setNext(splitNode);
		splitNode->setPrev(pDataNode->getPrev());
	}
	splitNode->setNext(pDataNode);
	pDataNode->setPrev(splitNode);

	i++;	// -> it means second element of map(split position data)

	if (pDataNode->getParent() == NULL)
	{
		//do not exist parent node of pDataNode -> create new node
		BpTreeIndexNode* splitParent = new BpTreeIndexNode;		//parnet node of split nodes

		splitParent->insertIndexMap(i->first, pDataNode);	//link child node
		splitParent->setMostLeftChild(splitNode);

		pDataNode->setParent(splitParent);	//link parent node
		splitNode->setParent(splitParent);

		i = pDataNode->getDataMap()->begin();
		pDataNode->deleteMap(i->first);

		root = splitParent;	//set root
	}
	else
	{
		//There exist parent node of pDataNode -> Combine with parent node
		pDataNode->getParent()->insertIndexMap(i->first, pDataNode);
		splitNode->setParent(pDataNode->getParent());
		
		if (pDataNode->getParent()->getMostLeftChild() == pDataNode)
		{
			//case of split most left child node
			pDataNode->getParent()->setMostLeftChild(splitNode);
		}
		else
		{
			map<string, BpTreeNode*>::iterator k = pDataNode->getParent()->getIndexMap()->find(i->first);
			k--;
			k->second = splitNode;
		}

		i = pDataNode->getDataMap()->begin();
		pDataNode->deleteMap(i->first);
		
		//Check if data node exceed the order
		if (excessIndexNode(pDataNode->getParent()))
		{
			//exceed order -> split
			splitIndexNode(pDataNode->getParent());
		}
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	BpTreeIndexNode* splitNode = new BpTreeIndexNode;		//splited node
	map<string, BpTreeNode*>::iterator i = pIndexNode->getIndexMap()->begin();	//first data

	//Setting splited data
	splitNode->insertIndexMap(i->first, i->second);
	splitNode->setMostLeftChild(pIndexNode->getMostLeftChild());

	//link children's parent node
	pIndexNode->getMostLeftChild()->setParent(splitNode);
	i->second->setParent(splitNode);

	i++;	// -> it means second element of map(split position data)

	if (pIndexNode->getParent() == NULL)
	{
		//do not exist parent node of pDataNode -> create new node
		BpTreeIndexNode* splitParent = new BpTreeIndexNode;		//parnet node of split nodes

		splitParent->insertIndexMap(i->first, pIndexNode);	//link child node
		splitParent->setMostLeftChild(splitNode);

		pIndexNode->setParent(splitParent);	//link parent node
		splitNode->setParent(splitParent);

		pIndexNode->setMostLeftChild(i->second);

		i = pIndexNode->getIndexMap()->begin();	//delete elements
		pIndexNode->deleteMap(i->first);
		i = pIndexNode->getIndexMap()->begin();
		pIndexNode->deleteMap(i->first);

		root = splitParent;	//set root
	}
	else
	{
		//There exist parent node of pDataNode -> Combine with parent node
		pIndexNode->getParent()->insertIndexMap(i->first, pIndexNode);
		splitNode->setParent(pIndexNode->getParent());
		
		if (pIndexNode->getParent()->getMostLeftChild() == pIndexNode)
		{
			//case of split most left child node
			pIndexNode->getParent()->setMostLeftChild(splitNode);
		}
		else
		{
			map<string, BpTreeNode*>::iterator k = pIndexNode->getParent()->getIndexMap()->find(i->first);
			k--;
			k->second = splitNode;
		}

		pIndexNode->setMostLeftChild(i->second);

		i = pIndexNode->getIndexMap()->begin();	//delete elements
		pIndexNode->deleteMap(i->first);
		i = pIndexNode->getIndexMap()->begin();
		pIndexNode->deleteMap(i->first);

		//Check if data node exceed the order
		if (excessIndexNode(pIndexNode->getParent()))
		{
			//exceed order -> split
			splitIndexNode(pIndexNode->getParent());
		}
	}
}

BpTreeNode* BpTree::searchDataNode(string name) {
	BpTreeNode* pCur = root;
	
	while (pCur->getMostLeftChild() != NULL)		//while index node
	{
		bool move = false;
		map<string, BpTreeNode*>::iterator i;

		//compare name string to determine which child node to move
		for (i = pCur->getIndexMap()->begin(); i != pCur->getIndexMap()->end(); i++)
		{
			//if parameter is smaller -> move to child node
			if (c.compareName(name, i->first) < 0)
			{
				if (i == pCur->getIndexMap()->begin())	//if smaller than first key, move to most left child
				{
					pCur = pCur->getMostLeftChild();
				}
				else									//move to left child
				{
					i--;
					pCur = i->second;
				}
				move = true;
				break;
			}
		}

		if (!move)
		{
			//if larger than last key, move to most right child
			i--;
			pCur = i->second;
		}
	}

	//data node
	map<string, LoanBookData*>::iterator i;
	for (i = pCur->getDataMap()->begin(); i != pCur->getDataMap()->end(); i++)
	{
		if (name.compare(i->first) == 0)
		{
			return pCur;	//same name
		}
	}

	pCur = NULL;	//same key node doesn't exist in BpTree
	return pCur;
}

bool BpTree::searchBook(string name) {
	if (root == NULL)
	{
		//BpTree is empty
		return false;
	}

	BpTreeNode* p = searchDataNode(name);	//search data node according to name string

	if (p == NULL)
	{
		//data doesn't exist
		return false;
	}
	else
	{
		map<string, LoanBookData*>::iterator i = p->getDataMap()->find(name);

		//print data
		*fout << "========SEARCH_BP========" << endl;
		*fout << i->second->getName() << '/';
		if (i->second->getCode() == 0)
		{
			*fout << "000";
		}
		else
		{
			*fout << i->second->getCode();
		}
		*fout << '/' << i->second->getAuthor() << '/' << i->second->getYear() << '/' << i->second->getLoanCount() << endl;
		*fout << "==========================" << endl << endl;

		return true;
	}

}

bool BpTree::searchRange(string start, string end) {
	if (root == NULL)
	{
		//BpTree is empty
		return false;
	}

	BpTreeNode* pCur = root;

	//move to node according to start parameter
	while (pCur->getMostLeftChild() != NULL)		//while index node
	{
		bool move = false;
		map<string, BpTreeNode*>::iterator i;

		//compare name string to determine which child node to move
		for (i = pCur->getIndexMap()->begin(); i != pCur->getIndexMap()->end(); i++)
		{
			string partial = c.partialString(i->first, start.length());	//From the beginning, copy letters as many as start string

			//if parameter is smaller -> move to child node
			if (c.compareName(start, partial) < 0)
			{
				if (i == pCur->getIndexMap()->begin())	//if smaller than first key, move to most left child
				{
					pCur = pCur->getMostLeftChild();
				}
				else									//move to left child
				{
					i--;
					pCur = i->second;
				}
				move = true;
				break;
			}
		}

		if (!move)
		{
			//if larger than last key, move to most right child
			i--;
			pCur = i->second;
		}
	}

	//print -> move to next data node with linked list of data node
	for (map<string, LoanBookData*>::iterator it = pCur->getDataMap()->begin(); it != pCur->getDataMap()->end(); it++)
	{
		string partial_start = c.partialString(it->first, start.length());
		string partial_end = c.partialString(it->first, end.length());

		//start <= key <= end
		if (c.compareName(partial_start, start) >= 0 && c.compareName(partial_end, end) <= 0)	//start position
		{
			*fout << "========SEARCH_BP========" << endl;
			do {
				while (it != pCur->getDataMap()->end())
				{
					string partial = c.partialString(it->first, end.length());

					if (c.compareName(partial, end) <= 0)
					{
						//print data
						*fout << it->second->getName() << '/';
						if (it->second->getCode() == 0)
						{
							*fout << "000";
						}
						else
						{
							*fout << it->second->getCode();
						}
						*fout << '/' << it->second->getAuthor() << '/' << it->second->getYear() << '/' << it->second->getLoanCount() << endl;
					}
					else
					{
						//range out
						*fout << "==========================" << endl << endl;
						return true;
					}

					it++;
				}

				pCur = pCur->getNext();	//move to next node
				if (pCur != NULL)
				{
					it = pCur->getDataMap()->begin();	//first data
				}

			} while (pCur != NULL);		//while pCur reaches last node in linked list of data node
			*fout << "==========================" << endl << endl;
			return true;
		}
	}				

	return false;
}

void BpTree::Delete(string name)		//delete the node in BpTree
{
	BpTreeNode* deletedNode = searchDataNode(name);

	if (root == deletedNode)
	{
		//data to be deleted is in the root
		if (root->getDataMap()->size() == 1)
		{
			root->deleteMap(name);
			delete root;
			root == NULL;
		}
		else
		{
			root->deleteMap(name);
		}

		return;
	}

	//Check if underflow will occur
	if (deletedNode->getDataMap()->size() == 1)
	{
		//underflow will occur
		//try to rearrange
		bool rearrange = false;
		BpTreeNode* p = deletedNode;

		for (int i = 1; i <= 2; i++)	//check left
		{
			p = p->getPrev();
			if (p == NULL)
			{
				break;
			}
			if (p->getParent() != deletedNode->getParent())
			{
				//not sibling
				break;
			}

			if (p->getDataMap()->size() > 1)
			{
				//rearrange
				if (i == 1)
				{
					map<string, LoanBookData*>::iterator it = p->getDataMap()->end();
					it--;
					deletedNode->insertDataMap(it->first, it->second);
					deletedNode->getParent()->deleteMap(name);
					deletedNode->getParent()->insertIndexMap(it->first, deletedNode);
					p->deleteMap(it->first);
				}
				else	//i == 2
				{
					map<string, LoanBookData*>::iterator it = p->getDataMap()->end();
					it--;
					p->getNext()->insertDataMap(it->first, it->second);
					p->getNext()->getParent()->deleteMap(name);
					p->getNext()->getParent()->insertIndexMap(it->first, p->getNext());
					p->deleteMap(it->first);

					p = p->getNext();
					it = p->getDataMap()->end();
					it--;
					deletedNode->insertDataMap(it->first, it->second);
					deletedNode->getParent()->deleteMap(name);
					deletedNode->getParent()->insertIndexMap(it->first, deletedNode);
					p->deleteMap(it->first);
				}

				rearrange = true;
				break;
			}
		}

		if(!rearrange)
		{
			p = deletedNode;

			for (int i = 1; i <= 2; i++)	//check right
			{
				p = p->getNext();
				if (p == NULL)
				{
					break;
				}
				if (p->getParent() != deletedNode->getParent())
				{
					//not sibling
					break;
				}

				if (p->getDataMap()->size() > 1)
				{
					//rearrange
					if (i == 1)
					{
						map<string, LoanBookData*>::iterator it = p->getDataMap()->begin();
						deletedNode->insertDataMap(it->first, it->second);
						p->getParent()->deleteMap(it->first);
						p->deleteMap(it->first);
						it = p->getDataMap()->begin();
						p->getParent()->insertIndexMap(it->first, p);
					}
					else	//i == 2
					{
						map<string, LoanBookData*>::iterator it = p->getDataMap()->begin();
						p->getPrev()->insertDataMap(it->first, it->second);
						p->getParent()->deleteMap(it->first);
						p->deleteMap(it->first);
						it = p->getDataMap()->begin();
						p->getParent()->insertIndexMap(it->first, p);

						p = p->getPrev();
						it = p->getDataMap()->begin();
						deletedNode->insertDataMap(it->first, it->second);
						p->getParent()->deleteMap(it->first);
						p->deleteMap(it->first);
						it = p->getDataMap()->begin();
						p->getParent()->insertIndexMap(it->first, p);
					}

					rearrange = true;
					break;
				}
			}
		}


		if (!rearrange)
		{
			//fail to rearrange -> merge
			mergeDataNode(deletedNode);
		}
	}

	//Check that the data to be deleted exists on the leftmost side
	if (deletedNode->getDataMap()->find(name) == deletedNode->getDataMap()->begin())
	{
		//exists on the leftmost side -> name of the data to be deleted exists in the index nodes
		//-> change key of index node
		map<string, LoanBookData*>::iterator k = deletedNode->getDataMap()->begin();
		k++;
		string newName = k->first;
		BpTreeNode* p = deletedNode->getParent();

		while (p != NULL)
		{
			for (map<string, BpTreeNode*>::iterator i = p->getIndexMap()->begin(); i != p->getIndexMap()->end(); i++)
			{
				if (i->first == name)
				{
					BpTreeNode* temp = i->second;
					p->deleteMap(name);
					p->insertIndexMap(newName, temp);

					//delete
					deletedNode->deleteMap(name);
					return;
				}
			}
			
			p = p->getParent();
		}
	}

	//delete
	deletedNode->deleteMap(name);
	return;
}

//Function used to merge data nodes when deleting node
void BpTree::mergeDataNode(BpTreeNode* pDataNode)
{
	BpTreeNode* sibling;
	bool merge = false;

	if (pDataNode->getNext() != NULL)
	{
		if (pDataNode->getParent() == pDataNode->getNext()->getParent())
		{
			//merge with right node
			sibling = pDataNode->getNext();
			pDataNode->insertDataMap(sibling->getDataMap()->begin()->first, sibling->getDataMap()->begin()->second);

			//link together
			if (sibling->getNext() == NULL)
			{
				pDataNode->setNext(NULL);
			}
			else
			{
				pDataNode->setNext(sibling->getNext());
				sibling->getNext()->setPrev(pDataNode);
			}
			
			if(pDataNode->getParent()->getIndexMap()->size() <= 1)
			{
				//merge index node
				pDataNode->getParent()->setMostLeftChild(pDataNode);
				mergeIndexNode(pDataNode->getParent());
			}
			else
			{
				//link with parent node
				if (pDataNode->getParent()->getMostLeftChild() == pDataNode)
				{
					pDataNode->getParent()->deleteMap(pDataNode->getParent()->getIndexMap()->begin()->first);
					pDataNode->getParent()->setMostLeftChild(pDataNode);
				}
				else
				{
					map<string, BpTreeNode*>::iterator i = pDataNode->getParent()->getIndexMap()->begin();
					i++;
					pDataNode->getParent()->deleteMap(i->first);
				}
			}

			delete sibling;
			merge = true;
		}
	}

	if (!merge)
	{
		//merge with left node
		sibling = pDataNode->getPrev();
		pDataNode->insertDataMap(sibling->getDataMap()->begin()->first, sibling->getDataMap()->begin()->second);

		//link together
		if (sibling->getPrev() == NULL)
		{
			pDataNode->setPrev(NULL);
		}
		else
		{
			pDataNode->setPrev(sibling->getPrev());
			sibling->getPrev()->setNext(pDataNode);
		}
			
		if(pDataNode->getParent()->getIndexMap()->size() <= 1)
		{
			//merge index node
			pDataNode->getParent()->setMostLeftChild(pDataNode);
			mergeIndexNode(pDataNode->getParent());
		}
		else
		{
			//link with parent node
			map<string, BpTreeNode*>::iterator i = pDataNode->getParent()->getIndexMap()->begin();
			i->second = pDataNode;
			i++;
			pDataNode->getParent()->deleteMap(i->first);
		}

		delete sibling;
		merge = true;
	}
}

//Function used to merge index nodes when deleting node
void BpTree::mergeIndexNode(BpTreeNode* pIndexNode)
{
	if (root == pIndexNode)
	{
		root = root->getMostLeftChild();
		root->setParent(NULL);
		delete pIndexNode;
		return;
	}

	BpTreeNode* parent = pIndexNode->getParent();
	BpTreeNode* sibling;

	//Choose sibling node to be merged with pIndexNode
	if (parent->getMostLeftChild() == pIndexNode)
	{
		sibling = parent->getIndexMap()->begin()->second;
		sibling->insertIndexMap(parent->getIndexMap()->begin()->first, sibling->getMostLeftChild());
		sibling->setMostLeftChild(pIndexNode->getMostLeftChild());
		parent->setMostLeftChild(sibling);
		pIndexNode->getMostLeftChild()->setParent(sibling);

		parent->deleteMap(parent->getIndexMap()->begin()->first);

		//check that overflow occur in sibling node
		if(excessIndexNode(sibling))
		{
			splitIndexNode(sibling);
		}

		//check that underflow occur in parent node
		if (parent->getIndexMap()->size() == 0)
		{
			mergeIndexNode(parent);
		}
		delete pIndexNode;
	}
	else
	{
		map<string, BpTreeNode*>::iterator i = parent->getIndexMap()->begin();
		if (i->second == pIndexNode)
		{
			i++;
			if (i != parent->getIndexMap()->end())
			{
				sibling = i->second;
				sibling->insertIndexMap(i->first, sibling->getMostLeftChild());
				sibling->setMostLeftChild(pIndexNode->getMostLeftChild());
				i--;
				i->second = sibling;
				pIndexNode->getMostLeftChild()->setParent(sibling);
				
				i++;
				parent->deleteMap(i->first);

				//check that overflow occur in sibling node
				if(excessIndexNode(sibling))
				{
					splitIndexNode(sibling);
				}

				//-> underflow do not occur in parent node

				delete pIndexNode;
			}
			else
			{
				sibling = parent->getMostLeftChild();
				sibling->insertIndexMap(parent->getIndexMap()->begin()->first, pIndexNode->getMostLeftChild());
				pIndexNode->getMostLeftChild()->setParent(sibling);

				parent->deleteMap(parent->getIndexMap()->begin()->first);

				delete pIndexNode;

				//check that overflow occur in sibling node
				if(excessIndexNode(sibling))
				{
					splitIndexNode(sibling);
					return;
				}

				//underflow occur in parent node
				mergeIndexNode(parent);
			}
		}
		else
		{
			sibling = i->second;

			i++;
			sibling->insertIndexMap(i->first, pIndexNode->getMostLeftChild());
			
			pIndexNode->getMostLeftChild()->setParent(sibling);
			
			parent->deleteMap(i->first);

			//check that overflow occur in sibling node
			if(excessIndexNode(sibling))
			{
				splitIndexNode(sibling);
			}

			//-> underflow do not occur in parent node
			
			delete pIndexNode;
		}
	}
}

bool BpTree::printBpTree()			//print all datas in BpTree ascending order
{
	if (root == NULL)
	{
		//BpTree is empty
		return false;
	}

	BpTreeNode* pCur = root;

	//move to most left data node
	while (pCur->getMostLeftChild() != NULL)
	{
		pCur = pCur->getMostLeftChild();
	}

	*fout << "========PRINT_BP========" << endl;

	//print data while pCur move to next data node in linked list
	while (pCur != NULL)
	{
		for (map<string, LoanBookData*>::iterator it = pCur->getDataMap()->begin(); it != pCur->getDataMap()->end(); it++)
		{
			*fout << it->second->getName() << '/'; 
		
			if (it->second->getCode() == 0)
			{
				*fout << "000";
			}
			else
			{
				*fout << it->second->getCode();
			}

			*fout << '/' << it->second->getAuthor() << '/' << it->second->getYear() << '/' << it->second->getLoanCount() << endl;
		}
		
		pCur = pCur->getNext();
	}
	*fout << "==========================" << endl << endl;

	return true;
}

//recursive function that delete all nodes in BpTree called by destructor
void BpTree::delTree(BpTreeNode* p)
{
	if (p->getMostLeftChild() == NULL)	//reach to data node
	{
		//unallocate all Loan Book Data in p
		for (map<string, LoanBookData*>::iterator it = p->getDataMap()->begin(); it != p->getDataMap()->end(); it++)
		{
			if (it->second != NULL)
			{
				delete it->second;
			}
		}

		delete p;
		return;
	}

	//recursive call
	delTree(p->getMostLeftChild());
	for (map<string, BpTreeNode*>::iterator it = p->getIndexMap()->begin(); it != p->getIndexMap()->end(); it++)
	{
		delTree(it->second);
	}
	
	delete p;
	return;
}