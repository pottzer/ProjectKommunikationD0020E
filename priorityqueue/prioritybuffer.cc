// An example of simple priority queue using linked lists.
// Priority depends on identity number. Small identity number has greater priority.
// If identity numbers are equal. Then FIFO rules are used.

//TODO: Clean out unneccesary packets
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>

//TODO: Create a sensible .h-file for this file.

using namespace std;


class PriorityBuffer
{
	vector< queue<string> > list;

	public:
	//The constructor creates a container and handler
	//for priorty handling. This is done via a number
	//of FIFO queues whom are popped in priority order.
	PriorityBuffer(int number)
	{
		list.reserve(number);
		//Creates a number of queues, in priority order.
		for(int i = 0; i < number; i++)
		{
			queue<string> que;
			list.push_back(que);
		}
	}
	
	//Enqueues a string into the correct queue
	//Returns true if succesful, false otherwise
	bool enqueue(int prio, string data)
	{
		int i;
		for(i = 0; i<= list.size(); i++)
		{
			if(i == prio)
			{
				list[i].push(data);
				return true;
			}
		}
		return false;
	}
	
	
	//dequeues the first string in the priority queue with highest priority
	string dequeue()
	{
		for(int i = 0; i < list.size(); i++)
		{
			if(list[i].size() != 0)
			{
				string t = list[i].front();
				list[i].pop();
				return t;
			}
		}
	}
	
	
	//For the moment, this function is broken. Do no use!
	int size()
	{
		int temp = 0;
		for(int i = 0; i < list.size(); i++)
		{
			temp+=list[i].size();
		}
		return temp;
	}
};






