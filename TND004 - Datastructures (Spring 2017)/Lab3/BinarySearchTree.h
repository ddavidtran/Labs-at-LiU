#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "dsexceptions.h"
#include <algorithm>
#include <iomanip>
#include <mutex>

using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// void preorderPrint( )  --> Print tree using pre-order with indentation [ADDED]
// void get_parent( )     --> Returns value stored in the parent node of x. [ADDED]
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinarySearchTree
{
    class BinaryNode;

  public:
    class BiIterator;
    BinarySearchTree( ) : root{ nullptr }
    {
    }

    /**
     * Copy constructor
     */
    BinarySearchTree( const BinarySearchTree & rhs ) : root{ nullptr }
    {
        root = clone( rhs.root, nullptr );
    }

    /**
     * Move constructor
     */
    BinarySearchTree( BinarySearchTree && rhs ) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }

    /**
     * Destructor for the tree
     */
    ~BinarySearchTree( )
    {
        makeEmpty( );
    }

    /**
     * Copy assignment
     */
    BinarySearchTree & operator=( const BinarySearchTree & rhs )
    {
        BinarySearchTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    /**
     * Move assignment
     */
    BinarySearchTree & operator=( BinarySearchTree && rhs )
    {
        std::swap( root, rhs.root );
        return *this;
    }


    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMin( root )->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMax( root )->element;
    }

    /**
     * Returns true if x is found in the tree.
     */
    //BinarySearchTree::BiIterator contains( const Comparable & x ) const
    bool contains( const Comparable & x ) const
    {
        return contains( x, root );
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
        return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ostream & out = cout ) const
    {
        if( isEmpty( ) )
            out << "Empty tree" << endl;
        else
            printTree( root, out );
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
        makeEmpty( root );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( const Comparable & x )
    {

        insert( x, root);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( Comparable && x )
    {

        insert( std::move( x ), root);
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x )
    {
        remove( x, root);
    }


    const Comparable get_parent(const Comparable x) {
        BinaryNode* tmp = search(root, x); //t when found x in tree.
        if(tmp && tmp->parent != nullptr) {
            return tmp->parent->element;
        }
        else{
            return Comparable();
        }
    }

    void find_pred_succ(const Comparable& x, Comparable& pred, Comparable& suc) const{
        //Call utility function
        utilityPredSuc(x, pred, suc, root);
    }

    //BiIterator class
public:
    class BiIterator
    {
    public:
        BiIterator(){
            current = nullptr;
        }
        BiIterator(BinarySearchTree::BinaryNode* p){
            current = p;
        }
        const Comparable& operator*() const{
            return current->element;
        }
        Comparable* operator->() const{
            return &current->element;
        }
        bool operator==(const BiIterator &it) const {
            return current == it.current;
        }
        bool operator!=(const BiIterator &it) const{
            return current != it.current;
        }
        BiIterator& operator++() //pre-increment
        {
            current = find_successor(current);
            return *this;
        }
        BiIterator& operator--() //pre-decrement
        {
            current = find_predecessor(current);
            return *this;
        }



    private:
        BinaryNode *current;
    };

    BiIterator begin() const{
        if (isEmpty()) return end();
        return BiIterator(findMin(root));
    }

    BiIterator end() const{
        return BiIterator(nullptr);
    }


  private:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;
        BinaryNode *parent;

        BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt, BinaryNode *pt )
          : element{ theElement }, left{ lt }, right{ rt }, parent{pt} { }

        BinaryNode( Comparable && theElement, BinaryNode *lt, BinaryNode *rt, BinaryNode* pt )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt }, parent{pt} { }
    };

    BinaryNode *root;

    void utilityPredSuc(const Comparable& key, Comparable& pred, Comparable& suc, BinaryNode* t ) const{
         if(t == nullptr){
             return;
         }
        if(key == t->element){
            //Maximum value in left subtree is pred
            if(t->left != nullptr){
                    pred = findMax(t->left)->element;
            }

            //Minimum value in right subtree is succ.
            if(t->right != nullptr){
                suc = findMin(t->right)->element;
            }
            else{
                return;
            }
        }
        else if(key < t->element) //if key is smaller than root node, go to left subtree.
        {
            suc = t->element;
            utilityPredSuc(key, pred, suc, t->left);
        }
        else if(key > t->element){ //Go to right subtree
            pred = t->element;
            utilityPredSuc(key, pred, suc, t->right);
        }
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable & x, BinaryNode * & t)
    {
        if( t == nullptr ) {
            t = new BinaryNode{x, nullptr, nullptr, t};
        }
        else if( x < t->element ) {
            insert(x, t->left);
            t->left->parent = t;
        }
        else if( t->element < x ) {
            insert(x, t->right);
            t->right->parent = t;
        }
        else
        {
            ;  // Duplicate; do nothing
        }
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( Comparable && x, BinaryNode * & t)
    {
        if( t == nullptr ) {
            t = new BinaryNode{std::move(x), nullptr, nullptr, t};
        }
        else if( x < t->element ) {
            insert(std::move(x), t->left);
            t->left->parent = t;
        }
        else if( t->element < x ) {
            insert(std::move(x), t->right);
            t->right->parent = t;
        }
        else
        {
             ;  // Duplicate; do nothing
        }
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, BinaryNode * &t)
    {

        if( t == nullptr )
            return;   // Item not found; do nothing

        if( x < t->element ) {
            remove(x, t->left);
        }
        else if( t->element < x ) {
            remove(x, t->right);
        }

        else if(t->left != nullptr && t->right != nullptr){ //Two children
            BinaryNode* tmp = findMin(t->right);
            detach(tmp);

            tmp->left = t->left;
            tmp->right = t->right;
            tmp->parent = t->parent;

            t->left = t->right = t->parent = nullptr;
            delete t;
            t = tmp;

        }
        else { //Leafs
            BinaryNode *tmp = t; //Create backup
            detach(tmp);
            delete tmp;
        }
    }

    void detach(BinaryNode* t){
        if(t->parent->left != nullptr){
            t->parent->left = (t->left != nullptr) ? t->left : t->right;
            if(t->left){
                t->left->parent = t->parent;
            }


        }
        else if(t->parent->right != nullptr){
            t->parent->right = (t->left != nullptr) ? t->left : t->right;
            if(t->right) {
                t->right->parent = t->parent;
            }

        }

        t->parent = nullptr;
        t->right = nullptr;
        t->left = nullptr;

    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    static BinaryNode * findMin(BinaryNode *t )
    {
        if( t == nullptr )
            return nullptr;
        if( t->left == nullptr )
            return t;
        return findMin( t->left );
    }

    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    static BinaryNode * findMax(BinaryNode *t )
    {
        if( t != nullptr )
            while( t->right != nullptr )
                t = t->right;
        return t;
    }

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     */
    //BinarySearchTree::BiIterator contains( const Comparable & x, BinaryNode *t ) const
    bool contains( const Comparable & x, BinaryNode *t ) const
    {
        if( t == nullptr )
            //return BiIterator();
            return false;
        else if( x < t->element )
            return contains( x, t->left );
        else if( t->element < x )
            return contains( x, t->right );
        else
            //return BiIterator(t);    // Match
            return true;
    }
/****** NONRECURSIVE VERSION*************************
    bool contains( const Comparable & x, BinaryNode *t ) const
    {
        while( t != nullptr )
            if( x < t->element )
                t = t->left;
            else if( t->element < x )
                t = t->right;
            else
                return true;    // Match

        return false;   // No match
    }
*****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( BinaryNode * & t )
    {
        if( t != nullptr )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = nullptr;
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     * In-order traversal is used
     */
    void printTree( BinaryNode *t, ostream & out ) const
    {
        preorderPrint(t, out, 2);
    }

    void preorderPrint (BinaryNode* t, ostream &out, int indentation)const {
        if (t != nullptr) {
            out << setw(indentation) << t->element << endl;
            preorderPrint(t->left, out, indentation + 2);
            preorderPrint(t->right, out, indentation + 2);
        }
    }

    BinaryNode* search(BinaryNode* t, const Comparable& x) const{
            if(t == nullptr){
                return nullptr;
            }
            else if(x < t->element)
                 return search(t->left, x);
            else if(x > t->element)
                return search(t->right, x);
            else
                return t;
    }

    static BinaryNode * find_successor(BinaryNode* t) {
        if(t && t->right){
            return findMin(t->right);
        }
        else{
            auto suc = t->parent;
            while(suc != nullptr && t == suc->right){
                t = suc;
                suc =  suc->parent;
            }
            return suc;

        }

    }

    static BinaryNode * find_predecessor(BinaryNode* t){
        if(t && t->left){
            return findMax(t->left);
        }
        else{
            auto pred = t->parent;
            while(pred != nullptr && t == pred->left){
                t = pred;
                pred = pred->parent;
            }
            return pred;

        }
    }
    /**
     * Internal method to clone subtree.
     */
    BinaryNode * clone( BinaryNode *t, BinaryNode* parent ) const
    {
        if( t == nullptr )
            return nullptr;
        else {
            auto tmp = new BinaryNode(t->element, nullptr, nullptr, parent);
            tmp->left = clone(t->left, tmp);
            tmp->right = clone(t->right, tmp);
            return tmp;
        }
    }
};

#endif
