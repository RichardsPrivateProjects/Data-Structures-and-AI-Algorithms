
#include "Maze.h"
#include <iostream>
#include <fstream>
#include<cstdlib>


/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Maze::Maze() :
	m_window{ sf::VideoMode{ 1920U, 1080U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupMaze();
	
	setupFontAndText(); // load font 
	setupSprite(); // load texture
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Maze::~Maze()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Maze::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Maze::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Maze::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::D == t_event.key.code)
	{
		Node* start = m_graph.nodeIndex(0);
		Node* end = m_graph.nodeIndex(239);
		searchPath(start, end, PathfindingAlgorithm::DepthFirstSearch);
	}
	if (sf::Keyboard::B == t_event.key.code)
	{
		Node* start = m_graph.nodeIndex(0);
		Node* end = m_graph.nodeIndex(239);
		searchPath(start, end, PathfindingAlgorithm::BreadthFirstSearch);
	}
	if (sf::Keyboard::R == t_event.key.code)
	{
		setupMaze();
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Maze::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Maze::render()
{
	m_window.clear(sf::Color::Black);
	for (int i = 0; i < MAX_CELL_INDEX; i++)
	{
		m_window.draw(m_mazeCells[i]);
	}
	m_window.draw(m_mazeLines);
	m_window.draw(m_mazeBorder);
	m_window.display();
}

void Maze::setupMaze()
{
	m_mazeBorder.clear();
	m_mazeLines.clear();
	m_graph = Graph_AdjacencyList<NodeData, int>(MAX_CELL_INDEX);

	setupMazeCells();
	createNodes();
	createArcs();
	createGraph();
	setupMazeBorder();
}

void Maze::setupMazeBorder()
{
	sf::Vertex vertex;
	vertex.color = sf::Color::White;
	//Vertexes setup in anti clockwise order
	//Top Left
	vertex.position = m_mazeCells[0].getPosition();
	m_mazeBorder.append(vertex);
	//Bottom left
	vertex.position = m_mazeCells[(MAX_ROWS - 1) * MAX_COLS].getPosition() + sf::Vector2f{ 0, m_cellSize.y };
	m_mazeBorder.append(vertex);
	//Bottom right
	vertex.position = m_mazeCells[(MAX_ROWS * MAX_COLS) - 1].getPosition() + sf::Vector2f{ m_cellSize.x, m_cellSize.y };
	m_mazeBorder.append(vertex);
	//Top Right
	vertex.position = m_mazeCells[MAX_COLS - 1].getPosition() + sf::Vector2f{ m_cellSize.x, 0 };
	m_mazeBorder.append(vertex);
	//Back to Top Left
	vertex.position = m_mazeCells[0].getPosition();
	m_mazeBorder.append(vertex);
}

void Maze::createNodes()
{
	//0 - 239 nodes
	std::ofstream myFile;

	myFile.open("nodes.txt");
	for (int i = 0; i < MAX_CELL_INDEX; i++)
	{
		myFile << i << "\n";
	}
	myFile.close();
}

void Maze::createArcs()
{
	std::ofstream myArcsFile;

	struct Cell
	{
		int index{ 0 };
		bool visited{ false };
	};

	Cell cells[MAX_CELL_INDEX];
	for (int i = 0; i < MAX_CELL_INDEX; i++)
	{
		cells[i].index = i;
	}
	auto validDirection = [](int t_randomDirection, int col, int row)
		{
			switch (t_randomDirection) //Check invalid directions
			{
			case 0:
				if (col == 0) //we cant go left
					return false;
				break;
			case 1:
				if (col == MAX_COLS - 1) //we cant go right
					return false;
				break;
			case 2:
				if (row == 0) //we cant go up
					return false;
				break;
			case 3:
				if (row == MAX_ROWS - 1) //we cant go down
					return false;
				break;
			default:
				return false;
			}

			return true; //Direction is valid
		};
	auto nextCellInDirection = [](int cell, int direction)
		{
			switch (direction) //Check invalid directions
			{
			case 0: //Going Left
				return cell - 1;
				break;
			case 1: //Going right
				return cell + 1;
				break;
			case 2: //Going up
				return cell - MAX_COLS;
				break;
			case 3: //Going down
				return cell + MAX_COLS;
				break;
			default:
				break;
			}

			return cell;
		};
	auto addWallNextToNeighbour = [this](int direction, int currentCell)
		{
			sf::Vertex vertex; 
			vertex.color = mazeWallColor;
			sf::Vector2f vertexPosOneOffset;
			sf::Vector2f vertexPosTwoOffset;
			switch (direction)
			{
			case 0: //To the left
				vertexPosOneOffset = { 0,0 };
				vertexPosTwoOffset = { 0, m_cellSize.y };
				break;
			case 1: //To the right
				vertexPosOneOffset = { m_cellSize.x, 0 };
				vertexPosTwoOffset = { m_cellSize.x, m_cellSize.y };
				break;
			case 2://Above
				vertexPosOneOffset = { 0, 0 };
				vertexPosTwoOffset = { m_cellSize.x,0 };
				break;
			case 3: //Down
				vertexPosOneOffset = { 0, m_cellSize.y };
				vertexPosTwoOffset = { m_cellSize.x,m_cellSize.y };
				break;
			default:
				break;
			}
			vertex.position = m_mazeCells[currentCell].getPosition() + vertexPosOneOffset;
			m_mazeLines.append(vertex);
			vertex.position = m_mazeCells[currentCell].getPosition() + vertexPosTwoOffset;
			m_mazeLines.append(vertex);
		};
	auto checkNeighbours = [&](int previousCell, int currentCell)
		{
			std::vector<int> neighbours;
			std::vector<int> direction;
			if (currentCell - 1 > 0) { //left neihgbour
				neighbours.push_back(currentCell - 1);
				direction.push_back(0);
			}			
			if (currentCell + 1 < MAX_CELL_INDEX) { //right neighbour
				neighbours.push_back(currentCell + 1);
				direction.push_back(1);
			}
			if (currentCell - MAX_COLS > 0) { //above neighbour
				neighbours.push_back(currentCell - MAX_COLS);
				direction.push_back(2);
			}
			if (currentCell + MAX_COLS < MAX_CELL_INDEX) { //below neighbour
				neighbours.push_back(currentCell + MAX_COLS);
				direction.push_back(3);
			}
			for (int i = 0; i < neighbours.size(); i++)
			{
				if (cells[neighbours[i]].visited == true && cells[neighbours[i]].index != previousCell)
				{
					addWallNextToNeighbour(direction[i], currentCell);
				}
			}
		};
	int cellIndex = std::rand() % MAX_CELL_INDEX;
	int cellsRemaining = MAX_CELL_INDEX;
	int previousCellIndex;
	int randomDirection;
	int row;
	int col;

	cells[cellIndex].visited = true;
	cellsRemaining--;

	myArcsFile.open("arcs.txt");

	while (cellsRemaining > 0)
	{
		col = cellIndex % MAX_COLS;
		row = cellIndex / MAX_COLS;

		randomDirection = std::rand() % 4;
		while (!validDirection(randomDirection, col, row))
		{
			randomDirection = std::rand() % 4;
		}
		previousCellIndex = cellIndex;
		cellIndex = nextCellInDirection(cellIndex, randomDirection);

		if (cells[cellIndex].visited == false)
		{
			cells[cellIndex].visited = true;
			myArcsFile << previousCellIndex << " " << cellIndex << " 0\n";
			myArcsFile << cellIndex << " " << previousCellIndex << " 0\n";
			checkNeighbours(previousCellIndex, cellIndex);
			cellsRemaining--;
		}
		
	}

	myArcsFile.close();
}

void Maze::createGraph()
{
	NodeData nodeData;
	int nodeIndex = 0;
	std::ifstream myFile;

	// Create each node, and assign a name (nodeLabel) from the text file
	myFile.open("nodes.txt");
	while (myFile >> nodeData.m_name)
	{
		m_graph.addNode(nodeData, nodeIndex++);
	}

	myFile.close();

	// Wire up the graph by creating arcs between nodes.
	myFile.open("arcs.txt");
	int from, to, weight;
	while (myFile >> from >> to >> weight)
	{
		m_graph.addArc(from, to, weight);
	}
	myFile.close();
}

void Maze::setupMazeCells()
{
	int cellIndex;
	sf::Vector2f startPos;
	startPos.x = 1920 / MAX_COLS * 4;
	startPos.y = 1080 / MAX_ROWS;
	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLS; col++) {
			//Cells
			cellIndex = row * MAX_COLS + col;
			m_mazeCells[cellIndex].setSize(m_cellSize);
			m_mazeCells[cellIndex].setFillColor(sf::Color::Blue);
			m_mazeCells[cellIndex].setPosition(startPos + sf::Vector2f(col * m_cellSize.x, row * m_cellSize.y));
		}
	}
}

