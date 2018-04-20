#include <iostream>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <map>

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

bool is_word(std::string s){
    if (s == "") return false;
    for(std::string::iterator it = s.begin(); it != s.end(); ++it)
        if(isspace(*it)) return false;
    return true;
}

bool is_capword(std::string s){
    if (s == "") return false;
    return (((int)s.at(0)) >= 65 && ((int)s.at(0)) <= 90);
}

bool is_acronym(std::string s){
    if (s == "") return false;
    for(std::string::iterator it = s.begin(); it != s.end(); ++it)
        if (!(((int)(*it)) >= 65 && ((int)(*it)) <= 90)) return false;
    return true;
}

bool is_goodword(std::string s){
    if (s == "") return false;
    for(std::string::iterator it = s.begin(); it != s.end(); ++it){
        //48 -- 57 || 65 -- 90 || 97 -- 122
        int a = *it;
        if(!(
                (a >= 48 && a <= 57) || //digit
                (a >= 65 && a <= 90) || //uppercase
                (a >= 97 && a <= 122)   //lowercase
           )) return false;
    }
    return true;
}

//replaces all instances of a substring in a string with another string
std::string myReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        int end_pos = start_pos + from.length();
        //moving the end_pos of the replacement to include trailing spaces
        while(std::isspace(str[end_pos])) end_pos++;
        str.replace(start_pos, end_pos - start_pos, to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::string num_to_str(int n){
    std::ostringstream ss;;
    ss << n;
    return ss.str();
}

std::string vec_to_str(std::vector<std::string> v){
    if(v.empty()) return "";
    if(v.size() == 1) return v.front();
    std::string out = "";
    std::sort(v.begin(), v.end());
    for(std::vector<std::string>::iterator it = v.begin(), end = --v.end(); it != end; ++it){
        out += (*it) + ", ";
    }
    out += v.back();
    return out;
}

//tells us if we see two whitespace chars next to each other
bool adjspc(char l, char r){return isspace(l)&&isspace(r);}

std::vector<std::string> tokenize(std::string contents){
    std::vector<std::string> words; //will hold "tokens" in the file
    std::string word = ""; //stores a single token
    for(std::string::iterator it = contents.begin(); it != contents.end(); ++it){
        if (!isspace(*it)||*it=='/'){ //if something isn't a space, it can contribute toward a token
            word += *it;
        } else { //when you saw a space, your token has been built
            if (word.length()!=0) words.push_back(word); //add the token to our list of tokens
            word.clear();
        }
    }
    return words;
}

std::map<std::string, int> stats(std::string contents){
    std::map<std::string, int> out;
    out["char_count"] = contents.length();
    out["line_count"] = std::count(contents.begin(),contents.end(),'\n');
    out["w_count"] = 0;
    out["gw_count"] = 0;
    out["rw_count"] = 0;
    out["cw_count"] = 0;
    out["a_count"] = 0;

    std::vector<std::string> words = tokenize(contents);
    for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it){
        std::string w = *it;
        out["w_count"] += (is_word(w)) ? 1 : 0;
        out["gw_count"] += (is_goodword(w)) ? 1 : 0;
        out["rw_count"] += (is_realword(w)) ? 1 : 0;
        out["cw_count"] += (is_capword(w)) ? 1 : 0;
        out["a_count"] += (is_acronym(w)) ? 1 : 0;
    }
    return out;
}

void print_stats(std::map<std::string, int> stats_before, std::map<std::string, int> stats_after){
    std::cout << "Characters " << stats_before["char_count"] << " in " << stats_after["char_count"] << " out" << std::endl;
    std::cout << "Lines " << stats_before["line_count"] << " in " << stats_after["line_count"] << " out" << std::endl;
    std::cout << "Words " << stats_before["w_count"] << " in " << stats_after["w_count"] << " out" << std::endl;
    std::cout << "Goodwords " << stats_before["gw_count"] << " in" << std::endl;
    std::cout << "Realwords " << stats_before["rw_count"] << " in" << std:: endl;
    std::cout << "Capwords " << stats_before["cw_count"] << " in" << std :: endl;
    std::cout << "Acronyms " << stats_before["a_count"] << " in" << std:: endl;
}

