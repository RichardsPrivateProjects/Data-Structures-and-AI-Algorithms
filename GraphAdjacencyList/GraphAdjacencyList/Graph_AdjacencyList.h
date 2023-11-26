#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <functional>


template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

// ---------------------------------------------------------------------
//  Name:           Graph
//  Description:    Manages the nodes and connections (arc) between them
// ---------------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph_AdjacencyList 
{
public:        
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

    // Constructor and destructor functions
    Graph_AdjacencyList( int size );
    ~Graph_AdjacencyList();

    // Accessors
    Node * nodeIndex(int index) const 
	{
		return m_nodes.at(index);
    }

    // Public member functions.
    bool addNode( NodeType data, int index );
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
    
    void depthFirst( Node* node, std::function<void(Node *)> f_visit);

    // To be completed
    void depthFirstNonRec(Node* node, std::function<void(Node*)> f_visit);
    void depthFirstNonRec2(Node* node, std::function<void(Node*)> f_visit);

    void breadthFirst( Node* node, std::function<void(Node *)> f_visit);

    // To be completed
	void adaptedBreadthFirst( Node* current, Node* goal, std::function<void(Node*)> f_visit);
 
    // To be completed
    void adaptedDepthFirst(Node* current, Node* goal, std::function<void(Node*)> f_visit);

    void printPath(Node* goal);

private:
	

	// ----------------------------------------------------------------
	//  Description:    A container of all the nodes in the graph.
	// ----------------------------------------------------------------

	std::vector<Node*> m_nodes;

};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph_AdjacencyList<NodeType, ArcType>::Graph_AdjacencyList( int maxNodes ) : m_nodes( maxNodes, nullptr) 
{  
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph_AdjacencyList<NodeType, ArcType>::~Graph_AdjacencyList() {
   for( int index = 0; index < m_nodes.size(); index++ ) 
   {
        if( m_nodes[index] != nullptr ) 
		{
			delete m_nodes.at(index);
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
bool Graph_AdjacencyList<NodeType, ArcType>::addNode( NodeType data, int index ) 
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
void Graph_AdjacencyList<NodeType, ArcType>::removeNode( int index ) 
{
     // Only proceed if node does exist.
     if( nullptr != m_nodes.at(index) ) 
	 {
         // now find every arc that points to the node that
         // is being removed and remove it.        
         Arc* arc;

         // loop through every node
         for( int node = 0; node < m_nodes.size(); node++ ) 
		 {
              // if the node is valid...
              if( nullptr != m_nodes.at(node) ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_nodes.at(node)->getArc(m_nodes.at(index) );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_nodes.at(index);
		m_nodes.at(index) = nullptr;       
    }
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
bool Graph_AdjacencyList<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) 
{
     bool proceed = true; 
     // make sure both nodes exist.
     if( nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to) ) 
	 {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_nodes.at(from)->getArc( m_nodes.at(to) ) != nullptr ) 
	 {
         proceed = false;
     }

     if (proceed == true) 
	 {
        // add the arc to the "from" node.
		 m_nodes.at(from)->addArc(m_nodes.at(to), weight );
     }
        
     return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph_AdjacencyList<NodeType, ArcType>::removeArc( int from, int to ) 
{
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to) ) 
	 {
         nodeExists = false;
     }

     if (nodeExists == true) 
	 {
        // remove the arc.
		 m_nodes.at(from)->removeArc(m_nodes.at(to) );
     }
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph_AdjacencyList<NodeType, ArcType>::getArc( int from, int to ) 
{
     Arc* arc = 0;
     // make sure the to and from nodes exist
     if( nullptr != m_nodes.at(from) && nullptr != m_nodes.at(to) ) 
	 {
         arc = m_nodes.at(from)->getArc(m_nodes.at(to) );
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
void Graph_AdjacencyList<NodeType, ArcType>::clearMarks() 
{
     for( int index = 0; index < m_nodes.size(); index++ ) 
	 {
          if( nullptr != m_nodes.at(index) ) 
		  {
			  m_nodes.at(index)->setMarked(false);
          }
     }
}


// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph_AdjacencyList<NodeType, ArcType>::depthFirst( Node* node, std::function<void(Node *)> f_visit ) 
{
     if( nullptr != node ) 
     {
           // process the current node and mark it
           f_visit( node );
           node->setMarked(true);

           // go through each connecting node
           auto iter = node->arcList().begin();
           auto endIter = node->arcList().end();
        
		   for( ; iter != endIter; ++iter) 
		   {
			    // process the linked node if it isn't already marked.
                if ( (*iter).destNode()->marked() == false ) 
				{
                   depthFirst( (*iter).destNode(), f_visit);
                }            
           }
     }
}

template<class NodeType, class ArcType>
inline void Graph_AdjacencyList<NodeType, ArcType>::depthFirstNonRec(Node* node, std::function<void(Node*)> f_visit)
{
    std::stack<Node*> nodeStack;
    if (nullptr != node)
    {
        f_visit(node);
        node->setMarked(true);   

        nodeStack.push(node);
        while (nodeStack.empty() == false)
        {
            auto iter = nodeStack.top()->arcList().begin();
            auto endIter = nodeStack.top()->arcList().end();
            for (; iter != endIter;)
            {
                if ((*iter).destNode()->marked() == false)
                {
                    (*iter).destNode()->setMarked(true);
                    nodeStack.push((*iter).destNode());
                    f_visit(nodeStack.top());
                    iter = nodeStack.top()->arcList().begin();
                    endIter = nodeStack.top()->arcList().end();
                }
                else
                {
                    iter++;
                }
            }
            
            nodeStack.pop();
         }
    }
}

template<class NodeType, class ArcType>
inline void Graph_AdjacencyList<NodeType, ArcType>::depthFirstNonRec2(Node* node, std::function<void(Node*)> f_visit)
{
    if (node == nullptr)
    {
        return;
    }

    std::stack<Node*> nodeStack;
    nodeStack.push(node);

    while (!nodeStack.empty())
    {
        Node* currentNode = nodeStack.top();
        nodeStack.pop();

        if (!currentNode->marked())
        {
            
            currentNode->setMarked(true);

            for (const auto& arc : currentNode->arcList())
            {
                Node* neighborNode = arc.destNode();
                if (!neighborNode->marked())
                {
                    nodeStack.push(neighborNode);
                    f_visit(neighborNode);
                }
            }
        }
    }
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a breadth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph_AdjacencyList<NodeType, ArcType>::breadthFirst( Node* node, std::function<void(Node *)> f_visit) 
{
   if( nullptr != node ) 
   {
	  std::queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( node );
      node->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) 
	  {
         // process the node at the front of the queue.
		 f_visit( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         auto iter = nodeQueue.front()->arcList().begin();
         auto endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) 
		 {
              if ( (*iter).destNode()->marked() == false) 
			  {
				 // mark the node and add it to the queue.
                 (*iter).destNode()->setMarked(true);
                 nodeQueue.push( (*iter).destNode() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}

template<class NodeType, class ArcType>
inline void Graph_AdjacencyList<NodeType, ArcType>::adaptedBreadthFirst(Node* current, Node* goal, std::function<void(Node*)> f_visit)
{
    if (nullptr != current && nullptr != goal)
    {
        std::queue<Node*> nodeQueue;
        // place the first node on the queue, and mark it.
        nodeQueue.push(current);
        current->setMarked(true);
        f_visit(current);

        // loop through the queue while there are nodes in it.
        while (nodeQueue.size() != 0)
        {

            auto iter = nodeQueue.front()->arcList().begin();
            auto endIter = nodeQueue.front()->arcList().end();

            for (; iter != endIter; iter++)
            {
                if ((*iter).destNode()->marked() == false)
                {
                    f_visit((*iter).destNode());
                    if ((*iter).destNode() == goal)
                    {
                        (*iter).destNode()->setPrevious(nodeQueue.front());
                        return;
                    }
                    (*iter).destNode()->setMarked(true);
                    (*iter).destNode()->setPrevious(nodeQueue.front());;
                    nodeQueue.push((*iter).destNode());
                }
            }

            // dequeue the current node.
            nodeQueue.pop();
        }
    }
}


// ----------------------------------------------------------------
//  Name:           adaptedBreadthFirst
//  Description:    Performs a breadth-first traversal the starting node
//                  specified as an input parameter, terminating at the goal.
//  Arguments:      The first parameter is the starting node.
//                  The second parameter is the goal node.
//  Return Value:   None.
// ----------------------------------------------------------------

template<class NodeType, class ArcType>
void Graph_AdjacencyList<NodeType, ArcType>::adaptedDepthFirst(Node* current, Node* goal, std::function<void(Node*)> f_visit)
{
    std::stack<Node*> nodeStack;
    bool goalFound = false;
    if (nullptr != current && nullptr != goal)
    {;
        current->setMarked(true);
        f_visit(current);
        nodeStack.push(current);
        while (nodeStack.empty() == false && !goalFound)
        {
            auto iter = nodeStack.top()->arcList().begin();
            auto endIter = nodeStack.top()->arcList().end();
            for (; iter != endIter && !goalFound;)
            {
                if ((*iter).destNode()->marked() == false)
                {                
                    f_visit((*iter).destNode());
                    if ((*iter).destNode() == goal)
                    {
                        (*iter).destNode()->setPrevious(nodeStack.top());
                        nodeStack.push((*iter).destNode());
                        goalFound = true;
                        return;
                    }
                    (*iter).destNode()->setMarked(true);
                    (*iter).destNode()->setPrevious(nodeStack.top());
                    nodeStack.push((*iter).destNode());

                    iter = nodeStack.top()->arcList().begin();
                    endIter = nodeStack.top()->arcList().end();
                }
                else
                {
                    iter++;
                }
            }
            if(!goalFound)
            nodeStack.pop();
        }
        
    }
}

template<class NodeType, class ArcType>
inline void Graph_AdjacencyList<NodeType, ArcType>::printPath(Node* goal)
{
    std::stack<Node*> pathStack;
    Node* previous = goal;
    while (previous != nullptr)
    {
        pathStack.push(previous);
        previous = previous->previous();
    }
    std::cout << "Path: ";
    while (!pathStack.empty())
    {
        std::cout << pathStack.top()->m_data.m_name << " -> ";
        pathStack.pop();
    }
}



#include "GraphNode.h"
#include "GraphArc.h"


#endif
