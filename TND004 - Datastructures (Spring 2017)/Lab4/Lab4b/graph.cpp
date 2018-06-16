/*********************************************
* file:	~\tnd004\lab\lab4b\graph.h           *
* remark:implementation of undirected graphs *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>
#include <queue>

using namespace std;

#include "graph.h"
#include "edge.h"
#include "heap.h"
#include "dsets.h"

const int INFINITY = 9999;

// -- CONSTRUCTORS

Graph::Graph(int n)
{
    assert(n >= 1);
    array = new List[n + 1];
    size  = n;
}

// -- DESTRUCTOR

Graph::~Graph()
{
    delete[] array;
}

// -- MEMBER FUNCTIONS

// insert undirected edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Graph::insertEdge(int u, int v, int w)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].insert(v, w);
    array[v].insert(u, w);
}

// remove undirected edge (u, v)
void Graph::removeEdge(int u, int v)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].remove(v);
    array[v].remove(u);
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {

    //Initialization
    bool *done = new bool[size + 1];
    int *dist = new int[size + 1];
    int *path = new int[size + 1];

    for (int i = 0; i <= size; i++) {
        done[i] = false;
        dist[i] = INFINITY;
        path[i] = 0;
    }

    int start = 1;
    dist[start] = 0;
    done[start] = true;
    int v = start;

    int totalWeight = 0;

    while (true) {
        Node *adjacent = array[v].getFirst(); //u = adjacent->vertex

        while (adjacent) {
            if (!done[adjacent->vertex] && dist[adjacent->vertex] >  adjacent->weight) {
                dist[adjacent->vertex] = adjacent->weight;
                path[adjacent->vertex] = v;
            }
            adjacent = array[v].getNext();
        }
        done[v] = true;
        int d = INFINITY;

        for(int i = 1; i <= size; i++){ //Loop through adjacency list
            if(!done[i] && dist[i] < d){
                d = dist[i];
                v = i;
            }
        }
        if(d == INFINITY){break;}
        cout << "( " << path[v] <<",  " << v <<",  " << dist[v] << ")" << endl;
        totalWeight+=dist[v];

    }
    cout << endl;
    cout << "Total weight = " << totalWeight << endl;
}


// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const{

    Heap<Edge> H;
    DSets D(size);
    int counter = 0;
    int totalWeight = 0;

    //Build heap of edges
    for(int i = 1; i <= size; i++){
        Node* adjacent = array[i].getFirst();
        while(adjacent){
            if(adjacent->vertex < i){ //adjacent->vertex = head, i = tail;
                H.insert(Edge(adjacent->vertex, i, adjacent->weight));
            }
            adjacent = array[i].getNext();
        }
    }

    Edge e;
    while(counter < size-1){
       e = H.deleteMin();
        if(D.find(e.head) != D.find(e.tail))
        {
            cout << e << endl;
            totalWeight += e.weight;
            D.join(D.find(e.head), D.find(e.tail));
            counter++;
        }
    }
    cout << endl;
    cout << "Total weight: " << totalWeight;
}


// print graph
void Graph::printGraph() const
{
    cout << "------------------------------------------------------------------" << endl;
    cout << "vertex  adjacency list                                            " << endl;
    cout << "------------------------------------------------------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " : ";
        array[v].print();
    }

    cout << "------------------------------------------------------------------" << endl;
}