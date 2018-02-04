#include <iostream>
#include <ctype.h>
#include <fstream>
#include <cstring>
using namespace std;

bool isWord(string word){
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

int main(int argc, char *argv[])
{    
    bool isQuietMode = false, isSquishMode = false, isCensorMode = false, isPrintMode = false, isLengthMode = false;
    ifstream ifile;
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
                    break;
                }
            }
            else
            {
                cout << argv[i] << " INVALID FLAG" << endl;
                break;
            }
        }
        else
        {
            ifile.open(argv[i]);	
            if(ifile.fail())
            {
                cout << argv[i] << " CANNOT OPEN" << endl;
            }
        }
    }
    (void)isPrintMode; (void)isLengthMode;
    if(( isSquishMode || isCensorMode ) && isQuietMode)
        cout << "CONFLICTING FLAGS" << endl;
    return 0;
}
