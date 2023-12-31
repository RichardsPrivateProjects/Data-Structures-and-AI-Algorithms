#ifndef GRAPHARC_H
#define GRAPHARC_H

#include "GraphNode.h"

// -------------------------------------------------------
// Name:        GraphArc
// Description: This is the arc class. The arc class
//              points to a graph node, and contains a 
//              weight.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphArc 
{
public:    
    
    // Accessor functions
    GraphNode<NodeType, ArcType>* destNode() const
    {
        return m_destNode;
    }
                          
    ArcType weight() const 
	{
        return m_weight;
    }
    
    // Manipulator functions
    void setDestNode(GraphNode<NodeType, ArcType>* t_node) 
	{
       m_destNode = t_node;
    }
    
    void setWeight(ArcType weight) 
	{
       m_weight = weight;
    }
private:
    // -------------------------------------------------------
    // Description: pointer to the node that the arc points to
    // -------------------------------------------------------
    GraphNode<NodeType, ArcType>* m_destNode;

    // -------------------------------------------------------
    // Description: Weight of the arc
    // -------------------------------------------------------
    ArcType m_weight;
};

#endif
