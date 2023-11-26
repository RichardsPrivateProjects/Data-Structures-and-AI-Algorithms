#include "SLinkedList.h"
#include <unordered_map>

// Need to repeat the alias template here.
template <typename T>
using Iterator = SListIterator<SLinkedList<T>>;

// Need to use explicit template instantiation here too 
//  because template member functions are defined in a separate file to the class.
template class SLinkedList<int>;
template class SLinkedList<char>;

template <typename T>
SLinkedList<T>::SLinkedList() :
	m_head(nullptr),
	m_tail(nullptr),
	m_count(0)
{}

template <typename T>
int SLinkedList<T>::size() const
{
	return m_count;
}

template <typename T>
bool SLinkedList<T>::isEmpty() const
{
	return m_count == 0;
}

template <typename T>
void SLinkedList<T>::swap(Iterator& t_first, Iterator& t_second)
{
	T temp = *t_first;
	*t_first = *t_second;
	*t_second = temp;
}

template <typename T>
void SLinkedList<T>::replace(Iterator& t_position, T t_element)
{
	*t_position = t_element;
}

template <typename T>
void SLinkedList<T>::insertFirst(T t_element)
{
	// create the new node.		
	std::unique_ptr<SListNode<T>> newNode = std::make_unique<SListNode<T>>(t_element, this);
	newNode->setNext(m_head);
	// set the head node, and the tail node if needed.
	std::swap(m_head, newNode);

	if (m_tail == nullptr)
	{
		m_tail = m_head.get();
	}

	m_count++;
}

template <typename T>
void SLinkedList<T>::insertLast(T t_element)
{

	std::unique_ptr<SListNode<T>> newNode = std::make_unique<SListNode<T>>(t_element, this);
	//Tail is just a raw pointer
	SListNode<T>* current = m_head.get();
	//We need to iterate through the list to get the tails previous
	while ( current != m_tail )
	{
		current = current->next().get();
	}
	if (current != nullptr)
	{
		current->setNext(newNode);
		m_tail = current->next().get();
		m_count++;
	}

}

template <typename T>
void SLinkedList<T>::insertAfter(Iterator& t_position, T t_element)
{
	SListNode<T>* temp = t_position.get();

	if (temp != nullptr)
	{
		std::unique_ptr<SListNode<T>> newNode = std::make_unique<SListNode<T>>(t_element, this);

		newNode->setNext(temp->next());
		// if the new node is the new end of list node
		if (newNode->next() == nullptr)
		{
			m_tail = newNode.get();
		}
		temp->setNext(newNode);	// newNode is now nullptr

		m_count++;
	}
}

template <typename T>
void SLinkedList<T>::insertBefore(Iterator& t_position, T t_element)
{
	SListNode<T>* temp = t_position.get();

	if (temp != nullptr)
	{
		std::unique_ptr<SListNode<T>> newNode = std::make_unique<SListNode<T>>(t_element, this);

		SListNode<T>* current = this->head();
		SListNode<T>* previous = current;

		for (; current != nullptr && current->element() != temp->element(); 
			current = current->next().get())
		{
			previous = current;
		}
		SLinkedList<T>::Iterator it = Iterator(previous);
		insertAfter(it, t_element);
		m_count++;
	}


}


template <typename T>
Iterator<T> SLinkedList<T>::remove(Iterator& t_position)
{
	SListNode<T>* node = t_position.get();

	// Advance the iterator position to the next node
	++t_position;

	// If the node to be removed is at the head of the list
	if (m_head.get() == node)
	{
		m_head = std::move(m_head->next());
	}
	else
	{ // Node to be deleted has a predecessor.
		SListNode<T>* previous = m_head.get();

		// Set up 'previous' to point to the node one before the node we wish to delete.
		for (; previous->next().get() != node; previous = previous->next().get());

		// If the node to be deleted is the tail.
		if (node == m_tail)
		{
			m_tail = previous;
		}
		// Get a (unique_ptr) reference to the node to be deleted. 
		std::unique_ptr<SListNode<T>>& deleteMe = previous->next();
		// Link to the node after the one to be deleted.
		deleteMe = std::move(deleteMe->next());
	}

	m_count--;

	// Return the new iterator position
	return t_position;
}


template <typename T>
int SLinkedList<T>::remove(T t_element)
{
	// Remove all occurrences of element
	SListNode<T>* current = m_head.get();
	SListNode<T>* temp;

	int matches = 0;
	while (current != 0)
	{
		// if we have found a match
		if (current->element() == t_element)
		{
			std::cout << "Found a match" << std::endl;
			// Remove the match we have found
			temp = current;
			current = current->next().get();
			Iterator iterToRemove(temp);
			remove(iterToRemove);
			matches++;
		}
		else
		{
			current = current->next().get();
		}
	}
	m_count--;
	return matches;
}

template<typename T>
void SLinkedList<T>::moveLastToFront()
{
	SListNode<T>* secondLast = m_head.get();
	SListNode<T>* current = m_head.get();

	for (; current != nullptr && current->element() != m_tail->element(); current = current->next().get())
	{
		secondLast = current;
	}

	std::unique_ptr<SListNode<T>> oldHead = std::move(m_head);
	m_head = std::move(secondLast->next());

	
	std::unique_ptr<SListNode<T>> tempNext = std::move(oldHead);
	m_head->setNext(tempNext);  
	m_tail = secondLast;

	if (!m_tail)
		m_tail = current;
}

template<typename T>
void SLinkedList<T>::reverse()
{

	std::unique_ptr<SListNode<T>> previous = nullptr;

	std::unique_ptr<SListNode<T>> current = std::move(m_head);

	std::unique_ptr<SListNode<T>> next = nullptr;

	while (current != nullptr)
	{
		next = std::move(current->next());
		current->next() = std::move(previous);
		if (current->next() == nullptr)
		{
			m_tail = current.get();
		}
		previous = std::move(current);
		current = std::move(next);

	}

	m_head = std::move(previous);

	// update head pointer here
}

template<typename T>
void SLinkedList<T>::makeNewHead(Iterator t_position)
{
	SListNode<T>* temp = t_position.get();
	SListNode<T>* tempHead = m_head.get();
	SListNode<T>* previous = m_head.get();

	for (; previous->next()->element() != temp->element(); previous = previous->next().get())
	{

	}
	//End of loop
	previous->next()->setNext(m_head->next());
	previous->setNext(m_head);
}

template<typename T>
void SLinkedList<T>::unique()
{
	std::unordered_map<T, int> elementCount;

	SLinkedList<T>::Iterator it = m_head.get();
	SLinkedList<T>::Iterator itEnd = end();

	for (; it != itEnd;)
	{
		elementCount[*it]++;
		if (elementCount[*it] > 1)
		{
			elementCount[*it]--;
			it = remove(it);		
		}
		else
		{
			it++;
		}
	}
	int lol = 56;

}


template <typename T>
Iterator<T> SLinkedList<T>::last() const
{
	if (m_tail != nullptr)
	{
		return Iterator(m_tail);
	}
	return this->end();
}


template <typename T>
SListNode<T>* SLinkedList<T>::head() const
{
	return m_head.get();
}

template <typename T>
SListNode<T>* SLinkedList<T>::tail() const
{
	return m_tail;
}

template <typename T>
Iterator<T> SLinkedList<T>::begin() const
{
	Iterator it(m_head.get());
	return it;
}

template <typename T>
Iterator<T> SLinkedList<T>::end() const
{
	// Return a "one past the end" iterator  
	SListNode<T>* temp = nullptr;

	Iterator it(temp);
	return it;
}

