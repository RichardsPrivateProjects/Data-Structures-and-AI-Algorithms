#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <vector>
#include <functional>


template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

// ---------------------------------------------------------------------
//  Name:           Graph
//  Description:    Manages the nodes and connections (arc) between them
// ---------------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph_EdgeList 
{
public:        
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

    // Constructor and destructor functions
    Graph_EdgeList( int size );
    ~Graph_EdgeList();

    // Accessors
    Node * nodeIndex(int index) const 
	{
		return m_nodes.at(index);
    }

    // Public member functions.
    bool addNode( NodeType data, int index );
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
    
    // Note: Since the type std::list<Arc>::iterator depends on the template parameters 
    //  NodeType, ArcType, we must use the keyword typename below.
    typename std::list<Arc*>::iterator  removeArc( typename std::list<Arc*>::iterator & t_iter );
    Arc* getArc( int from, int to );        
    void clearMarks();
  

private:
	

	// ----------------------------------------------------------------
	//  Description:    A container of all the nodes in the graph.
	// ----------------------------------------------------------------
	std::vector<Node*> m_nodes;


    // ----------------------------------------------------------------
    //  Description:    A container of all the arcs in the graph.
    // ----------------------------------------------------------------
    std::list<Arc*> m_arcs;

};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph_EdgeList<NodeType, ArcType>::Graph_EdgeList( int maxNodes ) 
    : m_nodes( maxNodes, nullptr)
{  
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node and arc
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph_EdgeList<NodeType, ArcType>::~Graph_EdgeList() {
   for( auto node : m_nodes ) 
   {
        if( node != nullptr ) 
		{
			delete node;
        }
   }
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph_EdgeList<NodeType, ArcType>::addNode( NodeType data, int index ) 
{
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( nullptr == m_nodes.at(index) ) 
   {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
	  m_nodes.at(index) = new Node;
	  m_nodes.at(index)->m_data = data;
	  m_nodes.at(index)->setMarked(false);
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph_EdgeList<NodeType, ArcType>::removeNode( int index ) 
{
     // Only proceed if node does exist.
     if( nullptr != m_nodes.at(index) ) 
	 {

         //Loop through the arcs and check if they have a link to the node and remove it.
         for (auto iterator = m_arcs.begin(); iterator != m_arcs.end(); )
         {
             if ((*iterator)->originNode() == m_nodes.at(index) || (*iterator)->destNode() == m_nodes.at(index))
             {
                 iterator = m_arcs.erase(iterator);
             }        
             else
             {
                 iterator++;
             }
         }
     }
        

     // now that every arc referencing the current node has been removed,
     // the node can be deleted.
     delete m_nodes.at(index);
	 m_nodes.at(index) = nullptr; //debug       

}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph_EdgeList<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) 
{
     bool proceed = true; 
     // make sure both nodes exist.
     if( nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to) ) 
	 {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     for (auto arcIter = m_arcs.begin(); proceed == true && arcIter != m_arcs.end(); ++arcIter)
     {
         Arc* arc = *arcIter;
         // If either origin or destination pointers of this arc refer to this node
         //  then this arc must be deleted.
         if (nullptr != arc)
         {
             // if an arc already exists connecting the "from" node to the "to" node
             if (arc->originNode() == m_nodes.at(from) && arc->destNode() == m_nodes.at(to))
             {
                 proceed = false;
             }             
         }
     }

     if (proceed == true) 
	 {
         // Create a new Arc instance, set the origin, destination node pointers and the weight.
         Arc* arc = new Arc();
         arc->setOriginNode(m_nodes.at(from));
         arc->setDestNode(m_nodes.at(to)); 
         arc->setWeight(weight);
         // Append the new arc to the existing list of arcs.
         m_arcs.push_back(arc);
     }
        
     return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      An iterator that references the arc to be removed.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
typename std::list<GraphArc<NodeType, ArcType>*>::iterator  Graph_EdgeList<NodeType, ArcType>::removeArc( typename std::list<Arc*>::iterator & t_iter)
{
    return m_arcs.erase(t_iter);
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or nullptr if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph_EdgeList<NodeType, ArcType>::getArc( int from, int to ) 
{
    Arc* arc = nullptr;

    //Loop through arcs and return the arc that has from and to in its origin and destination
    for (auto iterator = m_arcs.begin(); iterator != m_arcs.end(); iterator++)
    {
        if ((*iterator)->originNode() == m_nodes.at(from) && (*iterator)->destNode() == m_nodes.at(to)) {
            arc = *iterator;
            return arc;
        }
    }
                
     return arc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph_EdgeList<NodeType, ArcType>::clearMarks() 
{
     for( int index = 0; index < m_nodes.size(); index++ ) 
	 {
          if( nullptr != m_nodes.at(index) ) 
		  {
			  m_nodes.at(index)->setMarked(false);
          }
     }
}


#include "GraphNode.h"
#include "GraphArc.h"


#endif