void Maze::searchPath(Node* t_start, Node* t_goal, PathfindingAlgorithm t_algorithmType)
{
	setupMazeCells(); //Just in case we want to switch and compare algorithims visually so we refresh colors;
	m_graph = Graph_AdjacencyList<NodeData, int>(MAX_CELL_INDEX);
	createGraph();

	switch (t_algorithmType)
	{
	case PathfindingAlgorithm::None:

		break;
	case PathfindingAlgorithm::DepthFirstSearch:
		depthFirstSearch(t_start, t_goal);
		break;
	case PathfindingAlgorithm::BreadthFirstSearch:
		breadthFirstSearch(t_start, t_goal);
		break;
	case PathfindingAlgorithm::AStar:
		aStarSearch(t_start, t_goal);
		break;
	default:
		break;
	}
}

void Maze::visitMazeCell(Node* node)
{
	while (timeSinceTraversal.getElapsedTime() < traversalSpeed)
	{

	}//wait

	timeSinceTraversal.restart();
	m_mazeCells[node->m_data.m_name].setFillColor(visitColor);
	render();
}

void Maze::displayPath(Node* goalNode)
{
	Node* previous = goalNode;
	while (previous != nullptr)
	{
		m_mazeCells[previous->m_data.m_name].setFillColor(pathColor);
		previous = previous->previous();
	}
}

void Maze::depthFirstSearch(Node* t_start, Node* t_goal)
{
	auto visitCellLambda = [this](Node* node) {visitMazeCell(node); };

	m_graph.adaptedDepthFirst(t_start, t_goal, visitCellLambda);
	displayPath(t_goal);
}

void Maze::breadthFirstSearch(Node* t_start, Node* t_goal)
{
	auto visitCellLambda = [this](Node* node) {visitMazeCell(node); };

	m_graph.adaptedBreadthFirst(t_start, t_goal, visitCellLambda);
	displayPath(t_goal);
}

void Maze::aStarSearch(Node* t_start, Node* t_goal)
{

}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Maze::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Maze::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
}
