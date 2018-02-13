#include <iostream>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>

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

std::string myReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        int end_pos = start_pos + from.length();
        while(std::isspace(str[end_pos])) end_pos++;
        str.replace(start_pos, end_pos - start_pos, to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

bool adjspc(char l, char r){return isspace(l)&&isspace(r);}

std::string manipulate_file(const char* fileName, std::set<char>& flags){
    std::ifstream ifile(fileName);
    std::string contents( (std::istreambuf_iterator<char>(ifile)),
            (std::istreambuf_iterator<char>()));
    ifile.close();
    if (flags.find('c') != flags.end()){
        if (flags.find('s') != flags.end()) contents.erase(std::unique(contents.begin(), contents.end(), adjspc),contents.end());
        std::vector<std::string> words;
        std::string word = "";
        for(std::string::iterator it = contents.begin(); it != contents.end(); ++it){
            if (!isspace(*it)){
                word += *it;
            } else {
                if (word.length()!=0) words.push_back(word);
                word.clear();
            }
        }
        for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
            if (!is_realword(*it))
                contents = myReplaceAll(contents, *it, "");
    } else if (flags.find('s') != flags.end()) {
        std::vector<std::string> lines;
        std::string line = "";
        for(std::string::iterator it = contents.begin(); it != contents.end(); ++it){
            if(*it != '\n') line += *it;
            else {
                if (line.length()!=0) {
                    line.erase(std::unique(line.begin(), line.end(), adjspc),line.end());
                    line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
                    lines.push_back(line);
                }
                line.clear();
            }
        }
        std::string out = "";
        for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
            out += *it + '\n';
        return out;
    } else if (flags.find('q') != flags.end()){
        contents.clear();
    }
    return contents;
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
        std::cout << manipulate_file(fileName, *optSet);
    }
    std::exit(0);
}
