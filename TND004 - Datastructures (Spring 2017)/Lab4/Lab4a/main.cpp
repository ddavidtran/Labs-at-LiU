/*********************************************
* file:	~\tnd004\lab\lab4a\main.cpp          *
* remark: test program for lab 4 part A      *
**********************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

#include "digraph.h"

// -- FUNCTION DECLARATIONS

int readInt(string prompt);

int menu();

Digraph * readGraph(string fileName);

// -- MAIN PROGRAM

int main()
{
    int choice = 0;
    string fileName;
    int s, t;

    Digraph *G = new Digraph(1);
    Digraph *temp;

    while (choice != 9)
    {
        switch (choice = menu())
        {
            case 1:
                cout << "File name   ? ";
                cout << flush;
                getline(cin, fileName);
                temp = readGraph(fileName);
                if (temp != nullptr)
                {
                    delete G;
                    G = temp;
                }
                break;
            case 2:
                s = readInt("Source s    ? ");
                G->uwsssp(s);
                break;
            case 3:
                s = readInt("Source s    ? ");
                G->pwsssp(s);
                break;
            case 4:
                cout << endl;
                G->printGraph();
                break;
            case 5:
                cout << endl;
                G->printTree();
                break;
            case 6:
                t = readInt("Target t    ? ");
                cout << "\nShortest path =";
                G->printPath(t);
                break;
            case 9:
                cout << "Bye bye ..." << endl;
                break;
            default:
                cout << "Bad choice!" << endl;
        }
    }

    return 0;
}

// -- FUNCTION DEFINITIONS

int readInt(string prompt)
{
    string number;

    cout << prompt << flush;
    getline(cin, number);

    return atoi(number.c_str());
}

int menu()
{
    string choice;

    cout << endl;
    cout << "== Menu =======\n";
    cout << "1. readGraph   \n";
    cout << "2. uwsssp      \n";
    cout << "3. pwsssp      \n";
    cout << "4. printGraph  \n";
    cout << "5. printTree   \n";
    cout << "6. printPath   \n";
    cout << "9. quit        \n";
    cout << "===============\n";

    return readInt("Your choice ? ");
}

Digraph * readGraph(string fileName)
{
    int n, u, v, w;
    Digraph *temp = nullptr;

    ifstream file(fileName.c_str());

    if (file.good())
    {
         file >> n;
         temp = new Digraph(n);

         while (!file.eof())
         {
             file >> u >> v >> w;
             temp->insertEdge(u, v, w);
         }

         file.close();
    }
    else
    {
         cout << "File not found!" << endl;
    }

    return temp;
}
