#include <iostream>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>

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

bool is_realword(std::string s){
    for(std::string::iterator it = s.begin(); it != s.end(); ++it){
        int ascii = *it;
        if (!((ascii >= 65 && ascii <= 90) || (ascii >= 97 && ascii <= 122)))
            return false;
    }
    return true;
}

void manipulate_file(const char* fileName, std::set<char>& flags){
    std::ifstream infile(fileName);
    std::string line;
    if (flags.empty()){
        while(std::getline(infile, line)) std::cout << line << std::endl;
        std::exit(0);
    }
    if (flags.find('c') != flags.end()){
       while(std::getline(infile, line)){
           std::string buffer;
           std::stringstream ss(line);
           std::vector<std::string> words;
           while(ss >> buffer)
               words.push_back(buffer);
           for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
               if (is_realword(*it)) std::cout << *it << " ";
           std::cout << std::endl;
       }
       std::exit(0);
    }
    if (flags.find('s') != flags.end()){
        while(std::getline(infile, line)){
            for (std::string::iterator it = line.begin(); it != line.end(); ++it){
                if (!std::isspace(*it) || !std::isspace(*(it+1))) std::cout << *it;
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, char* argv[]){
    std::set<char> o;
    std::set<char> *optSet = &o; //holds the opts passed in by user
    const char* fileName; //holds filename
    int file_count = 0;
    for(int i = 1; i < argc; ++i){
        if (argv[i][0] == '-'){ //is it an opt?
            if(std::strlen(argv[i])==2){ //is the opt one character long?
                optSet->insert(argv[i][1]); //put it in the set of opts
            } else {
                std::cerr << argv[i] << " INVALID FLAG" << std::endl; //print bad flag
                std::exit(1); //exit program
            }
        } else { //if it's not an opt, it's a filename
            file_count += 1;
            fileName = argv[i];
        }
    }
    if (file_count > 1){
        std::cerr << "TOO MANY FILES" << std::endl;
        std::exit(1);
    }
    if(!(optSet->empty())) //check if there were any opts
        checkOpt(*optSet);//check if opts are valid and no conflicts
    if(fileName){
        std::ifstream infile(fileName); //make stream of file
        if(!infile.good()){ //check if file exists
            std::cerr << fileName << " CANNOT OPEN" << std::endl;
            std::exit(1);
        }
        infile.close();
        manipulate_file(fileName, *optSet);
    }
    std::exit(0);
}
