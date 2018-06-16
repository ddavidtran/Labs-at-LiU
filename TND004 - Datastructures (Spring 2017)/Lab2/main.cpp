
#include <iostream>
#include <iomanip>

#include "set.h"

using namespace std;


//Do not modify
int main()
{
	/*****************************************************
	* TEST PHASE 0                                       *
	* Default constructor, conversion constructor, and   *
	* operator<<                                         *
	******************************************************/
	cout << "TEST PHASE 0: default and conversion constructor\n\n";

	Set S1;
	Set S2(-4);

	cout << "S1 = " << S1 << endl;		//S1 = empty set
	cout << "S2 = " << S2 << endl;		//S2 = {-4}

	/*****************************************************
	* TEST PHASE 1                                       *
	* Constructor: create a Set from an array            *
	******************************************************/
	cout << "\nTEST PHASE 1: constructor from an array\n\n";

	int A1[] = { 1, 3, 5 };
	int A2[] = { 2, 3, 4 };

	Set S3(A1, 3);
	Set S4(A2, 3);

	cout << "S3 = " << S3 << endl;		//S3 = {1 3 5}
	cout << "S4 = " << S4 << endl;		//S4 = {2 3 4}

	/*****************************************************
	* TEST PHASE 2                                       *
	* Copy constructor                                   *
	******************************************************/
	cout << "\nTEST PHASE 2: copy constructor\n\n";

	Set S5(S3);

	cout << "S5 = " << S5 << endl;		//S5 = {1 3 5}

	/*****************************************************
	* TEST PHASE 3                                       *
	* Assignment operator: operator=                     *
	******************************************************/
	cout << "\nTEST PHASE 3: operator=\n\n";

	S1 = S2 = S5;

	cout << "S1 = " << S1 << endl;		//S1 = {1 3 5}
	cout << "S2 = " << S2 << endl;		//S2 = {1 3 5}

	/*****************************************************
	* TEST PHASE 4                                       *
	* is_member                                          *
	******************************************************/
	cout << "\nTEST PHASE 4: is_member\n\n";

	for (int i = 1; i <= 5; i++)
	{
		cout << "S1.is_member(" << i << ") ? "
			 << (S1.is_member(i) ? 'T' : 'F') << endl;
	}
	cout << "\nS1.is_member(99999) ? "
		 << (S1.is_member(99999) ? 'T' : 'F') << endl;

	/*****************************************************
	* TEST PHASE 5                                       *
	* cardinality, make_empty                            *
	******************************************************/
	cout << "\nTEST PHASE 5: cardinality and make_empty\n\n";

	cout << "S1.cardinality() = "
		 << S1.cardinality() << endl;	//3

	cout << "S1 becomes the empty set ..." << endl;

	S1.make_empty();

	cout << "S1.cardinality() = "
		 << S1.cardinality() << endl;	//0

	/*****************************************************
	* TEST PHASE 6                                       *
	* Overloaded operators: operator+=, operator*=       *
	*                   and operator-=                   *
	******************************************************/
	cout << "\nTEST PHASE 6: operator+=, operator*=, operator-=\n\n";

	S5 += S4;
	S4 *= S4;
	S1 -= S1;

	cout << "S5 = " << S5 << endl;		//S5 = {1 2 3 4 5}
	cout << "S4 = " << S4 << endl;		//S5 = {2 3 4}
	cout << "S1 = " << S1 << endl;		//S1 = empty set


	/*****************************************************
	* TEST PHASE 7                                       *
	* Overloaded operators: union, intersection, and     *
	* and difference                                     *
	******************************************************/
	cout << "\nTEST PHASE 7: union, intersection, and difference\n\n";
	S1 = (S3 + S4);
	S2 = S3 * S4;
	S5 = S3 - S4;


	cout << "S1 = " << S1 << endl;		//S1 = {1 2 3 4 5}
	cout << "S2 = " << S2 << endl;		//S2 = {3}
	cout << "S3 = " << S3 << endl;		//S3 = {1 3 5}
	cout << "S4 = " << S4 << endl;		//S4 = {2 3 4}
	cout << "S5 = " << S5 << endl;		//S5 = {1 5}

	cout << "S2 + S1 * S5 = " << S2 + S1 * S5 << endl;	//{1 3 5}

	/*****************************************************
	* TEST PHASE 8                                       *
	* Overloaded operators: equality, subset, and        *
	* strict subset                                      *
	******************************************************/
	cout << "\nTEST PHASE 8: equality, subset, strict subset\n\n";

	cout << "S4 == S1 ? " << (S4 == S1 ? 'T' : 'F') << endl;	//F
	cout << "S4!== S1 ? " << (S4 != S1 ? 'T' : 'F') << endl;	//T
	cout << "S4 <= S1 ? " << (S4 <= S1 ? 'T' : 'F') << endl;	//T
	cout << "S4 <  S1 ? " << (S4 <  S1 ? 'T' : 'F') << endl;	//T

	/*****************************************************
	* TEST PHASE 9                                       *
	* Overloaded operators: mixed-mode arithmetic        *
	******************************************************/
	cout << "\nTEST PHASE 9: mixed-mode arithmetic\n\n";

	//Note: conversion constructor is called
	S2 = 4 + S3 - 5 - (S3 + S4) - 99999;
	S5 = S4 - 2 + S2 + 24;

	cout << "S2 = " << S2 << endl;		//S2 = empty set
	cout << "S5 = " << S5 << endl;		//S5 = {3 4 24}

	/*****************************************************
	* TEST PHASE 10                                      *
	* Move constructor                                   *
	******************************************************/
	cout << "\nTEST PHASE 10: move constructor\n\n";

	Set *ptr_S = new Set(S4);

	*ptr_S += 24;

	cout << "*ptr_S = " << *ptr_S << endl;
	cout << "S5 = " << S5 << endl;

    cout << "std::S5 * move(*ptr_S) = " << S5 * std::move(*ptr_S) << endl;
	//cout << "std::move(*ptr_S) * S5 = " << std::move(*ptr_S) * S5 << endl;		//S6 = {3 4 24}

	delete ptr_S;

	/*****************************************************
	* TEST PHASE 11                                      *
	* move assignment operator                           *
	******************************************************/
	cout << "\nTEST PHASE 11: move assignment operator\n\n";

	ptr_S = new Set(S1);

	*ptr_S += 0;

	S5 = std::move(*ptr_S);			

	cout << "S5 = " << S5 << endl;	//S5 = {0 1 2 3 4 5}

	delete ptr_S;

    cout << "Ending ...." << endl;

	return 0;
}
