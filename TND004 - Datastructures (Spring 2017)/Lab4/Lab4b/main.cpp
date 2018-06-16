/*********************************************
* file:	~\tnd004\lab\lab4b\main.cpp          *
* remark: test program for lab 4 part B      *
**********************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

#include "graph.h"

// -- FUNCTION DECLARATIONS

int readInt(string prompt);

int menu();

Graph * readGraph(string fileName);

// -- MAIN PROGRAM

int main()
{
    int choice = 0;
    string fileName;

    Graph *G = new Graph(1);
    Graph *temp;

    while (choice != 9)
    {
        switch (choice = menu())
        {
            case 1:
                cout << "File name   ? ";
                cout << flush;
                getline(cin, fileName);
                temp = readGraph(fileName);
                if (temp != NULL)
                {
                    delete G;
                    G = temp;
                }
                break;
            case 2:
                cout << endl;
                G->mstPrim();
                break;
            case 3:
                cout << endl;
                G->mstKruskal();
                break;
            case 4:
                cout << endl;
                G->printGraph();
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
    cout << "2. mstPrim     \n";
    cout << "3. mstKruskal  \n";
    cout << "4. printGraph  \n";
    cout << "9. quit        \n";
    cout << "===============\n";

    return readInt("Your choice ? ");
}

Graph * readGraph(string fileName)
{
    int n, u, v, w;
    Graph *temp = NULL;

    ifstream file(fileName.c_str());

    if (file.good())
    {
         file >> n;
         temp = new Graph(n);

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
