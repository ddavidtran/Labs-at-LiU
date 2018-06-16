 #include <iostream>
#include <utility> //std::move

using namespace std;


/** Class to represent a Set of ints
 *
 * Set is implemented as a sorted doubly linked list
 * Sets should not contain repetitions, i.e.
 * two ints with the same value cannot belong to a Set
 *
 * All Set operations have a linear complexity, in the worst case
 */
class Set
{

private:

	/** Class Node
	 *
	 * This class represents an internal node of a doubly linked list storing an int
	 * All members of class Node are public
	 * but only class Set can access them, since Node is declared in the private part of class Set
	 *
	 */
	class Node
	{
	public:
		/** Constructor
		 *
		 * \param nodeVal int to be stored be stored in the Node
		 * \param nextPtr a pointer to the next Node in the list
		 * \param prevPtr a pointer to the previous Node in the list
		 *
		 */
		explicit Node (int nodeVal = 0, Node* nextPtr = nullptr, Node* prevPtr = nullptr)
			: value(nodeVal), next (nextPtr), prev(prevPtr)
		{  }

		//Destructor
		~Node() = default;

		//Copy constructor
		Node(const Node& rhs) = default;

		//Assignment operator
		Node& operator=(const Node& rhs) = default;

		//Data members
		int value;		//int stored in the Node
		Node* next;		//Pointer to the next Node
		Node* prev;		//Pointer to the previous Node
	};


public:

	//Default constructor: create an empty Set
	//IMPLEMENT before HA session on week 14
	Set ();

	//Conversion constructor: Convert val into a singleton {val}
	//IMPLEMENT before HA session on week 14
	Set (int val);


	/** Constructor to create a Set from an array of int
	 *
	 * Create a Set with (a copy of) all ints in array a
	 * \param a sorted array of ints
	 * \param n number of ints in array a
	 *
	 */
	 //IMPLEMENT before HA session on week 14
	Set (int a[], int n);


	/** Copy constructor
	 *
	 * Create a new Set as a copy of Set b
	 * \param b Set to be copied
	 * Function does not modify Set b in any way
	 *
	 */
	 //IMPLEMENT before HA session on week 14
	Set (const Set& b);

    /** Copy constructor for move semantics
     *  Not const since we want to steal from it.
     */
    Set (Set&& b);


    /** Destructor
     *
     * Deallocate all memory (Nodes) allocated by the constructor
     *
     */
	 //IMPLEMENT before HA session on week 14
	~Set ();


	/** Assignment operator
	 *
	 * Assigns new contents to the Set, replacing its current content
	 * \param source Set to be copied into Set *this
	 *
	 */
	 //IMPLEMENT before HA session on week 14
	Set& operator=(Set source);


	/** Test whether the Set is empty
	 *
	 * This function does not modify the Set in any way
	 * Return true if the set is empty, otherwise false
	 *
	 */
	bool _empty () const;


	/** Count the number of values stored in the Set
	 *
	 * This function does not modify the Set in any way
	 * Return number of elements in the set
	 *
	 */
	unsigned cardinality() const;


	/** Test whether val belongs to the Set
	 *
	 * This function does not modify the Set in any way
	 * Return true if val belongs to the set, otherwise false
	 *
	 */
	 //IMPLEMENT before HA session on week 14
	bool is_member (int val) const;


	/** Transform the Set into an empty se
	*
	* Remove all nodes from the list
	*
	*/
	//IMPLEMENT before HA session on week 14
	void make_empty();


	/** Modify *this such that it becomes the union of *this with Set S
	*
	* Set *this is modified and then returned
	*
	*/
	//IMPLEMENT before HA session on week 14
	Set& operator+=(const Set& S);


	/** Modify *this such that it becomes the intersection of *this with Set S
	*
	* Set *this is modified and then returned
	*
	*/
	//IMPLEMENT
	Set& operator*=(const Set& S);

	/** Modify *this such that it becomes the Set difference between Set *this and Set S
	*
	* Set *this is modified and then returned
	*
	*/
	//IMPLEMENT
	Set& operator-=(const Set& S);


	/** Test whether *this is a subset of Set b
	 *
	 * a <= b iff every member of a is a member of b
	 * Function does not modify *this nor b in any way
	 * Return true, if *this is a subset of b, otherwise false
	 *
	 */
	 //IMPLEMENT
	bool operator<=(const Set& b) const;


	/** Test whether *this and b represent the same set
	 *
	 * a == b, iff a <= b but not b <= a
	 * Function does not modify *this nor b in any way
	 * Return true, if *this stores the same elements as Set b, otherwise false
	 *
	 */
	 //IMPLEMENT
	bool operator==(const Set& b) const;


	/** Test whether *this and b represent different sets
	 *
	 * a != b, iff (a==b) is false
	 * Function does not modify *this nor b in any way
	 * Return true, if *this stores the same elements as Set b, otherwise false
	 *
	 */
	 //IMPLEMENT
	bool operator!=(const Set& b) const;


	/** Test whether *this is a strict subset of Set b
	 *
	 * a < b iff a <= b but not b <= a
	 * Function does not modify *this nor b in any way
	 * Return true, if *this is a strict subset of b, otherwise false
	 */
	 //IMPLEMENT
	bool operator<(const Set& b) const;




private:
	Node* head;			//Pointer to the dummy header Node
	Node* tail;			//Pointer to the dummy tail Node

	unsigned counter;	//Count number of values in the Set


	/* ************************** *
	* Private Member Functions    *
	* **************************  */

	//Add here declaration of any private auxiliary member functions, if needed

    void insert(Node* p, int val);
    void del(Node* p);
    void erase(Node* p);


	/* **************************** *
	* Overloaded Global Operators   *
	* ***************************** */


	/** Overloaded operator<<
	 *
	 * \param os ostream object where the set b elements are written
	 *
	 */
	friend ostream& operator<<(ostream& os, const Set& b);


	/** Overloaded operator+: Set union S1+S2
	 *
	 * S1+S2 is the Set of elements in Set S1 or in Set S2 (without repeated elements)
	 * Function does not modify S1 nor S2 in any way
	 * Return a new Set representing the union of S1 with S2, S1+S2
	 *
	 */
	friend Set operator+(Set S1, const Set& S2)
	{
		return (S1 += S2);
	}


	/** Overloaded operator*: Set intersection S1*S2
	 *
	 * S1*S2 is the Set of elements in both Sets S1 and set S2
	 * Function does not modify S1 nor S2 in any way
	 * Return a new Set representing the intersection of S1 with S2, S1*S2
	 *
	 */
	friend Set operator*(Set S1, const Set& S2)
	{
		return (S1 *= S2);
	}


	/** Overloaded operator-: Set difference S1-S2
	 *
	 * S1-S2 is the Set of elements in Set S1 that do not belong to Set S2
	 * Function does not modify S1 nor S2 in any way
	 * Return a new Set representing the set difference S1-S2
	 *
	 */
	friend Set operator-(Set S1, const Set& S2)
	{
		return (S1 -= S2);
	}
};
