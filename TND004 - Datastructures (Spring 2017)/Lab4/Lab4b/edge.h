/*********************************************
* file:	~\tnd004\lab\lab4b\edge.h            *
* remark: interface for (explicit) edges     *
**********************************************/

#ifndef EDGE_H
#define EDGE_H

#include <utility>

using namespace std::rel_ops;

class Edge
{
public:
    // -- CONSTRUCTORS
    Edge(int u = 0, int v = 0, int w = 0)
    : head(u), tail(v), weight(w) { }

    // -- RELATIONAL OPERATORS
    bool operator< (const Edge &e) const;
    bool operator==(const Edge &e) const;

    // -- FRIENDS
    friend ostream & operator<< (ostream &os, const Edge &e);

    // -- DATA MEMBERS
    int head;
    int tail;
    int weight;
};

#endif
