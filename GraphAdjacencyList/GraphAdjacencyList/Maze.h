#pragma once
#include <SFML/Graphics.hpp>
#include "Graph_AdjacencyList.h"
#include "NodeData.h"

typedef GraphArc<NodeData, int> Arc;
typedef GraphNode<NodeData, int> Node;

enum class PathfindingAlgorithm
{
	None,
	DepthFirstSearch,
	BreadthFirstSearch,
	AStar
};

class Maze
{
public:
	Maze();
	~Maze();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void setupMaze();
	void setupMazeBorder();

	void createNodes();
	void createArcs();
	void createGraph();
	void setupMazeCells();

	void visitMazeCell(Node* node);
	void displayPath(Node* goalNode);

	void searchPath(Node* t_start, Node* t_goal, PathfindingAlgorithm t_algorithmType);
	void depthFirstSearch(Node* t_start, Node* t_goal);
	void breadthFirstSearch(Node* t_start, Node* t_goal);
	void aStarSearch(Node* t_start, Node* t_goal);
	
	void setupFontAndText();
	void setupSprite();

	const static int MAX_COLS = 12;
	const static int MAX_ROWS = 20;
	const static int MAX_CELL_INDEX = MAX_ROWS * MAX_COLS;

	Graph_AdjacencyList<NodeData, int> m_graph{ MAX_COLS * MAX_ROWS};

	sf::RectangleShape m_mazeCells[MAX_CELL_INDEX];
	sf::Vector2f m_cellSize = { 40,40 };
	sf::VertexArray m_mazeBorder{sf::LinesStrip};
	sf::VertexArray m_mazeLines{sf::Lines};
	sf::Color mazeWallColor{ sf::Color::Black };
	sf::Color visitColor{0, 255, 255};
	sf::Color pathColor{sf::Color::Green};

	sf::Time traversalSpeed = sf::seconds(0.2);
	sf::Clock timeSinceTraversal;

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	bool m_exitGame; // control exiting game

};
