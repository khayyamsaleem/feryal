#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <set>
#include <cstring>
#include "projlex.h"

using namespace std;

ostream& operator<<(ostream& out, const Token& tok){
        if (tok == PLUS)        out << "PLUS";
        else if (tok == MINUS)  out << "MINUS";
        else if (tok == SET)    out << "PRINT";
        else if (tok == REPEAT) out << "REPEAT";
        else if (tok == IDENT)  out << "IDENT(" << tok.GetLexeme() << ")";
        else if (tok == ICONST) out << "ICONST(" << tok.GetLexeme() << ")";
        else if (tok == SCONST) out << "SCONST(" << tok.GetLexeme() << ")";
        else if (tok == STAR)   out << "STAR";
        else if (tok == COLON)  out << "COLON";
        else if (tok == LSQ)    out << "LSQ";
        else if (tok == RSQ)    out << "RSQ";
        else if (tok == SC)     out << "SC";
        else if (tok == VAR)    out << "VAR";
        else if (tok == PRINT)  out << "PRINT";
        return out;
}

Token getNextToken(istream& in, int *linenumber){
    char c;
    string lexeme;
    stringstream ss;
    string current;

    while(true){ //repeat forever or until asked to break
        c = in.get();
        if (c == '#'){ //if we see a comment, skip to the next line
            ++(*linenumber);
            in.ignore(256, '\n');
        } else {
            ss << c;
            ss >> current;
            switch (c) {
                case '+':
                    return Token(PLUS, current, *linenumber);
                case '-':
                    return Token(MINUS, current, *linenumber);
                case '*':
                    return Token(STAR, current, *linenumber);
                case ':':
                    return Token(COLON, current, *linenumber);
                case '(':
                    return Token(LPAREN, current, *linenumber);
                case ')':
                    return Token(RPAREN, current, *linenumber);
                case '[':
                    return Token(LSQ, current, *linenumber);
                case ']':
                    return Token(RSQ, current, *linenumber);
                case ';':
                    return Token(SC, current, *linenumber);
                default:
                    unsigned char n;
                    if(isalpha(c)){
                        lexeme += current;
                        while(true){
                            n = in.peek();
                            if(isalnum(n)){
                                lexeme += n;
                                c = in.get();
                            } else {
                                if (lexeme == "print") return Token(PRINT, lexeme, *linenumber);
                                if (lexeme == "set") return Token(SET, lexeme, *linenumber);
                                return Token(IDENT, lexeme, *linenumber);
                            }
                        }
                    } else if(isdigit(c)){
                        lexeme += current;
                        while(true){
                            n = in.peek();
                            if(isdigit(n)){
                                lexeme += n;
                                current = in.get();
                            } else {
                                return Token(ICONST, lexeme, *linenumber);
                            }
                        }
                    } else if(c == '"') {
                        do {
                            c = in.get();
                            if(c == '\n'){
                                cout << "Error on line " << *linenumber << " (" << lexeme << ")" << endl;
                                return Token(ERR, lexeme, *linenumber);
                            } else {
                                if (c != '"') lexeme += current;
                            }
                        } while (c != '"');
                        return Token(SCONST, lexeme, *linenumber);
                    } else if (c == '\n') {
                        ++(*linenumber);
                    } else if (in.eof()){
                        return Token(DONE, lexeme, *linenumber);
                    }
            }
        }
    }
}

void checkOpt(set<string>& s){
    //checking if all flags are valid
    static string const v[] = {"-v","-mci","-sum"};
    set<string> valid(std::begin(v),std::end(v));
    for(set<string>::iterator it=s.begin(); it!= s.end(); ++it) {
        if(valid.find(*it) == valid.end()){ //checking if the flag entered was valid
            cerr << '-' << *it << " INVALID FLAG" << endl;
            exit(1);
        }
    }
}

void getStats(string fileName, set<string>& s){
    ifstream infile(fileName);
    int *curline = 0;
    Token tok = Token();
    map<string, int> counts;
    do {
        tok = getNextToken(infile, curline);
        if (tok == PLUS)        counts["PLUS"]+=1;
        else if (tok == MINUS)  counts["MINUS"]+=1;
        else if (tok == SET)    counts["PRINT"]+=1;
        else if (tok == REPEAT) counts["REPEAT"]+=1;
        else if (tok == IDENT)  counts["IDENT"]+=1;
        else if (tok == ICONST) counts["ICONST"]+=1;
        else if (tok == SCONST) counts["SCONST"]+=1;
        else if (tok == STAR)   counts["STAR"]+=1;
        else if (tok == COLON)  counts["COLON"]+=1;
        else if (tok == LSQ)    counts["LSQ"]+=1;
        else if (tok == RSQ)    counts["RSQ"]+=1;
        else if (tok == SC)     counts["SC"]+=1;
        else if (tok == VAR)    counts["VAR"]+=1;
        else if (tok == PRINT)  counts["PRINT"]+=1;
        if (s.find("-v")!=s.end()) cout << tok << endl;
    } while (tok != ERR || tok != DONE);

}

int main(int argc, char *argv[]){
    set<string> o;
    set<string> *optSet = &o; //holds the opts passed in by user
    const char* fileName = ""; //holds filename
    int file_count = 0;
    for(int i = 1; i < argc; ++i){
        if (argv[i][0] == '-'){ //is it an opt?
            if(strlen(argv[i])==2 || strlen(argv[1])==4){ //is the opt good?
                optSet->insert(argv[i]); //put it in the set of opts
            } else {
                cerr << "INVALID FLAG " << argv[i] << std::endl; //print bad flag
                exit(1); //exit program
            }
        } else { //if it's not an opt, it's a filename
            file_count += 1;
            fileName = argv[i];
        }
    }
    if (file_count > 1){
        cerr << "TOO MANY FILE NAMES" << std::endl;
        exit(1);
    }
    if(fileName){ //if fileName was actually passed
        ifstream infile(fileName); //make stream of file
        if(!infile.good()){ //check if file exists
            std::cerr << "UNABLE TO OPEN " << fileName << std::endl;
            std::exit(1);
        }
        if(infile.peek() == std::ifstream::traits_type::eof()) exit(0);
        infile.close();
    } else {
        exit(1);
    }
    if(!(optSet->empty())) //check if there were any opts
        checkOpt(*optSet);//check if opts are valid and no conflicts
    getStats(fileName, *optSet);
}

