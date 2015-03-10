/* 
This is a simple unittest of the prioritybuffer class.
It tests for two things, in essence.
First, that FIFO behavour holds.
Second, it checks that prioitizing works independent of order of entry.

Also, this test is meant to be run, and obviously not included
into any other program. Feel free to alter it, though.
*/


#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

#include "prioritybuffer.cc"

using namespace std;

//Generates a vector containing expected results
vector<string> generateExpectedResult(int size)
{
	vector<string> exp_res;
	string temp;
	for(int i = 0; i < size; i ++)
	{
		std::ostringstream ss;
	    ss << i;
		temp = ss.str();
		exp_res.push_back(temp);
	}
	return exp_res;
}


vector<string> read(PriorityBuffer* buffer)
{
	cout << "[READ] Begin reading all elements in buffer." << endl;
	vector<string> result;
	int size = buffer->size();
	cout << "[READ] Elements in buffer: " << size << endl;
	for(int i = 0; i < size; i ++)
	{
		result.push_back( buffer->dequeue() );
		//cout << "[READ] Pushed element " << result[i] << " onto the vector at position " << i << endl;
	}
	cout << "[READ] Finished reading elements." << endl;
	return result;
}


int checkResult(vector<string> expected, vector<string> got)
{
	cout << "[CHK] Begin checking." << endl;
	int passed_tests = 0;
	//cout << "[CHK] Sizes are: expected = "<< expected.size() << " and got = " << got.size() << endl;
	if(expected.size() == got.size())
	{
		for(int i = 0; i < expected.size(); i ++)
		{
			//cout << "[CHK] Checking if " << expected[i] << " is the same as " << got[i] << endl;
			if( !expected[i].compare(got[i]) )
			{
				passed_tests++;
			}
		}
	}
	cout << "[CHK] Passed tests: " << passed_tests << endl;
	//not the same size, then there are no matches.
	return passed_tests;
}

PriorityBuffer* basicFIFOEnqueue(vector<string> toEnqueue)
{
	PriorityBuffer* temp = new PriorityBuffer(1);
	cout << "[BFEQ] Begin Basic FIFO enqueueing." << endl;
	for(int i = 0; i < toEnqueue.size(); i++)
	{
		temp->enqueue(0, toEnqueue[i]);
		//cout << "[BFE] Enqueueing element " << i << " at Priority 0" << endl;
	}
	cout << "[BFEQ] Done!" << endl;
	return temp;
}


PriorityBuffer* twoQueueEnqueue(vector<string> toEnqueue)
{
	PriorityBuffer* temp = new PriorityBuffer(2);
	cout << "[2QEQ] Begin Basic FIFO enqueueing." << endl;
	for(int i = 0; i < floor(toEnqueue.size()/2.0); i++)
	{
		temp->enqueue(0, toEnqueue[i]);
		//cout << "[2QEQ] Enqueueing element " << i << " at Priority 0" << endl;
	}
	for(int i = ceil(toEnqueue.size()/2.0); i < toEnqueue.size(); i++)
	{
		temp->enqueue(1, toEnqueue[i]);
		//cout << "[2QEQ] Enqueueing element " << i << " at Priority 1" << endl;
	}
	cout << "[2QEQ] Done!" << endl;
	return temp;
}


PriorityBuffer* fiveQueueEnqueue(vector<string> toEnqueue)
{
	PriorityBuffer* temp = new PriorityBuffer(5);
	cout << "[5QEQ] Begin Basic FIFO enqueueing." << endl;
	int modifier = floor(toEnqueue.size()/5.0);
	for(int i = 0; i < 5; i++)
	{
		for(int j = i*modifier; j < (i+1)*modifier; j++)
		{
			temp->enqueue(i, toEnqueue[j]);
			//cout << "[5QEQ] Enqueueing element " << j << " at Priority " << i << endl;
		}
	}
	cout << "[5QEQ] Done!" << endl;
	return temp;
}


int main()
{

	cout << "Initialized unit test of PriorityBuffer..." << endl<<endl;
	
	//initialize variables
	int n = 20;
	int n_o_tests = 0;
	int failed_tests = 0;
	int subtests;
	string error_messages = "";
	vector<string> expected, result;
	
	
	cout << "Tests are of size " << n << endl<<endl;
	
	
	
	cout << "Generating expected result vector of size " << n << "... ";
	expected = generateExpectedResult(n);
	cout << "Done!" << endl<<endl;
	
	cout << "Created testing queue handler... ";
	PriorityBuffer* testing;
	cout << "Done!" << endl<<endl;
	
	//Test 1: If FIFO holds for a single queue.
	cout << "Initializing basic FIFO enqueueing test..." << endl;
	testing = basicFIFOEnqueue(expected);
	cout << "Dequeueing buffer..." << endl;
	result = read(testing);
	cout << "Checking equality..." << endl;
	subtests = checkResult(expected, result);
	cout << "Matched " << subtests << " of " << n << " values." << endl;
	if(subtests != n)
	{
		failed_tests++;
		error_messages += "[BFEQ] Failed to match up to expected results!\n";
	}
	n_o_tests++;
	cout << "Basic FIFO enqueueing test done!" << endl<<endl;
	
	
	//Test 2: If dequeueing holds when half of the values are in different queues.
	cout << "Initializing two-queue enqueueing test..." << endl;
	testing = twoQueueEnqueue(expected);
	cout << "Dequeueing buffer..." << endl;
	result = read(testing);
	cout << "Checking equality..." << endl;
	subtests = checkResult(expected, result);
	cout << "Matched " << subtests << " of " << n << " values." << endl;
	if(subtests != n)
	{
		failed_tests++;
		error_messages += "[2QEQ] Failed to match up to expected results!\n";
	}
	n_o_tests++;
	cout << "Two-queue enqueueing test done!" << endl<<endl;
	
	
	//Test 3: If dequeueing holds when the values are split among five queues.
	cout << "Initializing five-queue enqueueing test..." << endl;
	testing = fiveQueueEnqueue(expected);
	cout << "Dequeueing buffer..." << endl;
	result = read(testing);
	cout << "Checking equality..." << endl;
	subtests = checkResult(expected, result);
	cout << "Matched " << subtests << " of " << n << " values." << endl;
	if(subtests != n)
	{
		failed_tests++;
		error_messages += "[5QEQ] Failed to match up to expected results!\n";
	}
	n_o_tests++;
	cout << "Five-queue enqueueing test done!" << endl<<endl;
	
	
	
	cout<< "Test finished!" << endl;
	cout << endl << "Passed " << n_o_tests-failed_tests << " out of " << n_o_tests << " tests." << endl;
	if(failed_tests != 0)
	{
		cout << "[ERRORS:]" << endl << error_messages;
	}
	return 0;
}
