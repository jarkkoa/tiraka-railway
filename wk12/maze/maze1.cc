#include <iterator>
#include <vector>
#include <algorithm>
#include <set>
#include "maze.hh"

using namespace std;

// Remember to implement your own container here or in another cc file to store the nodes in the maze
// If you define it elsewhere, remember to make sure it's accessible by this file as well.

/**
 * @brief Create a Node object
 * 
 * @param loc  The location of the node
 * @return Node*  A pointer to the node that is created. If the node already exists, return a pointer to the existing node
 */
Node* createNode  (std::pair<int, int> loc, std::map<std::string, Node*> paths) {

    for (const auto &node : maze)
    {
        if (node->location == loc)
        {
            return node;
        }
    }

    // Otherwise create a new node
    Node* newNode = new Node;
    newNode->location = loc;
    newNode->neighbors = paths;

    maze.push_back(newNode);
    return newNode;
}
