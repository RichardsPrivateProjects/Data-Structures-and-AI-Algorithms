#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>

// Forward references
template <typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              is a simple container of data.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphNode 
{
public:
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

	// Constructor function
	GraphNode( Node * t_previous = 0 ) : m_previous( t_previous ) {}

   
    bool marked() const 
	{
        return m_marked;
    }

	Node * previous() const 
	{
		return m_previous;
	}

    void setMarked(bool mark) 
	{
        m_marked = mark;
    }
           
	void setPrevious(Node *t_previous) 
	{
		m_previous = t_previous;
	}

   
	// -------------------------------------------------------
	// Description: data stored in each node
	// -------------------------------------------------------
	NodeType m_data;

private:

	// -------------------------------------------------------
	// Description: This remembers if the node is marked.
	// -------------------------------------------------------
	bool m_marked;

	// -------------------------------------------------------
	// Description: pointer to previous node
	// -------------------------------------------------------
	Node* m_previous;
};



#include "GraphArc.h"

#endif
