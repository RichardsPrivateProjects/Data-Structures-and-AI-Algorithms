#include <iostream>
#include <memory> // for std::unique_ptr

// Program to demonstrate basics of std::unique_ptr using a Node class

// A Node class that can only handle integers
class Node
{
public:
	Node(int t_val) : m_val(t_val) {}

	void setNext(std::unique_ptr<Node>& t_next)
	{
		m_next.swap(t_next);
	}

	std::unique_ptr<Node>& next()
	{
		return m_next;
	}

	int element() const
	{
		return m_val;
	}

private:
	int m_val;
	std::unique_ptr<Node> m_next;
};

//-----------------------------------------------------------
// Creates a new node with value t_param and adds it to the 
//  end of the list. The function returns a pointer
//  to the new tail of the list.
//-----------------------------------------------------------
Node* append(Node* t_tail, int t_param)
{
	/* 
	* Sample solution to exercise below
	*/

	// Make a new node with value t_param
	std::unique_ptr<Node> temp =
		std::make_unique<Node>(t_param);
	// Point tail at new node
	t_tail->setNext(temp);
	// The new node has been appended to the list, so update the tail pointer.
	t_tail = t_tail->next().get();
	return t_tail;
}

//-----------------------------------------------------------
//  New node with value t_param is inserted into the list, where the 
//  list elements are maintained in ascending sequence.
//-----------------------------------------------------------
void insert(Node* t_head, int t_param)
{
	// Example: if the list contains:
	// 1->3->5
	// Then insert(4) will result in the following list: 
	// 1->3->4->5
	Node* current = t_head;
	Node* previous = t_head;

	for (; current != nullptr && current->element() < t_param; current = current->next().get())
	{
		previous = current;
	}
	if (current == nullptr)
	{
		append(previous, t_param);
	}
	if (current != nullptr)
	{
		std::unique_ptr<Node> temp = std::make_unique<Node>(t_param);
		append(temp.get(), current->element());
		previous->setNext(temp);
	}

}


int main2()
{
	//-----------------------------------------------------------
	// Dynamic creation of a Node object with std::make_unique
	std::unique_ptr<Node> n1 = std::make_unique<Node>(1);
	std::unique_ptr<Node> n2;
	// Assignment from n2 to n1 not possible, but we can swap
	n1.swap(n2);		// Note n1 is now empty.

	//-----------------------------------------------------------
	// Make a list and point n1 at n2
	n1 = std::make_unique<Node>(1);
	n2 = std::make_unique<Node>(2);
	n1->setNext(n2);	// Note that Node::setNext() implementation calls swap

	//-----------------------------------------------------------
	// Find the tail
	Node* tail = n1->next().get();
	// Call append
	tail = append(tail, 100);
	tail = append(tail, 200);

	// Output the list contents
	// Point at the first node
	Node* temp = n1.get();
	// Walk the list
	while (temp != nullptr)
	{
		// Print the node data
		std::cout << temp->element() << "\n";
		// Go to the next node
		temp = temp->next().get();
	}

	//-----------------------------------------------------------
	// Some miscellaneous notes
	// To get the successor of node n1 (note: variable successor *must* be a reference)
	std::unique_ptr<Node>& successor = n1->next();

	// With references - can be initialised only, can never be reassigned
	std::unique_ptr<Node>& nodeRef = n1;
	// Reference cannot be reassigned
	// nodeRef = n2;

	// We can get a raw pointer from a std::unique_ptr like this:
	Node* first = n1.get();

	//Insert
	std::unique_ptr<Node> n4 = std::make_unique<Node>(1);
	std::unique_ptr<Node> n5 = std::make_unique<Node>(3);

	n4->setNext(n5);
	insert(n4.get(), 5);
	insert(n4.get(), 4);
	insert(n4.get(), 7);


	
	return 1;
}
