#pragma once

#include "Container.h"

// Forward references.
template <typename SLinkedList> class SListIterator;
template <typename T> class SLinkedList;


template <typename T>
class Sequence : public Container<T> 
{
public:
	using Iterator = SListIterator<SLinkedList<T>>;
public:       
    virtual void swap(Iterator & t_first, Iterator & t_second) = 0;
	virtual void replace(Iterator & t_position, T t_element) = 0;
};



