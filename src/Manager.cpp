#include "Manager.h"

void Manager::run(const char* command) 
{
	fin.open(command);
	if(!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}

	int index = 0;

	while (!fin.eof())
	{
		//Extract cmd
		while (true)
		{
			ch = fin.get();
			if (ch == '\n' || ch == '\r' || ch == '\t' || fin.eof())
			{
				cmd[index] = '\0';
				break;
			}	//end of cmd
			else{
				if (index == 9)	//wrong command (too long)
				{
					index++;
					break;
				}
				else
				{
					cmd[index] = ch;
				}
			}
			
			index++;
		}

		if (index == 10)	//wrong command (too long)
		{
			printErrorCode(700);

			while (true)	//remove remaining characters
			{
				ch = fin.get();

				if (ch == '\n' || ch == '\r')
				{
					while (ch == '\n' || ch == '\r')
					{
						ch = fin.get();
					}
					break;
				}	//end of a line
				if (fin.eof())
				{
					break;
				}
			}
			
			index = 0;
			cmd[index++] = ch;
			continue;
		}
		
		//Compare and execute command
		if (!strcmp(cmd, "LOAD") || !strcmp(cmd, "PRINT_BP") || !strcmp(cmd, "DELETE") || !strcmp(cmd, "EXIT"))
		{
			//case of commands that do not have parameters
			bool error = false;

			//Check if There are parameters
			if (ch == '\n' || ch == '\r')	//no parameters
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}
			}
			else if (ch == ' ' || ch == '\t')
			{
				//ignore white spaces
				while (ch == ' ' || ch == '\t')
				{
					ch = fin.get();
				}

				if (ch == '\n' || ch == '\r')	//no parameters
				{
					while (ch == '\n' || ch == '\r')
					{
						ch = fin.get();
					}
				}
				else	//There are parameters -> error
				{
					error = true;

					while (true)	//remove remaining characters
					{
						ch = fin.get();

						if (ch == '\n' || ch == '\r')
						{
							while (ch == '\n' || ch == '\r')
							{
								ch = fin.get();
							}
							break;
						}	//end of a line
						if (fin.eof())
						{
							//end of file
							if (!strcmp(cmd, "LOAD"))
							{
								printErrorCode(100);
							}
							else if (!strcmp(cmd, "PRINT_BP"))
							{
								printErrorCode(400);
							}
							else if (!strcmp(cmd, "DELETE"))
							{
								printErrorCode(600);
							}
							else	//exit
							{
								printErrorCode(700);
							}
							
							break;
						}
					}
				}
			}
			else	//eof
			{
				error = false;
			}

			//function call
			if (!error)
			{
				bool success;
				if (!strcmp(cmd, "LOAD"))
				{
					success = LOAD();

					if (!success)
					{
						printErrorCode(100);
					}
					else
					{
						printSuccessCode("LOAD");
					}
				}
				else if (!strcmp(cmd, "PRINT_BP"))
				{
					success = PRINT_BP();

					if (!success)
					{
						printErrorCode(400);
					}
				}
				else if (!strcmp(cmd, "DELETE"))
				{
					success = DELETE();

					if (!success)
					{
						printErrorCode(600);
					}
					else
					{
						printSuccessCode("DELETE");
					}
				}
				else	//EXIT
				{
					printSuccessCode("EXIT");
					break;
				}
			}
			else
			{
				if (!strcmp(cmd, "LOAD"))
				{
					printErrorCode(100);
				}
				else if (!strcmp(cmd, "PRINT_BP"))
				{
					printErrorCode(400);
				}
				else if (!strcmp(cmd, "DELETE"))
				{
					printErrorCode(600);
				}
				else	//exit
				{
					printErrorCode(700);
				}
			}
		}
		else if (!strcmp(cmd, "ADD"))
		{
			bool success = ADD();

			if (!success)
			{
				printErrorCode(200);
			}
		}
		else if (!strcmp(cmd, "SEARCH_BP"))
		{
			//Check if there are parameters
			while (ch == '\t')
			{
				//Pass whitespace
				ch = fin.get();
			}
			if (fin.eof())
			{
				//end of file
				printErrorCode(300);
				break;
			}
			if (ch == '\n' || ch == '\r')
			{
				//There are no parameters
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}

				printErrorCode(300);

				index = 0;
				cmd[index++] = ch;
				continue;
			}

			//Check first parameter
			bool success;
			string parameter1, parameter2;

			while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ')
			{	
				//store parameter1
				parameter1.push_back(ch);
				ch = fin.get();
			}

			while (ch == '\t')
			{
				//Pass whitespace
				ch = fin.get();
			}
			if (fin.eof())
			{
				//end of file
				success = SEARCH_BP_BOOK(parameter1);

				if (!success)
				{
					printErrorCode(300);
				}

				break;
			}
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}
				success = SEARCH_BP_BOOK(parameter1);
				
				if (!success)
				{
					printErrorCode(300);
				}

				index = 0;
				cmd[index++] = ch;
				continue;
			}

			//Check second parameter
			while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ')
			{	
				//store parameter2
				parameter2.push_back(ch);
				ch = fin.get();
			}

			while (ch == '\t')
			{
				//Pass whitespace
				ch = fin.get();
			}
			if (fin.eof())
			{
				//end of file
				success = SEARCH_BP_RANGE(parameter1, parameter2);

				if (!success)
				{
					printErrorCode(300);
				}

				break;
			}
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}
			}
			else	//another parameters exist -> error
			{
				while (true)	//remove remaining characters
				{
					if (ch == '\n' || ch == '\r')
					{
						while (ch == '\n' || ch == '\r')
						{
							ch = fin.get();
						}
						break;
					}
					ch = fin.get();
				}

				printErrorCode(300);

				index = 0;
				cmd[index++] = ch;
				continue;
			}
			
			bool succees;
			succees = SEARCH_BP_RANGE(parameter1, parameter2);

			if(!succees)
			{
				printErrorCode(300);
			}
		}
		else if (!strcmp(cmd, "PRINT_ST"))
		{
			bool success;

			success = PRINT_ST();

			if (!success)
			{
				printErrorCode(500);
			}
		}
		else
		{
			//wrong command
			while (true)
			{
				ch = fin.get();
				if (ch == '\n' || ch == '\r')
				{
					while (ch == '\n' || ch == '\r')
					{
						ch = fin.get();
					}

					break;
				}
				if (fin.eof())
				{
					//end of file
					break;
				}
			}
			printErrorCode(700);
		}
		
		index = 0;
		cmd[index++] = ch;
	}

	fin.close();
	return;
}

