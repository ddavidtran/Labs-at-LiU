/*********************************************
* file:	~\tnd004\lab\lab4b\heap.h            *
* remark:  class template for generic heaps  *
**********************************************/

#ifndef HEAP_H
#define HEAP_H

#include <cassert>

using namespace std;

template <class Comparable>
class Heap
{
public:
    // -- CONSTRUCTORS
    explicit Heap(int theCapacity = 10);

    // -- DESTRUCTOR
    ~Heap();

    // -- MEMBER FUNCTIONS

    void makeEmpty();
    bool isEmpty() const;
    void insert(const Comparable x);
    Comparable deleteMin();

private:
    // -- MEMBER FUNCTIONS
    bool isFull() const;

    // -- DATA MEMBERS
    Comparable *array;
    int size;
    int capacity;
};

// -- CONSTRUCTORS

template <class Comparable>
Heap<Comparable>::Heap(int theCapacity)
{
    assert(theCapacity > 0);
    capacity = theCapacity;
    array = new Comparable[capacity + 1];
    size = 0;
}

// -- DESTRUCTOR

template <class Comparable>
Heap<Comparable>::~Heap()
{
  delete[] array;
}

// -- MEMBER FUNCTIONS

template <class Comparable>
void Heap<Comparable>::makeEmpty()
{
    size = 0;
}

template <class Comparable>
bool Heap<Comparable>::isEmpty() const
{
    return size == 0;
}

template <class Comparable>
bool Heap<Comparable>::isFull() const
{
    return size == capacity;
}

template <class Comparable>
void Heap<Comparable>::insert(const Comparable x)
{
    if (isFull())
    {
        Comparable *oldArray = array;
        capacity *= 2;
        array = new Comparable[capacity + 1];

        for (int i = 1; i <= size; i++)
        {
            array[i] = oldArray[i];
        }

        delete[] oldArray;
    }

    size++;

    int i = size;
    int p = i / 2;

    while (p >= 1 && array[p] > x)
    {
        array[i] = array[p];
        i = p;
        p = i / 2;
    }
    array[i] = x;
}

template <class Comparable>
Comparable Heap<Comparable>::deleteMin()
{
    assert(!isEmpty());

    Comparable x = array[1];
    Comparable y = array[size];

    size--;

    int i = 1;
    int c = 2 * i;

    while (c <= size)
    {
        if (c < size)
        {
            if (array[c + 1] < array[c])
            {
                c++;
            }
        }
        if (array[c] < y)
        {
            array[i] = array[c];
            i = c;
            c = 2 * i;
        }
        else
        {
            break;
        }
    }
    array[i] = y;
    return x;
}
#endif
