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


    // Is the new node already a neighboring node's neighbor? (= already created)
    for (const auto &node : paths)
    {
        for (const auto &neighbor : node.second->neighbors)
        {
            if (neighbor.second->location == loc)
            {
                return neighbor.second;
            }
        }
    }

    // Otherwise create a new node
    Node* newNode = new Node;
    newNode->location = loc;
    newNode->neighbors = paths;

    return newNode;
}
