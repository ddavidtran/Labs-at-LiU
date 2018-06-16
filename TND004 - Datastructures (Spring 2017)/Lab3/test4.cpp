#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

#include "BinarySearchTree.h"

using namespace std;

//Test program 4: BiIterators and remove
int main( )
{
	BinarySearchTree<int> t1;

	vector<int> V = { 8, 3, 10, 1, 6, 14, 4, 7, 13};

	/**************************************/
	cout << "PHASE 0: insert\n\n";
	/**************************************/

	//Create the tree shown in exercise 3 of lab 3
	for(auto j: V)
		t1.insert( j );

	//Display the tree
	cout << "T1" << endl;
	t1.printTree( );
	cout << endl;

	/**************************************/
	cout << "PHASE 3: BiIterator and remove\n\n";
	/**************************************/

	BinarySearchTree<int>::BiIterator it1 = t1.contains(3);
	BinarySearchTree<int>::BiIterator it2 = t1.contains(4);

	cout << "*it1 = " << *it1 << endl;
	cout << "*it2 = " << *it2 << endl;

	cout << "Remove 3 ..." << endl;

	t1.remove(3);

    cout << "T1" << endl;
    t1.printTree( );
    cout << endl;

	cout << "*it1 = " << *it1 << endl; //iterator it1 should be invalidated!
	cout << "*it2 = " << *it2 << endl; //iterator it2 should still point to 4

	cout << "\nFinished testing" << endl;

	return 0;
}