bool Manager::LOAD()
{
	//Open file
	ifstream fdata;
	fdata.open("loan_book.txt");
	if(!fdata)
	{
		return false;
	}

	//Check if datas store in BpTree
	if (bptree->getRoot() != NULL)
	{
		return false;
	}

	char c = fdata.get();

	//Read file
	while (!fdata.eof())
	{
		string name, temp_code, author, temp_year, temp_loan_count;
		int code, year, loan_count;

		//name
		while (true)
		{
			if (c == '\t')
			{
				break;
			}

			name.push_back(c);
			c = fdata.get();
		}

		//code
		while (true)
		{
			c = fdata.get();

			if (c == '\t')
			{
				break;
			}

			temp_code.push_back(c);
		}
		code = stoi(temp_code);

		//author
		while (true)
		{
			c = fdata.get();

			if (c == '\t')
			{
				break;
			}

			author.push_back(c);
		}

		//year
		while (true)
		{
			c = fdata.get();

			if (c == '\t')
			{
				break;
			}

			temp_year.push_back(c);
		}
		year = stoi(temp_year);

		//loan_count
		while (true)
		{
			c = fdata.get();

			if (c == '\n' || c == '\r' || fdata.eof())
			{
				while (c == '\n' || c == '\r')
				{
					c = fdata.get();
					if (fdata.eof())
					{
						break;
					}
				}	
				break;
			}

			temp_loan_count.push_back(c);
		}
		loan_count = stoi(temp_loan_count);

		//create new LoanBookData instance
		LoanBookData* newData = new LoanBookData;
		newData->setBookData(name, code, author, year);
		

		while (newData->getLoanCount() < loan_count)
		{
			newData->updateCount();
		}
		
		//Insert into BpTree
		bool success_insert = bptree->Insert(newData);

		if (!success_insert)
		{
			//book is all loaned -> delete node and insert LoanBookHeap, Selection tree
			bptree->Delete(name);	//delete node in BpTree

			//insert in to Loan Book Heap
			if (stree->getRun(code)->getRoot() != NULL)
			{
				bool replay = stree->getRun(code)->Insert(newData);
				if (replay)
				{
					stree->Insert(stree->getRun(code)->getRoot()->getBookData());	//root is changed -> replay selection tree
				}
			}
			else
			{
				//first node of Loan Book Heap of the code
				LoanBookHeapNode* n = new LoanBookHeapNode;
				n->setBookData(newData);
				stree->getRun(code)->setRoot(n);
				stree->getRun(code)->setComponentNumber(1);
				stree->Insert(stree->getRun(code)->getRoot()->getBookData());	//root is changed -> replay selection tree
			}
		}
	}
	
	fdata.close();
	return true;
}

