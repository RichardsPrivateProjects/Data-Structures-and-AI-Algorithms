#ifndef GRAPHARC_H
#define GRAPHARC_H

#include "GraphNode.h"

// -------------------------------------------------------
// Name:        GraphArc
// Description: This is the arc class. The arc class
//              has a pointer to the origin node and a
//              pointer to the destination node. 
//              It also contains a weight.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphArc 
{
public:    
    
    // Accessor functions
    GraphNode<NodeType, ArcType>* originNode() const 
	{
        return m_originNode;
    }

    GraphNode<NodeType, ArcType>* destNode() const
    {
        return m_destNode;
    }

    ArcType weight() const 
	{
        return m_weight;
    }
    
    // Manipulator functions
    void setOriginNode(GraphNode<NodeType, ArcType>* t_originNode) 
	{
       m_originNode = t_originNode;
    }
   
    void setDestNode(GraphNode<NodeType, ArcType>* t_destNode)
    {
        m_destNode = t_destNode;
    }


    void setWeight(ArcType weight) 
	{
       m_weight = weight;
    }
private:
    // -------------------------------------------------------
    // Description: pointer to the origin node    
    // -------------------------------------------------------
    GraphNode<NodeType, ArcType>* m_originNode { nullptr };

    // -------------------------------------------------------
    // Description: pointer to the destination node    
    // -------------------------------------------------------
    GraphNode<NodeType, ArcType>* m_destNode { nullptr };


    // -------------------------------------------------------
    // Description: Weight of the arc
    // -------------------------------------------------------
    ArcType m_weight;
};

#endif
