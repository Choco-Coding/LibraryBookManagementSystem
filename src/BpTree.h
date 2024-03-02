#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "LoanBookData.h"
#include <fstream>
#include <iostream>
#include <string>

class BpTree {
private:
	BpTreeNode* root;
	int			order;		// m children
	ofstream* fout;
	Compare c;
public:
	BpTree(ofstream *fout, int order = 3) {
		root = NULL;
		this->order = order;
		this->fout = fout;
	}
	~BpTree();
	/* essential */
	bool		Insert(LoanBookData* newData);
	bool		excessDataNode(BpTreeNode* pDataNode);
	bool		excessIndexNode(BpTreeNode* pIndexNode);
	void		splitDataNode(BpTreeNode* pDataNode);
	void		splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode* getRoot() { return root; }
	BpTreeNode* searchDataNode(string name);

	bool searchBook(string name);
	bool searchRange(string start, string end);

	void Delete(string name);				//delete the node in BpTree
	void mergeDataNode(BpTreeNode* pDataNode);	//Function used to merge data nodes when deleting node
	void mergeIndexNode(BpTreeNode* pIndexNode);	//Function used to merge index nodes when deleting node
	bool printBpTree();						//print all datas in BpTree ascending order
	void delTree(BpTreeNode* p);			//recursive function that delete all nodes in BpTree called by destructor

};

#endif