bool Manager::ADD()
{
	//pass whitespace
	while (ch == '\t')
	{
		ch = fin.get();
	}

	//Check if command is done
	if (fin.eof())
	{
		//end of file
		return false;
	}
	if (ch == '\n' || ch == '\r')
	{
		while (ch == '\n' || ch == '\r')
		{
			ch = fin.get();
		}

		return false;
	}

	//Check parameters
	string name, temp_code, author, temp_year;
	int code, year;

	//name
	while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ')
	{
		name.push_back(ch);
		ch = fin.get();
	}

	//pass whitespace
	while (ch == '\t')
	{
		ch = fin.get();
	}

	//Check if command is done
	if (fin.eof())
	{
		//end of file
		return false;
	}
	if (ch == '\n' || ch == '\r')
	{
		while (ch == '\n' || ch == '\r')
		{
			ch = fin.get();
		}

		return false;
	}

	//code
	if (ch >= '0' && ch <= '9')
	{
		while (ch >= '0' && ch <= '9')
		{
			temp_code.push_back(ch);
			ch = fin.get();
		}
		code = stoi(temp_code);
	}
	else
	{
		while (true)
		{
			ch = fin.get();
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}

				return false;
			}
			if (fin.eof())
			{
				//end of file
				return false;
			}
		}
	}

	//valid code?
	if (code != 0 && !(code >= 100 && code <= 700 && code % 100 == 0))
	{
		while (true)	//remove remaining characters
		{
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}
				break;
			}
			ch = fin.get();
		}

		return false; //invalid code
	}

	//pass whitespace
	while (ch == '\t')
	{
		ch = fin.get();
	}

	//Check if command is done
	if (fin.eof())
	{
		//end of file
		return false;
	}
	if (ch == '\n' || ch == '\r')
	{
		while (ch == '\n' || ch == '\r')
		{
			ch = fin.get();
		}

		return false;
	}

	//author
	while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ')
	{
		author.push_back(ch);
		ch = fin.get();
	}

	//pass whitespace
	while (ch == '\t')
	{
		ch = fin.get();
	}

	//Check if command is done
	if (fin.eof())
	{
		//end of file
		return false;
	}
	if (ch == '\n' || ch == '\r')
	{
		while (ch == '\n' || ch == '\r')
		{
			ch = fin.get();
		}

		return false;
	}

	//year
	if (ch >= '0' && ch <= '9')
	{
		while (ch >= '0' && ch <= '9')
		{
			temp_year.push_back(ch);
			ch = fin.get();
		}
		year = stoi(temp_year);
	}
	else
	{
		while (true)
		{
			ch = fin.get();
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}

				return false;
			}
			if (fin.eof())
			{
				//end of file
				return false;
			}
		}
	}

	//pass whitespace
	while (ch == '\t')
	{
		ch = fin.get();
	}
	if (fin.eof())
	{
		//create new LoanBookData instance
		LoanBookData* newData = new LoanBookData;
		newData->setBookData(name, code, author, year);

		//Insert into BpTree
		bool success_insert = bptree->Insert(newData);

		flog << "========ADD========" << endl;
		flog << name << '/';
		if (code == 0)
		{
			flog << "000";
		} 
		else
		{
			flog << code;
		}
		flog << '/' << author << '/' << year << endl;
		flog << "==========================" << endl << endl;

		if (!success_insert)
		{
			//book is all loaned -> delete node and insert LoanBookHeap, Selection tree
			bptree->Delete(name);	//delete node in BpTree

			//insert in to Loan Book Heap
			if (stree->getRun(code)->getRoot() != NULL)
			{
				bool replay = stree->getRun(code)->Insert(newData);
				if (replay)
				{
					stree->Insert(stree->getRun(code)->getRoot()->getBookData());	//root is changed -> replay selection tree
				}
			}
			else
			{
				//first node of Loan Book Heap of the code
				LoanBookHeapNode* n = new LoanBookHeapNode;
				n->setBookData(newData);
				stree->getRun(code)->setRoot(n);
				stree->getRun(code)->setComponentNumber(1);
				stree->Insert(stree->getRun(code)->getRoot()->getBookData());	//root is changed -> replay selection tree
			}
		}

		return true;
	}
	if (ch == '\n' || ch == '\r')
	{
		while (ch == '\n' || ch == '\r')
		{
			ch = fin.get();
		}
	}
	else	//another parameters exist -> error
	{
		while (true)	//remove remaining characters
		{
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}
				break;
			}
			ch = fin.get();
		}

		return false;
	}

	//create new LoanBookData instance
	LoanBookData* newData = new LoanBookData;
	newData->setBookData(name, code, author, year);
	
	//Insert into BpTree
	bool success_insert = bptree->Insert(newData);


	flog << "========ADD========" << endl;
	flog << name << '/';
	if (code == 0)
	{
		flog << "000";
	} 
	else
	{
		flog << code;
	}
	flog << '/' << author << '/' << year << endl;
	flog << "==========================" << endl << endl;

	if (!success_insert)
	{
		//book is all loaned -> delete node and insert LoanBookHeap, Selection tree
		bptree->Delete(name);	//delete node in BpTree

		//insert in to Loan Book Heap
		if (stree->getRun(code)->getRoot() != NULL)
		{
			bool replay = stree->getRun(code)->Insert(newData);
			if (replay)
			{
				stree->Insert(stree->getRun(code)->getRoot()->getBookData());	//root is changed -> replay selection tree
			}
		}
		else
		{
			//first node of Loan Book Heap of the code
			LoanBookHeapNode* n = new LoanBookHeapNode;
			n->setBookData(newData);
			stree->getRun(code)->setRoot(n);
			stree->getRun(code)->setComponentNumber(1);
			stree->Insert(stree->getRun(code)->getRoot()->getBookData());	//root is changed -> replay selection tree
		}
	}

	return true;
}

