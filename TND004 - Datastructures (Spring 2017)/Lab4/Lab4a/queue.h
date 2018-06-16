/*********************************************
* file:	~\tnd004\lab\lab4a\queue.h           *
* remark:  class template for generic queues *
**********************************************/


#ifndef QUEUE_H
#define QUEUE_H

#include <cassert>

using namespace std;

template <class Object>
class Queue
{
public:
    // -- CONSTRUCTORS
    explicit Queue(int theCapacity = 10);

    // -- DESTRUCTOR
    ~Queue();

    // -- MEMBER FUNCTIONS

    void makeEmpty();
    bool isEmpty() const;
    void enqueue(const Object &x);
    void dequeue();
    const Object & getFront() const;

private:
    // -- MEMBER FUNCTIONS
    bool isFull() const;

    // -- DATA MEMBERS
    Object *theArray;
    int front;
    int rear;
    int size;
    int capacity;
};

// -- CONSTRUCTORS

template <class Object>
Queue<Object>::Queue(int theCapacity)
{
    assert(theCapacity > 0);
    capacity = theCapacity;
    theArray = new Object[capacity];
    makeEmpty();
}

// -- DESTRUCTOR

template <class Object>
Queue<Object>::~Queue()
{
    delete[] theArray;
}

// -- MEMBER FUNCTIONS

template <class Object>
void Queue<Object>::makeEmpty()
{
    front = 0;
    rear  = capacity - 1;
    size  = 0;
}

template <class Object>
bool Queue<Object>::isEmpty() const
{
    return size == 0;
}

template <class Object>
bool Queue<Object>::isFull() const
{
    return size == capacity;
}

template <class Object>
void Queue<Object>::enqueue(const Object &x)
{
    if (isFull())
    {
        Object *oldArray = theArray;
        theArray = new Object[2 * capacity];

        for (int i = 0; i < front; i++)
        {
            theArray[i] = oldArray[i];
        }

        for (int i = front; i < capacity; i++)
        {
            theArray[i + capacity] = oldArray[i];
        }

        delete[] oldArray;

        front += capacity;

        if (rear == capacity - 1)
        {
            rear += capacity;
        }

        capacity *= 2;
    }

    rear = (rear + 1) % capacity;
    theArray[rear] = x;
    size++;
}

template <class Object>
void Queue<Object>::dequeue()
{
    assert(!isEmpty());
    front = (front + 1) % capacity;
    size--;
}

template <class Object>
const Object & Queue<Object>::getFront() const
{
    assert(!isEmpty());
    return theArray[front];
}

#endif
