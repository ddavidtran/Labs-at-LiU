/*********************************************
* file:	~\tnd004\lab\lab4a\digraph.cpp       *
* remark: mplementation of directed graphs   *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>
#include <queue>

using namespace std;

#include "digraph.h"
#include "queue.h"

const int INFINITY = 9999;

// -- CONSTRUCTORS

Digraph::Digraph(int n)
{
    assert(n >= 1);
    array = new List[n + 1];
    dist  = new int [n + 1];
    path  = new int [n + 1];
    done  = new bool[n + 1];
    size  = n;
}

// -- DESTRUCTOR

Digraph::~Digraph()
{
    delete[] array;
    delete[] dist;
    delete[] path;
    delete[] done;
}

// -- MEMBER FUNCTIONS

// insert directed edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Digraph::insertEdge(int u, int v, int w)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].insert(v, w);
}

// remove directed edge (u, v)
void Digraph::removeEdge(int u, int v)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].remove(v);
}

// unweighted single source shortest paths
void Digraph::uwsssp(int s)
{
    if (s < 1 || s > size)
    {
         cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
         return;
    }

    // *** TODO ***


    //First initialize graph.
    initialize(s);

    Queue<int> Q;
    Q.enqueue(s);
    while(!Q.isEmpty()){
        int v = Q.getFront();
        Node* adjacent = array[v].getFirst(); // u = adjacent->vertex
        Q.dequeue();

        while(adjacent){
            if(dist[adjacent->vertex] == INFINITY){
                dist[adjacent->vertex] = dist[v]+1;
                path[adjacent->vertex] = v;
                Q.enqueue(adjacent->vertex);
            }
            adjacent = array[v].getNext();
        }
    }
}

// positive weighted single source shortest pats
void Digraph::pwsssp(int s)
{
    if (s < 1 || s > size)
    {
         cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
         return;
    }

    // *** TODO ***

    initialize(s);
    int v = s; //v = start;

    while(true)
    {
        Node *adjacent = array[v].getFirst();

        while(!adjacent)
        {
            if(!done[adjacent->vertex] && dist[adjacent->vertex] > dist[v] + adjacent->weight) //if u has not been visited and find shortest path to another vertex.
            {
                dist[adjacent->vertex] = dist[v] + adjacent->weight; //Add weight from one path to new path.
                path[adjacent->vertex] = v; //Set new path.
            }
            adjacent = array[v].getNext(); //Proceed to do this for every new point.
        }

        done[v] = true; //Set as visited

        int d = INFINITY;


        for(int i = 1; i <= size; i++){ //Loop through adjacency list
            if(!done[i] && dist[i] < d){  //Check only the unvisited nodes smallest distance from a point.
                d = dist[i];
                v = i;
            }
        }
        if(d == INFINITY) break; //If all nodes in adjacency list has been visited, break from loop.
    }

}

// print graph
void Digraph::printGraph() const
{
    cout << "------------------------------------------------------------------" << endl;
    cout << "vertex  adjacency list                  " << endl;
    cout << "------------------------------------------------------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " : ";
        array[v].print();
    }

    cout << "------------------------------------------------------------------" << endl;
}

// print shortest path tree for s
void Digraph::printTree() const
{
    cout << "----------------------" << endl;
    cout << "vertex    dist    path" << endl;
    cout << "----------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " :" << setw(8) << dist[v] << setw(8) << path[v] << endl;
    }

    cout << "----------------------" << endl;
}

// print shortest path from s to t
void Digraph::printPath(int t) const
{
    if (t < 1 || t > size)
    {
         cout << "\nERROR: expected target t in range 1.." << size << " !" << endl;
         return;
    }
    printHelp(t);

    cout << " " << "(" << dist[t] << ")";


}

void Digraph::printHelp(int t) const {
    if(path[t] != 0){
        printHelp(path[t]);
        cout << " -> ";
    }
    cout << " " << t << " ";
}

void Digraph::initialize(int s){
    for(int i = 0; i < size+1; i++){
        dist[i] = INFINITY;
        path[i] = 0;
        done[i] = false;
    }
    dist[s] = 0;
    done[s] = true;
}
