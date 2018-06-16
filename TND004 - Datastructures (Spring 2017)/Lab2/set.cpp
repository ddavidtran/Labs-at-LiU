#include "set.h"

/*****************************************************
* Implementation of the member functions             *
******************************************************/

//Default constructor
Set::Set ()
	: counter(0)
{
	//IMPLEMENT before HA session on week 14
    head = new Node();
    tail = new Node(0, nullptr, head);
    head->next = tail;

}


//Conversion constructor
Set::Set (int n)
	: counter(0)
{
    //IMPLEMENT before HA session on week 14
    head = new Node();
    tail = new Node(0, nullptr, head);
    head->next = tail;
    insert(tail,n);
    counter++;
}


//Constructor to create a Set from a sorted array
Set::Set (int a[], int n) // a is sorted
	: counter(0)
{
	//IMPLEMENT before HA session on week 14

    head = new Node();
    tail = new Node(0, nullptr, head);
    head->next = tail;

    for(int i = 0; i < n; i++){
        insert(tail, a[i]);
        counter++;
    }

}

Set::~Set()
{
	//IMPLEMENT before HA session on week 14
    make_empty();
    delete head;
    delete tail;

}


//Copy constructor
Set::Set (const Set& source)
	: counter(source.counter)
{
	//IMPLEMENT before HA session on week 14
    head = new Node();
    tail = new Node(0, nullptr, head);
    head->next = tail;

    Node *head= source.head->next;
    while(head->next){
        insert(tail,head->value);
        head = head->next;
    }

}

//Copy constructor with move semantics
//&&b is rvalue reference to Set.
Set::Set(Set &&b){
    head = b.head;
    tail = b.tail;
    counter = b.counter;

    b.head = nullptr;
    b.tail = nullptr;
    b.counter = 0;
}



//Copy-and-swap assignment operator
Set& Set::operator=(Set source)
{
    if(this != &source){
        Set copy(source);
        swap(head, copy.head);
        swap(tail, copy.tail);
        swap(counter, copy.counter);
    }

	return *this;
}


//Test whether a set is empty
bool Set::_empty () const
{
	return (!counter);
}


//Test set membership
bool Set::is_member (int val) const
{
	//IMPLEMENT before HA session on week 14
    Node *p = head->next;
    while (p->next != nullptr){
        if(p->value == val){
            return true;
        }
        p = p->next;
    }
    return false;
}



//Return number of elements in the set
unsigned Set::cardinality() const
{
	return counter;
}


//Make the set empty
void Set::make_empty()
{
	//IMPLEMENT before HA session on week 14
    if(head == nullptr){
        return;
    }
    while(head->next != tail)
    {
        erase(head->next);
    }
}

void Set::insert(Node* p , int val)
{
        Node* newNode = new Node(val, p, p->prev);
        p->prev = p->prev->next = newNode;
}

void Set::erase(Node* p)
{
    p->prev->next = p->next;
    p->next->prev = p->prev;
    counter--;

    delete p;
}

//Modify *this such that it becomes the union of *this with Set S
//Add to *this all elements in Set S (repeated elements are not allowed)
Set& Set::operator+=(const Set& S)
{
	//IMPLEMENT before HA session on week 14
    Node *p = head->next;
    Node *p2 = S.head->next;
    while (p != tail && p2 != S.tail) {
        if (p->value == p2->value){
            p = p->next;
            p2 = p2->next;
        }
        else if(p->value < p2->value){
            p = p->next;
        }
        else if(p->value > p2->value){
            insert(p,p2->value);
            p2 = p2->next;
        }
    }

    while(p2 && p2 != S.tail){
        insert(tail, p2->value);
        p2 = p2->next;
    }


	return *this;
}

//Return true, if the set is a subset of b, otherwise false
//a <= b iff every member of a is a member of b
bool Set::operator<=(const Set& b) const {
if(b.cardinality() < cardinality()){
    return false;
}
    Node *p = head->next;
    Node *p2 = b.head->next;

    while(p->next != nullptr && p2->next != nullptr){
        if(p->value == p2->value){
            p = p->next;
            p2 = p2->next;
        }
        else if(p->value < p2->value){
            return false;
        }
        else if(p->value > p2->value){
            p2 = p2->next;
        }
    }

}


//Return true, if the set is equal to set b
//a == b, iff a <= b and b <= a
bool Set::operator==(const Set& b) const
{
    if(*this <= b && b <= *this){
        return true;
    }
    else{
        return false;
    }
}


//Return true, if the set is different from set b
//a == b, iff a <= b and b <= a
bool Set::operator!=(const Set& b) const
{
    /*Node *p = head;
    Node *p2 = b.head;
    while(p->next != nullptr && p2->next != nullptr){
        p = p->next;
        p2 = p2-> next;
        if(p->value != p2->value){
            return true;
        }

    }
    return false;*/
    if(!(b <= *this)){
        return true;
    }
    else{
        return false;
    }
}



//Return true, if the set is a strict subset of S, otherwise false
//a == b, iff a <= b but not b <= a
bool Set::operator<(const Set& b) const
{
    if(*this <= b){
        return true;
    }
    else if(!(b <= *this)){
        return false;
    }
}


//Modify *this such that it becomes the intersection of *this with Set S
Set& Set::operator*=(const Set& S)
{
	//IMPLEMENT
    Node* p = head->next;
    Node* p2 = S.head->next;
    while(p != tail && p2 != S.tail){
        if(p->value == p2->value){
            p = p->next;
            p2 = p2->next;
        }
        else if(p->value > p2->value){
            p2 = p2->next;
        }
        else if(p->value < p2->value){
            erase(p);
            p = p->next;

        }
    }

    while(p->next != nullptr){
        erase(p);
        p = p->next;

    }
    return *this;

}


//Modify *this such that it becomes the Set difference between Set *this and Set S
Set& Set::operator-=(const Set& S)
{
	//IMPLEMENT
    Node* p1 = head->next;
    Node* p2 = S.head->next;
    while(p2->next != nullptr && p1->next != nullptr)
    {
        if(p2->value == p1->value)
        {
            erase(p1);
            p1 = p1->next;
            p2 = p2->next;
        }
        else if(p2->value > p1->value)
        {
            p1 = p1->next;
        }
        else if(p2->value < p1->value)
        {
            p2 = p2->next;
        }

    }

	return *this;
}


// Overloaded operator<<
ostream& operator<<(ostream& os, const Set& b)
{
	if (b._empty())
		os << "Set is empty!" << endl;
	else
	{
		auto temp = b.head->next;

		os << "{ ";
		while (temp != b.tail)
		{
			os << temp->value << " ";
			temp = temp->next;
		}

		os << "}" << endl;
	}

	return os;
}


