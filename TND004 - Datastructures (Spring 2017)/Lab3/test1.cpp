#include <iostream>
#include <vector>

#include "BinarySearchTree.h"

using namespace std;

// Test program 1: get_parent
int main( )
{
    BinarySearchTree<int> t1;

    vector<int> V = {20, 10, 30, 5, 15, 35, 25, 12, 14, 33};

    /**************************************/
    cout << "PHASE 0: insert\n\n";
    /**************************************/

    for(auto j: V)
        t1.insert( j );

    //Display the tree
    cout << "T1" << endl;
    t1.printTree( );
    cout << endl;

    /**************************************/
    cout << "\nPHASE 1: findMin, FindMax\n\n";
    /**************************************/

    cout << "Min = " << t1.findMin( ) << endl;
    cout << "Max = " << t1.findMax( ) << endl;

    /**************************************/
    cout << "\nPHASE 2: contains\n\n";
    /**************************************/

    for(int i = 2; i < 36; i+=2 )
    {
        if ( t1.contains(i) )
            cout << i << " in the tree" << endl;
        else
            cout << i << " not in the tree" << endl;
    }

    /**************************************/
    cout << "\nPHASE 3: copy constructor\n\n";
    /**************************************/

    BinarySearchTree<int> t2(t1);

    //Display the tree
    cout << "T2" << endl;
    t2.printTree( );
    cout << endl;

    /**************************************/
    cout << "\nPHASE 4: makeEmpty\n\n";
    /**************************************/

    t2.makeEmpty();

    //Display the tree
    cout << "T2" << endl;
    t2.printTree( );
    cout << endl;

    /**************************************/
    cout << "\nPHASE 5: operator=\n\n";
    /**************************************/

    t2 = t1;

    //Display the tree
    cout << "T2" << endl;
    t2.printTree( );
    cout << endl << endl;

    /**************************************/
    cout << "\nPHASE 6: get_parent\n\n";
    /**************************************/
    cout << "Parent of node 14: "
         << t2.get_parent(14) << endl;

    cout << "Parent of node 10: "
         << t2.get_parent(10) << endl;

    cout << "Parent of node 33: "
         << t2.get_parent(33) << endl;

    cout << "Parent of node 20: "
         << t2.get_parent(20) << endl;

    cout << "Parent of node 28: "
         << t2.get_parent(28) << endl;


    /**************************************/
    cout << "\nPHASE 7: remove\n\n";
    /**************************************/

    cout << "\nT1" << endl;
    t1.printTree( );
    cout << endl;
    for(int i = 0; i < 35; i+= 2 )
    {
        cout << "remove " << i << endl;
        t1.remove( i );
    }

    //Display the tree
    cout << "\nT1" << endl;
    t1.printTree( );
    cout << endl;

    cout << "\nFinished testing" << endl;

    return 0;
}
