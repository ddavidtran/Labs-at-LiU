/*********************************************
* file:	~\tnd004\lab\lab4b\list.h            *
* remark: interface for adjacency lists      *
**********************************************/

#ifndef LIST_H
#define LIST_H

class Node
{
public:
    // -- CONSTRUCTORS
    Node(int v = 0, int w = 0, Node *n = NULL)
    : vertex(v), weight(w), next(n) { }

    // -- DATA MEMBERS
    int vertex;
    int weight;
    Node *next;
};

class List
{
public:
    // -- CONSTRUCTORS
    List();

    // -- DESTRUCTOR
    ~List();

    // -- MEMBER FUNCTIONS

    // return pointer to first node (or NULL)
    Node * getFirst();

    // return pointer to next node (or NULL)
    Node * getNext();

    // insert vertex v in sorted order
    // update weight w if v is present
    void insert(int v, int w);

    // remove vertex v
    void remove(int v);

    // print list
    void print() const;

private:
    // -- DATA MEMBERS
    Node *header;
    Node *current;
};

#endif
