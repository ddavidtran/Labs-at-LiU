//
// Created by David Tran on 2018-04-25.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <sstream>
#include <ctime>

/*****************************************************
* FUNCTION DECLARATIONS                              *
* Lab3 Part2                                         *
******************************************************/
void genNumericalFiles(std::unordered_map<std::string, int> &hashtable);
void convert2numerical(std::unordered_map<std::string,int> &hashtable, const std::vector<std::string>& vec, std::ofstream &outputFile);
void checkPlagiarization(const std::string& filename, std::unordered_map<std::string,int> map);
void preprocessSentence(std::string& sentence);


/*****************************************************
* MAIN                                               *
* Lab3 Part2                                         *
******************************************************/
int main(int argc, char* argv[]){
    std::unordered_map<std::string, int> map;
    std::string filename;
    std::clock_t start;
    double duration;

    if(argc != 2){
        std::cerr << "Wrong number of arguments given! Expected file" << std::endl;
        return 0;
    }

    try{
        filename = static_cast<std::string>(argv[1]);
        if(filename.find_first_of(".txt") == std::string::npos){
            std::cerr << "The filename doesn't end with '.txt'";
            return 0;
        }
        std::ifstream tryFile ("../text/"+filename);
        if(!tryFile.is_open()){
            std::cerr << "Could not open the file!";
            return 0;
        }
        else{
            tryFile.close();
        }
    }
    catch(const std::exception& e){
        std::cerr << "Can't cast filename to string!!";
        return 0;
    }
    genNumericalFiles(map);

    std::cout << std::endl;
    std::cout << "Generated numerical text files based on the original text files." << std::endl;
    std::cout << "Created a hashed dictionary of size: " << map.size() << std::endl;
    std::cout << "Checking for plagiarization of filename: " << filename << std::endl;
    std::cout << std::endl;

    start = std::clock();
    checkPlagiarization(filename,map);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << std::endl;
    std::cout << "Checking for plagiarization took " << duration << " seconds.";

    //02.txt <-> 08.txt (Whole paragraph)
    //06.txt <-> 02.txt (Sentence)
}


/*****************************************************
* FUNCTION DEFINITIONS                               *
* Lab3 Part2                                         *
******************************************************/
void genNumericalFiles(std::unordered_map<std::string, int> &hashtable) {
    for (unsigned int i = 1; i <= 10; i++) {
        std::ifstream input_file;
        std::ofstream output_file;

        (i == 10) ? input_file.open("../text/" + std::to_string(i) + ".txt") : input_file.open(
                "../text/0" + std::to_string(i) + ".txt");

        (i == 10) ? output_file.open("../converted/" + std::to_string(i) + ".txt") : output_file.open(
                "../converted/0" + std::to_string(i) + ".txt");

        std::string line;
        while (getline(input_file, line, '.')) {

            preprocessSentence(line);

            std::istringstream iss(line);
            std::vector<std::string> temp{std::istream_iterator<std::string>{iss},
                                          std::istream_iterator<std::string>{}};

            convert2numerical(hashtable, temp, output_file);
        }
        input_file.close();
        output_file.close();
    }

}

void convert2numerical(std::unordered_map<std::string,int> &hashtable, const std::vector<std::string>& vec,std::ofstream &outputFile){
    for_each(vec.begin(), vec.end(), [&hashtable, &outputFile](std::string word){
        auto temp = hashtable.find(word);
        if(temp == hashtable.end()) { //Not found, put it in hash table.
            hashtable.insert(std::make_pair(word, hashtable.size()));
            outputFile <<  hashtable.size()-1 << " ";
        }
        else{
            outputFile <<  temp->second << " ";
        }
    });
    outputFile << std::endl;
}


void checkPlagiarization(const std::string& filename, std::unordered_map<std::string,int> map) {
    std::ifstream input_file("../converted/" + filename);
    std::string filenum = filename.substr(0, filename.size() - 4);
    std::string comp_line, input_line;

    unsigned int row{0}, num_words{0}, comp_row{0}, num_lines{0};

    if (filenum != "10")
        filenum.erase(0, 1);
    while (getline(input_file, input_line)) {

        row++;

        num_words = static_cast<unsigned int>(std::distance(std::istream_iterator<std::string>(std::istringstream(input_line) >> std::ws),
                std::istream_iterator<std::string>()));


        for (unsigned int i = 1; i <= 10; i++) {
            if (i == std::stoi(filenum)) { continue; }
            else {
                std::ifstream compare_file((i == 10 ? "../converted/" : "../converted/0") + std::to_string(i) + ".txt");
                if(!compare_file.is_open()){
                    std::cout << "Could not open compare file, terminating.." << std::endl;
                    exit(0);
                }
                else{
                    while(getline(compare_file, comp_line)){
                        comp_row++;
                        std::string comp_filename;
                        i == 10 ? comp_filename = std::to_string(i)+".txt" : comp_filename = "0"+std::to_string(i)+".txt";

                        if(comp_line == input_line && num_words > 1){
                            num_lines++;
                            std::cout << filename << " at line " << row << " is similar to " << comp_filename << " at line " << comp_row << std::endl;
                            std::cout << "Sentence of interest:" << std::endl;
                            std::istringstream iss(input_line);
                            std::vector<std::string> temp{std::istream_iterator<std::string>{iss},
                                                          std::istream_iterator<std::string>{}};

                            for(auto& word : temp){
                                std::for_each(map.begin(), map.end(), [&word](auto p){
                                    if(std::to_string(p.second) == word)
                                        std::cout << p.first << " ";
                                });
                            }

                            std::cout << std::endl;
                            std::cout << std::endl;

                        }
                    }
                }
                compare_file.close();
            }
            comp_row = 0;
        }
    }
    input_file.close();
    std::cout << "------------------------------------" << std::endl;
    std::cout << "The file " << filename << " contains ";
    std::cout << (static_cast<float>(num_lines)/ static_cast<float>(row)) * 100 << "% content that is plagiarized" << std::endl;
}


void preprocessSentence(std::string& sentence){
    sentence.erase(std::remove_if(sentence.begin(), sentence.end(), ispunct), sentence.end());
    std::transform(sentence.begin(), sentence.end(), sentence.begin(), [](auto c) {
        return std::tolower(c);
    });
}