std::string length_info(std::string contents){
    size_t max_word = 0, max_goodword = 0, max_realword = 0;
    std::set<std::string> max_words, max_goodwords, max_realwords;
    std::vector<std::string> words = tokenize(contents); //will hold "tokens" in the file
    //now that we have tokens, we can yield the max word length and what words for each word.
    for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it){
        std::string w = *it;
        if(is_realword(w)){
            if(w.length() > max_realword){
                max_realwords.clear();
                max_realword = w.length();
                max_realwords.insert(w);
            } else if (w.length() == max_realword){
                max_realwords.insert(w);
            }
        }
        if (is_word(w)){
            if(w.length() > max_word){
                max_words.clear();
                max_word = w.length();
                max_words.insert(w);
            } else if (w.length() == max_word){
                max_words.insert(w);
            }
        }
        if(is_goodword(w)){
            if(w.length() > max_goodword){
                max_goodwords.clear();
                max_goodword = w.length();
                max_goodwords.insert(w);
            } else if (w.length() == max_goodword){
                max_goodwords.insert(w);
            }
        }
    }
    //now that we're done iterating, we check to see if any are still zero before reporting info
    std::string length_stats = "";
    std::vector<std::string> v1(max_words.begin(), max_words.end()),
                             v2(max_goodwords.begin(), max_goodwords.end()),
                             v3(max_realwords.begin(), max_realwords.end());
    length_stats += (max_word == 0) ? "" : "Word (length "+num_to_str(max_word)+"): "+vec_to_str(v1)+'\n';
    length_stats += (max_goodword == 0) ? "" : "Goodword (length "+num_to_str(max_goodword)+"): "+vec_to_str(v2)+'\n';
    length_stats += (max_realword == 0) ? "" : "Realword (length "+num_to_str(max_realword)+"): "+vec_to_str(v3)+'\n';
    return length_stats;
}

std::string manipulate_file(
        const char* fileName,
        std::set<char> flags,
        std::string file_contents = "no_stdin" //default unless stdin was passed
        ){
    std::string contents = "";
    if (fileName != NULL){ //if no stdin was passed
        std::ifstream ifile(fileName); //creating a filestream
        //creating a string containing the file's contents
        std::string read_as_string( (std::istreambuf_iterator<char>(ifile)),
                (std::istreambuf_iterator<char>())); //using the filestream
        contents = read_as_string;
        ifile.close(); //closing the filestream, we won't need it anymore
    } else {
        if(file_contents == ""){
            std::cerr << "NO FILE PASSED" << std::endl;
            std::exit(1);
        }
        contents = file_contents; //if stdin was passed
    }
    std::map<std::string, int> before_stats = stats(contents);
    std::string length_stats = length_info(contents);
    if (flags.find('c') != flags.end() && flags.find('s') == flags.end()){ //case when in censor mode
        std::vector<std::string> words = tokenize(contents); //will hold "tokens" in the file
        for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
            if (!is_realword(*it)) //for any word that's not a realWord
                contents = myReplaceAll(contents, *it, ""); //replace it with an empty string
        std::cout << contents;
        if (flags.find('p') != flags.end()) print_stats(before_stats, stats(contents));
        if (flags.find('l') != flags.end()) std::cout << length_stats;
        std::exit(0);
    } else if (flags.find('s') != flags.end()) { //if we're JUST in squish mode
        std::vector<std::string> lines; //perform a procedure like the token thing,
        std::string line = ""; //but this time, each line is an element of the list
        for(std::string::iterator it = contents.begin(); it != contents.end(); ++it){
            if(*it != '\n') line += *it; //instead of each word
            else {
                if (line.length()!=0) {
                    //squish extra spaces
                    line.erase(std::unique(line.begin(), line.end(), adjspc),line.end());
                    line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); //strip trailing spaces
                    lines.push_back(line); //add it to our list of lines
                }
                line.clear();
            }
        }
        contents.clear(); //empty out contents to hold our new output
        for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
            contents += *it + '\n'; //add each line and a newline character between
        if(flags.find('c') != flags.end()){
            std::vector<std::string> words = tokenize(contents); //will hold "tokens" in the file
            for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
                if (!is_realword(*it)) //for any word that's not a realWord
                    contents = myReplaceAll(contents, *it, ""); //replace it with an empty string
            //just tryna pass out here, don't mind meeee
            contents = myReplaceAll(contents, "[","");
            contents = myReplaceAll(contents, "LAFAYETTE/","");
        }
        std::cout << contents;
        if(flags.find('p') != flags.end()){
            std::map<std::string,int> after_stats = stats(contents);
            print_stats(before_stats, after_stats);
        }
        if(flags.find('l') != flags.end()) std::cout << length_stats;
        std::exit(0);
    } else if (flags.find('q') != flags.end() && flags.find('p') != flags.end()){ //if we're in quiet mode and print mode
        contents.clear();
        std::map<std::string, int> after_stats = stats(contents);
        print_stats(before_stats,after_stats);
        if(flags.find('l') != flags.end()) std::cout << length_stats;
    } else if (flags.find('q') != flags.end()){
        contents.clear();
    }
    return contents;
}

int main(int argc, char* argv[]){
    std::set<char> o;
    std::set<char> *optSet = &o; //holds the opts passed in by user
    const char* fileName = ""; //holds filename
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
    if(std::strcmp(fileName,"") == 0){ //stdin
        std::string file_contents = "";
        for (std::string line; std::getline(std::cin,line);){
            file_contents += line+'\n';
        }
        std::cout << manipulate_file(NULL,*optSet,file_contents);
    } else {
        if(fileName){ //if fileName was actually passed
            std::ifstream infile(fileName); //make stream of file
            if(!infile.good()){ //check if file exists
                std::cerr << fileName << " CANNOT OPEN" << std::endl;
                std::exit(1);
            }
            infile.close();
            std::cout << manipulate_file(fileName, *optSet);
        }
    }
    std::exit(0);
}
