#include <iostream>
#include <list>
#include "SLinkedList.h"

int main() 
{			
	// Sample usage of SLinkedList class. 

	// Create a list with 3 nodes.
	SLinkedList<int> list;	
	list.insertFirst(20);
	list.insertFirst(10);
	list.insertFirst(5);

	SLinkedList<int> newlist;
	newlist.insertFirst(20);
	newlist.insertFirst(15);
	newlist.insertFirst(10);
	newlist.insertFirst(5);
	newlist.insertLast(2);

	SLinkedList<int> secondList;
	secondList.insertFirst(20);
	secondList.insertFirst(10);
	secondList.insertFirst(5);
	
	SLinkedList<int>::Iterator it = list.begin();
	it++;
	it++;
	secondList.insertBefore(it, 15);

	SLinkedList<int> thirdList;
	thirdList.insertFirst(20);
	thirdList.insertFirst(10);
	thirdList.insertFirst(5);

	thirdList.moveLastToFront();

	SLinkedList<int> fourthList;
	fourthList.insertFirst(3);
	fourthList.insertFirst(2);
	fourthList.insertFirst(1);
	fourthList.reverse();

	SLinkedList<int> fifthList;
	fifthList.insertFirst(6);
	fifthList.insertFirst(5);
	it = fifthList.begin();
	fifthList.insertFirst(4);
	fifthList.insertFirst(3);
	fifthList.insertFirst(2);
	fifthList.insertFirst(1);
	fifthList.makeNewHead(it);

	SLinkedList<char> sixthList;
	sixthList.insertFirst('A');
	sixthList.insertFirst('D');
	sixthList.insertFirst('C');
	sixthList.insertFirst('B');
	sixthList.insertFirst('C');
	sixthList.insertFirst('A');
	sixthList.unique();

	std::list<int> intList;
	std::list<int> destinationList;
	intList.push_back(1);
	intList.push_back(3);
	intList.push_back(9);
	intList.push_back(3);
	intList.push_back(10);
	intList.push_back(4);
	intList.push_back(5);
	intList.push_back(6);
	int num1 = 3;
	int num2 = 6;

	auto reverseFromNum1It = std::find(intList.rbegin(), intList.rend(), num1);

	if (reverseFromNum1It != intList.rend())
	{
		//get iterator of 2nd num
		auto toNum2It = std::find(intList.begin(), intList.end(), num2);


		//turn first num1 iterator into a normal forward iterator for splice
		auto fromNum1It = reverseFromNum1It.base();

		if (toNum2It != intList.end())
		{
			destinationList.splice(destinationList.begin(), intList, fromNum1It, toNum2It);
		}
		else
		{
			destinationList.splice(destinationList.begin(), intList, fromNum1It, intList.end());
		}
	}
	//done


	// Get an iterator to list beginning and "one past the end"
	it = list.begin();
	SLinkedList<int>::Iterator itEnd = list.end();
	
	// Simple equality test on two iterators
	bool result = (it != itEnd);

	// Output the value of the first list element
	std::cout << "First element: " << *it << std::endl;

	// Output the number of nodes in the list
	std::cout << list.size() << std::endl;
	
	system("PAUSE");
}

