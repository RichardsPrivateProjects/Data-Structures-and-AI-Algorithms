
#include <iostream>
#include <fstream>
#include <string>
#include "Graph_AdjacencyList.h"
#include "NodeData.h"

typedef GraphArc<NodeData, int> Arc;
typedef GraphNode<NodeData, int> Node;

void visit(Node* t_node)
{
	std::cout << "Visiting: " << t_node->m_data.m_name << std::endl;
}

void createGraph(Graph_AdjacencyList<NodeData, int>& graph)
{
	NodeData nodeData;
	int nodeIndex = 0;
	std::ifstream myfile;

	// Create each node, and assign a name (nodeLabel) from the text file
	myfile.open("nodes.txt");
	while (myfile >> nodeData.m_name)
	{
		graph.addNode(nodeData, nodeIndex++);
	}

	myfile.close();

	// Wire up the graph by creating arcs between nodes.
	myfile.open("arcs.txt");
	int from, to, weight;
	while (myfile >> from >> to >> weight)
	{
		graph.addArc(from, to, weight);
	}
	myfile.close();
}

int main()
{
	// Create a graph with capacity for x amount of nodes.
	// Templates parameters are NodeType (std::string), ArcType (int)
	Graph_AdjacencyList<NodeData, int> graph(25);
	createGraph(graph);

	Node* start;

	//Perform a breadth-first traversal of the graph.
	/*start = graph.nodeIndex(0);
	graph.breadthFirst(start, visit);*/

	//adapted breadth first search algorithm.
	/*graph.adaptedBreadthFirst( graph.nodeIndex(0), graph.nodeIndex(15),visit);
	Node *goal = graph.nodeIndex(15);
	graph.printPath(goal);*/


	//adapted Depth First search algorithm
	start = graph.nodeIndex(0);
	Node* end = graph.nodeIndex(19);
	graph.adaptedDepthFirst(start, end, visit);
	graph.printPath(end);

	system("Pause");

}
