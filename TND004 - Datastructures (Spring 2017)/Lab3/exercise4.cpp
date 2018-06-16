//
// Created by David Tran on 2017-05-09.
//

#include <iostream>
#include <vector>
#include <fstream>


#include "BinarySearchTree.h"

using namespace std;
class Row{
public:
    string key;
    int frequency;

    //Default constructor
    Row(string s, int f ): key{s}, frequency{f}{};

    //Copy constructor
    Row(const Row &r): key{r.key}, frequency{r.frequency}{}

    bool operator<(const Row &r) const{
        return key < r.key;
    }

    friend ostream& operator<<(ostream& out, const Row& r){
        out << r.key << setw(3) << r.frequency << endl;
        return out;
    }


};

int main(){
    BinarySearchTree<Row> T;
    ifstream file("words.txt");

    if (!file)
    {
        cout << "couldn't open file words.txt" << endl;
        return 1;
    }

    vector<string> V1 = { istream_iterator<string>{file}, istream_iterator<string>{} };
    file.close();


    for(auto i : V1) {
        Row row(i, 1);
        auto tmp = T.contains(row);
        if(tmp != T.end()){ //NOTE: contains should now return a bi-iterator
            tmp->frequency++;
        }
        else{
            T.insert(row);
        }
    }

    for(auto it = T.begin(); it != T.end(); ++it){
        cout << *it;
    }
    return 0;
}