bool Manager::SEARCH_BP_BOOK(string book) 
{
	return bptree->searchBook(book);
}

bool Manager::SEARCH_BP_RANGE(string s, string e) 
{
	return bptree->searchRange(s, e);
}

bool Manager::PRINT_BP() 
{
	return bptree->printBpTree();
}

bool Manager::PRINT_ST() 
{
	//pass whitespace
	while (ch == '\t')
	{
		ch = fin.get();
	}

	//Check if there is parameter
	if (fin.eof())
	{
		//end of file
		return false;
	}
	if (ch == '\n' || ch == '\r')
	{
		while (ch == '\n' || ch == '\r')
		{
			ch = fin.get();
		}

		return false;
	}

	//Check parameter
	string temp_code;
	int code;

	while (ch >= '0' && ch <= '9')
	{
		temp_code.push_back(ch);
		ch = fin.get();
	}
	code = stoi(temp_code);

	//valid code?
	if (code != 0 && !(code >= 100 && code <= 700 && code % 100 == 0))
	{
		while (true)	//remove remaining characters
		{
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}
				break;
			}
			ch = fin.get();
		}

		return false; //invalid code
	}

	//pass whitespace
	while (ch == '\t')
	{
		ch = fin.get();
	}
	if (fin.eof())
	{
		//print data stored in Loan book heap in selection tree according to code
		bool success = stree->printBookData(code);
		if (success == false)
		{
			return false;
		}

		return true;
	}
	if (ch == '\n' || ch == '\r')
	{
		while (ch == '\n' || ch == '\r')
		{
			ch = fin.get();
		}
	}
	else	//another parameters exist -> error
	{
		while (true)	//remove remaining characters
		{
			if (ch == '\n' || ch == '\r')
			{
				while (ch == '\n' || ch == '\r')
				{
					ch = fin.get();
				}
				break;
			}
			ch = fin.get();
		}

		return false;
	}


	//print data stored in Loan book heap in selection tree according to code
	bool success = stree->printBookData(code);
	if (success == false)
	{
		return false;
	}

	return true;
}

bool Manager::DELETE() 
{
	//delete root of selection tree
	bool success = stree->Delete();

	if (!success)
	{
		return false;
	}
	return true;
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	flog << "========ERROR========" << endl;
	flog << n << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode(string command) {//SUCCESS CODE PRINT 
	flog << "========" << command << "========" << endl;
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

