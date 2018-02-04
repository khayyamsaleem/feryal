#include <iostream>
#include <set>
#include <fstream>
#include <cstring>

void checkOpt(std::set<char>& s){
    //checking for conflicting flags
    if((s.find('q') != s.end()) && (s.find('s') != s.end() || s.find('c') != s.end())){
        std::cerr << "CONFLICTING FLAGS" << std::endl;
        std::exit(1);
    }
    //checking if all flags are valid
    static char const v[] = {'q','s','c','p','l'};
    std::set<char> valid(std::begin(v),std::end(v));
    for(std::set<char>::iterator it=s.begin(); it!= s.end(); ++it) {
        if(valid.find(*it) == valid.end()){
            std::cerr << '-' << *it << " INVALID FLAG" << std::endl;
            std::exit(1);
        }
    }
}

int main(int argc, char* argv[]){
    std::set<char> o;
    std::set<char> *optSet = &o; //holds the opts passed in by user
    const char* fileName; //holds filename
    for(int i = 1; i < argc; ++i){
        if (argv[i][0] == '-'){ //is it an opt?
            if(std::strlen(argv[i])==2){ //is the opt one character long?
                optSet->insert(argv[i][1]); //put it in the set of opts
            } else {
                std::cerr << argv[i] << " INVALID FLAG" << std::endl; //print bad flag
                std::exit(1); //exit program
            }
        } else { //if it's not an opt, it's a filename
            fileName = argv[i];
        }
    }
    if(!(optSet->empty())) //check if there were any opts
        checkOpt(*optSet);//check if opts are valid and no conflicts
    if(fileName){
        std::ifstream infile(fileName); //make stream of file
        if(!infile.good()){ //check if file exists
            std::cerr << fileName << " CANNOT OPEN" << std::endl;
            std::exit(1);
        }
    }
    delete optSet;
    std::exit(0);
}
