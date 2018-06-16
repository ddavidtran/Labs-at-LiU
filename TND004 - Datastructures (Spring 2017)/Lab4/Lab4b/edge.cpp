/*********************************************
* file:	~\tnd004\lab\lab4b\edge.cpp          *
* remark: implementation of (explicit) edges *
**********************************************/


#include <iostream>
#include <iomanip>

using namespace std;

#include "edge.h"

// -- RELATIONAL OPERATORS

bool Edge::operator< (const Edge &e) const
{
   return weight <  e.weight;
}

bool Edge::operator==(const Edge &e) const
{
   return weight == e.weight;
}

// -- FRIENDS

ostream & operator<< (ostream &os, const Edge &e)
{
    os << "(" << setw(2) << e.head   << ", "
              << setw(2) << e.tail   << ", "
              << setw(2) << e.weight << ") ";

    return os;
}
