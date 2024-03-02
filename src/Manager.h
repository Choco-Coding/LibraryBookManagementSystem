#pragma once
#include "SelectionTree.h"
#include "BpTree.h"

class Manager
{
private:
	char* cmd;
	char ch;
	BpTree* bptree;
	SelectionTree* stree;

public:
	Manager(int bpOrder)	//constructor
	{
		/* You must fill here */
		flog.open("log.txt", ios::app);
		if(!flog)
		{
			exit(-1);
			return;
		}

		cmd = new char[10];
		bptree = new BpTree(&flog);
		stree = new SelectionTree(&flog);
	}


	~Manager()//destructor
	{
		/* You must fill here */
		flog.close();
		delete cmd;
		delete bptree;
		delete stree;
	}

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	bool ADD();

	bool SEARCH_BP_BOOK(string book);
	bool SEARCH_BP_RANGE(string start, string end);

	bool PRINT_BP();
	bool PRINT_ST();

	bool DELETE();

	void printErrorCode(int n);
	void printSuccessCode(string command);

};

