/*********************************************
* file:	~\tnd004\lab\lab4b\list.cpp          *
* remark: implementation of adjacency lists  *
**********************************************/

#include <iostream>
#include <iomanip>
using namespace std;

#include "list.h"

// -- CONSTRUCTORS

List::List()
{
    header  = new Node;
    current = header->next;
}

// -- DESTRUCTOR

List::~List()
{
    while (header != NULL)
    {
        Node *temp = header;
        header = temp->next;
        delete temp;
    }
}

// -- MEMBER FUNCTIONS

// return pointer to first node (or NULL)
Node * List::getFirst()
{
    current = header->next;
    return current;
}

// return pointer to next node (or NULL)
Node * List::getNext()
{
    if (current != NULL)
    {
        current = current->next;
    }
    return current;
}

// insert vertex v in sorted order
// update weight w if v is present
void List::insert(int v, int w)
{
    Node *p = header;

    while (p->next != NULL && p->next->vertex < v)
    {
        p = p->next;
    }

    if (p->next == NULL || p->next->vertex > v)
    {
        p->next = new Node(v, w, p->next);
    }
    else
    {
        p->next->weight = w;
    }

    current = header->next;
}

// remove vertex v
void List::remove(int v)
{
    Node *p = header;

    while (p->next != NULL && p->next->vertex < v)
    {
        p = p->next;
    }

    if (p->next != NULL && p->next->vertex == v)
    {
        Node *temp = p->next;
        p->next = temp->next;
        delete temp;
    }

    current = header->next;
}

// print list
void List::print() const
{
    Node *p = header->next;

    while (p != NULL)
    {
        cout << " (" << setw(2) << p->vertex
             << ", " << setw(2) << p->weight << ") ";
        p = p->next;
    }
    cout << endl;
}
