
#include <iostream>
#include <fstream>
#include <string>
#include "Graph_EdgeList.h"
#include "NodeData.h"

typedef GraphArc<NodeData, int> Arc;
typedef GraphNode<NodeData, int> Node;

void visit( Node * t_node ) 
{
	std::cout << "Visiting: " << t_node->m_data.m_name << std::endl;
}

int main() 
{

	// Create a graph with capacity for 25 nodes.
	// Templates parameters are NodeType (std::string), ArcType (int)
    Graph_EdgeList<NodeData, int> graph( 25 );
	
	NodeData nodeData;
	int nodeIndex = 0;
	std::ifstream myfile;

	// Create each node, and assign a name (nodeLabel) from the text file
	myfile.open ("nodes.txt");
	while ( myfile >> nodeData.m_name ) 
	{
		graph.addNode(nodeData, nodeIndex++);
	}

	myfile.close();

	// Wire up the graph by creating arcs between nodes.
	myfile.open("arcs.txt");
	int from, to, weight;
	while ( myfile >> from >> to >> weight ) 
	{
		graph.addArc(from, to, weight);
	}
    myfile.close();


	Arc* arc = graph.getArc(17, 18);

	//Remove node
	graph.removeNode(17);



	
}
