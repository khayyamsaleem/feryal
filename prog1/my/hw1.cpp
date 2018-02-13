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
        if(valid.find(*it) == valid.end()){ //checking if the flag entered was valid
            std::cerr << '-' << *it << " INVALID FLAG" << std::endl;
            std::exit(1);
        }
    }
}

//predicate that tells if a string is a realWord using ascii table value ranges
bool is_realword(std::string s){
    for(std::string::iterator it = s.begin(); it != s.end(); ++it){
        int ascii = *it;
        if (!((ascii >= 65 && ascii <= 90) || (ascii >= 97 && ascii <= 122)))
            return false;
    }
    return true;
}

//replaces all instances of a substring in a string with another string
std::string myReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        int end_pos = start_pos + from.length();
        while(std::isspace(str[end_pos])) end_pos++; //moving the end_pos of the replacement to include trailing spaces
        str.replace(start_pos, end_pos - start_pos, to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

bool adjspc(char l, char r){return isspace(l)&&isspace(r);} //tells us if we see two whitespace chars next to each other

std::string manipulate_file(const char* fileName, std::set<char>& flags){
    std::ifstream ifile(fileName); //creating a filestream
    std::string contents( (std::istreambuf_iterator<char>(ifile)), //creating a string containing the file's contents
            (std::istreambuf_iterator<char>())); //using the filestream
    ifile.close(); //closing the filestream, we won't need it anymore
    if (flags.find('c') != flags.end()){ //case when in censor mode
        if (flags.find('s') != flags.end()) //checking to see if we're also in squish mode
            contents.erase(std::unique(contents.begin(), contents.end(), adjspc),contents.end()); //mild squishery
        std::vector<std::string> words; //will hold "tokens" in the file
        std::string word = ""; //stores a single token
        for(std::string::iterator it = contents.begin(); it != contents.end(); ++it){
            if (!isspace(*it)){ //if something isn't a space, it can contribute toward a token
                word += *it;
            } else { //when you saw a space, your token has been built
                if (word.length()!=0) words.push_back(word); //add the token to our list of tokens
                word.clear();
            }
        }
        for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
            if (!is_realword(*it)) //for any word that's not a realWord
                contents = myReplaceAll(contents, *it, ""); //replace it with an empty string
    } else if (flags.find('s') != flags.end()) { //if we're JUST in squish mode
        std::vector<std::string> lines; //perform a procedure like the token thing,
        std::string line = ""; //but this time, each line is an element of the list
        for(std::string::iterator it = contents.begin(); it != contents.end(); ++it){
            if(*it != '\n') line += *it; //instead of each word
            else {
                if (line.length()!=0) {
                    line.erase(std::unique(line.begin(), line.end(), adjspc),line.end()); //squish extra spaces
                    line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); //strip trailing spaces
                    lines.push_back(line); //add it to our list of lines
                }
                line.clear();
            }
        }
        contents.clear(); //empty out contents to hold our new output
        for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
            contents += *it + '\n'; //add each line and a newline character between
    } else if (flags.find('q') != flags.end()){ //if we're in quiet mode, we want to print nothing
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
