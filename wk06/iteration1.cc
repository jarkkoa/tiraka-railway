/**
 * iteration1.cc
 *
 * Print all items of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include <iostream>
#include <iterator> // for iterators
#include <list> // for lists
using namespace std;


void printAllItems(const list<int>& lst)
{
  /**
  * Use iterators to go through the list and print elements
  */

  // ADD YOUR CODE HERE

  auto it = lst.begin();

  while (distance(it, lst.end()) > 0)
  {
    cout << *it++ << " ";
  }
  cout << endl;
}
