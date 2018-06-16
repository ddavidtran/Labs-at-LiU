#include <iostream>
#include <vector>

#include "BinarySearchTree.h"

using namespace std;



// Test program 2
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
    cout << "\nPHASE 6: remove\n\n";
    /**************************************/

    for(int i = 0; i < 35; i+= 2 )
    {
        cout << "remove " << i << endl;
        t1.remove( i );
    }

    //Display the tree
    cout << "\nT1" << endl;
    t1.printTree( );
    cout << endl;

    /**************************************/
    cout << "\nPHASE 7: find_pred_succ\n\n";
    /**************************************/

     //Display the tree
    cout << "T2" << endl;
    t2.printTree( );
    cout << endl << endl;

    int low = 15, high = 15;
    t2.find_pred_succ(15, low, high);
    cout << low << " < " << 15 << " < " << high << endl;

    low = 13, high = 13;
    t2.find_pred_succ(13, low, high);
    cout << low << " < " << 13 << " < " << high << endl;

    low = 28, high = 28;
    t2.find_pred_succ(28, low, high);
    cout << low << " < " << 28 << " < " << high << endl;

    low = high = 12;
    t2.find_pred_succ(12, low, high);
    cout << low << " < " << 12 << " < " << high << endl;

    low = high = 35;
    t2.find_pred_succ(35, low, high);
    cout << low << " < " << 35 << " < " << high << endl;

    low = high = 5;
    t2.find_pred_succ(5, low, high);
    cout << low << " < " << 5 << " < " << high << endl;

    cout << "\nFinished testing" << endl;

    return 0;
}
