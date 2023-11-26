#pragma once

#include <memory>	// for std::unique_ptr

#include "List.h"
#include "SListNode.h"
#include "SListIterator.h"

// First the class interface
template <typename T>
class SLinkedList : public List<T> 
{
public:
	using ValueType = T;	
	using Iterator = SListIterator<SLinkedList<T>>;
public:        
               
    SLinkedList();           

    int size() const override;
    bool isEmpty() const override;
	void swap(Iterator & t_first, Iterator & t_second) override;
    void replace(Iterator & t_position, T t_element) override;        
    void insertFirst(T t_element) override;
    void insertLast(T t_element) override;
    void insertAfter(Iterator & t_position, T t_element) override;  	
	void insertBefore(Iterator & t_position, T t_element) override;
	Iterator remove(Iterator & t_position) override;
    int remove(T t_element) override;

    void moveLastToFront();
    void reverse();
    void makeNewHead(Iterator t_position);
    void unique();
	
    // Member functions specific to SLinkedList
    SListNode<T>* head() const;
    SListNode<T>* tail() const;
    
	Iterator begin() const;
	Iterator end() const;
	Iterator last() const;
	
    
private:
    std::unique_ptr<SListNode<T>> m_head;
	SListNode<T>* m_tail;
    int m_count;
};

