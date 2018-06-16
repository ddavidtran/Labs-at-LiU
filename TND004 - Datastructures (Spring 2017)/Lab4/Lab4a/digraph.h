/*********************************************
* file:	~\tnd004\lab\lab4a\digraph.h         *
* remark: interface for directed graphs      *
**********************************************/

#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "list.h"

class Digraph
{
public:
    // -- CONSTRUCTORS
    explicit Digraph(int n);

    // -- DESTRUCTOR
    ~Digraph();

    // -- MEMBER FUNCTIONS

    // insert directed edge (u, v) with weight w
    // update weight w if edge (u, v) is present
    void insertEdge(int u, int v, int w);

    // remove directed edge (u, v)
    void removeEdge(int u, int v);

    // unweighted single source shortest paths
    void uwsssp(int s);

    // positive weighted single source shortest pats
    void pwsssp(int s);

    // print graph
    void printGraph() const;

    // print shortest path tree for s
    void printTree() const;

    // print shortest path from s to t
    void printPath(int t) const;

    void printHelp(int t) const;

    void initialize(int s);

private:
    // -- DATA MEMBERS
    List *array;
    int  *dist;
    int  *path;
    bool *done;
    int  size;
};

#endif
