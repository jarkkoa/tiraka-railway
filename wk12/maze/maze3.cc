#include <iterator>
#include <vector>
#include <algorithm>
#include "maze.hh"

using namespace std;

// Remember to implement your own container here or in another cc file to store the nodes in the maze
// If you define it elsewhere, remember to make sure it's accessible by this file as well.

/**
 * @brief Connect two adjacent nodes together. If the nodes are not adjacent, do nothing
 * 
 * @param fromNode  The node to connect from
 * @param toNode  The node to connect to
 */
void connectNodes(Node& node1, Node& node2) {

    int xDist = node2.location.first - node1.location.first;
    int yDist = node2.location.second - node1.location.second;

    if (xDist != 0 && yDist != 0)
    {
        return;
    }

    if (xDist == 1)
    {
        node1.neighbors.at(RIGHT) = &node2;
        node2.neighbors.at(LEFT) = &node1;
    }
    else if (xDist == -1)
    {
        node1.neighbors.at(LEFT) = &node2;
        node2.neighbors.at(RIGHT) = &node1;
    }
    else if (yDist == 1)
    {
        node1.neighbors.at(ABOVE) = &node2;
        node2.neighbors.at(BELOW) = &node1;
    }
    else if (yDist == -1)
    {
        node1.neighbors.at(BELOW) = &node2;
        node2.neighbors.at(ABOVE) = &node1;
    }
}
