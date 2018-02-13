#include <iostream>
#include <iterator>
#include <ctype.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string.h>
using namespace std;

bool isWord(string word)
{
    for(size_t i = 0; i < word.length(); i++)
        if(isspace(word[i]))
            return false;
    return true;
}

bool isGoodWord(string word)
{
    for(size_t i = 0; i < word.length(); i++)
        if(!isalnum(word[i]))
            return false;
    return true;
}

bool isRealWord(string word)
{
    for(size_t i = 0; i < word.length(); i++)
        if(!isalpha(word[i]))
            return false;
    return true;
}

bool isCapWord(string word)
{
    if(isupper(word[0]))
        return true;
    return false;
}

bool isAcronym(string word)
{
    for(size_t i = 0; i < word.length(); i++)
        if(!isupper(word[i]))
            return false;
    return true;
}

string printFile(string filename)
{
    ifstream ifile(filename);
    string line;
    string out = "";
    while(getline(ifile, line))
        out += line + '\n';
    ifile.close();
    return out;
}


bool adjspc(char l, char r){return isspace(l)&&isspace(r);}

string removeSpaces(string s)
{
    s.erase(std::unique(s.begin(), s.end(), adjspc), s.end());
    return s;
}

string squishFile(string filename)
{
    ifstream ifile(filename);
    string line;
    string out = "";
    while(getline(ifile, line)){
        line = removeSpaces(line);
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        out += line;
        if(ifile.peek() != '\n') out += '\n';
    }
    ifile.close();
    return out;
}

string myReplaceAll(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        int end_pos = start_pos + from.length();
        while(isspace(str[end_pos])) end_pos++;
        str.replace(start_pos, end_pos - start_pos, to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

string censorFile(string filename, bool withSquish){
    ifstream ifile(filename);
    string contents( (istreambuf_iterator<char>(ifile)),
                       (istreambuf_iterator<char>()));
    ifile.close();
    if (withSquish) contents = squishFile(filename);
    vector<string> words;
    string word = "";
    string out = "";
    for(string::iterator it = contents.begin(); it != contents.end(); ++it){
        if (!isspace(*it)){
            word += *it;
        } else {
            if (word.length()!=0) words.push_back(word);
            word = "";
        }
    }
    for(vector<string>::iterator it = words.begin(); it != words.end(); ++it){
        if (!isRealWord(*it)){
            contents = myReplaceAll(contents, *it, "");
        }
    }
    return contents;
    // cout << "[ ";
    // for (vector<string>::iterator it = words.begin(); it != words.end(); ++it) cout << "("<<*it<<")" << " ";
    // cout << "]";
}

int main(int argc, char *argv[])
{
    bool isQuietMode = false, isSquishMode = false, isCensorMode = false, isPrintMode = false, isLengthMode = false;
    ifstream ifile;
    char filename[30];
    /*bool isExtraSpaceFileBoolean;*/
    for(int i=1; i< argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(strlen(argv[i]) == 2)
            {
                if(argv[i][1] == 'q')
                {
                    isQuietMode = true;
                }
                else if(argv[i][1] == 's')
                {
                    isSquishMode = true;
                }
                else if(argv[i][1] == 'c')
                {
                    isCensorMode = true;
                }
                else if(argv[i][1] == 'p')
                {
                    isPrintMode = true;
                }
                else if(argv[i][1] == 'l')
                {
                    isLengthMode = true;
                }
                else
                {
                    cout << argv[i] << " INVALID FLAG" << endl;
                    exit(1);
                }
            }
            else
            {
                cout << argv[i] << " INVALID FLAG" << endl;
                exit(1);
            }
        }
        else
        {
            ifile.open(argv[i]);
            if(ifile.fail())
            {
                cout << argv[i] << " CANNOT OPEN" << endl;
                exit(1);
            }
            else
            {
                strcpy(filename, argv[i]);
            }
            if(i != (argc-1))
            {
                cout << "TOO MANY FILES" << endl;
                exit(1);
            }

        }
    }
    //isExtraSpaceFileBoolean = isExtraSpaceFile(filename);
    if(( isSquishMode || isCensorMode ) && isQuietMode){
        cout << "CONFLICTING FLAGS" << endl;
        exit(1);
    }
    else if(isQuietMode && !isPrintMode && !isLengthMode)
        return 0;
    else if(!(isQuietMode || isSquishMode || isCensorMode || isPrintMode || isLengthMode))
    {
        cout << printFile(filename);
        exit(0);
    }
    else if(/*isExtraSpaceFileBoolean &&*/ isSquishMode && isCensorMode)
    {
        cout << censorFile(filename, true);
        exit(0);
    }
    else if(/*isExtraSpaceFileBoolean &&*/ isSquishMode)
    {
        cout << squishFile(filename);
        exit(0);
    }
    else if(isCensorMode)
    {
        cout << censorFile(filename, false);
        exit(0);
    }
    return 0;
}
