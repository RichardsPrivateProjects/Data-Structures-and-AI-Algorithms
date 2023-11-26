#include <iostream>

#pragma once

#include "SListNode.h"

template <typename SLinkedList>
class SListIterator : public std::iterator<std::forward_iterator_tag, SLinkedList> 
{ 
public:
	using ValueType = typename SLinkedList::ValueType;
	using Node = SListNode<ValueType>;
public:
    SListIterator(Node* t_initialNode = nullptr) 
	{
        m_current = t_initialNode; 
    }
        
    ValueType & operator *()
	{ 
	    return (*m_current).element();  
    } 
    
	Node* get() 
	{
		return m_current;
	}

	// Prefix ++
	SListIterator& operator ++()
	{
		m_current = m_current->next().get();
		return *this;
	}

	// Postfix ++	
	SListIterator operator ++(int)
	{
		SListIterator original(m_current);
		m_current = m_current->next().get();
		return original;
	}

	bool operator ==(SListIterator & t_other) const
	{
		return m_current == t_other.m_current;
	}

	bool operator !=(SListIterator & t_other) const
	{
		return m_current != t_other.m_current;
	}
	
	ValueType& operator [](int t_index)
	{
		for (int i = 0; i < t_index; i++)
		{
			m_current = m_current->next().get();
		}
		return m_current->element();
	}
	
private:
	Node* m_current;
};



