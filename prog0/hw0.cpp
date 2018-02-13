//Feryal Chaudhry
//CS280 Programming Assignment 0
#include <iostream> //default, used to collect input from command line
#include <cstdlib> //standard library, contains functions like atoi and strcmp
#include <map> //used to create the association between type (small, med, large) and length
#include <string.h> //used to be able to say string instead of char*
using namespace std;

//READ STARTING FROM BELOW THE DASHED LINE FIRST, THEN SCROLL BACK UP TO STARRED LINE
//READ STARTING FROM BELOW THE DASHED LINE FIRST, THEN SCROLL BACK UP TO STARRED LINE
//READ STARTING FROM BELOW THE DASHED LINE FIRST, THEN SCROLL BACK UP TO STARRED LINE
//READ STARTING FROM BELOW THE DASHED LINE FIRST, THEN SCROLL BACK UP TO STARRED LINE

//****************************************************************************************

// this function takes in the list of words and filters them by length
// it takes 3 arguments:
// type: the type specified by the user (small, medium, large)
// check: the list of arguments passed (argv from the main function)
// len: the length of the list of arguments passed (argc from the main function)
void filter(string type, char* check[], int len){
    //the next four lines create an association from the "type" to the corresponding length
    //as dictated in the assignment.
    //it is a MAP from STRINGS (small,medium,large) to
    //SIZE_T(just a fancy int, specifically for sizes of things)
    map<string,size_t> sizes;
    sizes["small"]=5;
    sizes["medium"]=10;
    sizes["large"]=20;
    //a loop that iterates from the first word after the type (s,m,l)
    //to the end of "check" (list of args we got from the main function)
    for(int i = 2; i < len; i++){
        //checks if string we're looking at is longer than the length
        //associated with the type specifier (s,m,l)
        //requires "cast" from char* to string to be able to use string's length function
        if(((string)check[i]).length() >= sizes[type])
            cout << check[i] << endl;; //prints word if it passed the check
    }
}


//---------------------------------------------------------------------------------------


// argc is the number of arguments passed to the program from the command line
// it is always at least 1, because the name of the program itself (hw1 in this case)
// is always passed as an argument. this is at index zero in the argument vector, argv
// argv contains all of the command line arguments passed to the program, including
// the program name itself. so, if someone ran: ./hw1 a b c q, argv would look like:
// ["hw1", "a", "b", "c", "q"] , with indices:
// [  0,    1,   2,   3,   4 ]

//C++ knows that when it sees a main function, it can optionally accept command line args.
//these args are stored in argv, which is just an array of strings.
//a string's type in C++ is either string or char*.
//char* just means a sequence of characters. More abstractly, it is a point to the beginning of a
//sequence of characters. that "pointer" knows when a word ends because strings are "null-terminated",
//meaning they end in a hidden character that looks like '\0' (a null terminator).
//argc is just bound to the length of argv by default.
int main(int argc, char* argv[]){
    // ERROR CHECKING
    if(!argv[1]){ //CHECKS IF FIRST ARGUMENT EXISTS
        cerr << "MISSING SIZE" << endl; //PRINTS ERROR
        return 1; //ENDS PROGRAM
    }
    if(!(strcmp(argv[1],"small") == 0 || //CHECKS IF ARGUMENT IS VALID
         strcmp(argv[1],"medium") == 0|| //CHECKS IF ARGUMENT IS VALID
         strcmp(argv[1],"large") == 0)){ //CHECKS IF ARGUMENT IS VALID
        cerr << argv[1] << " NOT A VALID SIZE" << endl; //PRINTS ERROR
        return 1; //ENDS PROGRAM
    }
    if(argc == 2) return 0; //case when user has entered no words after the first argument
    filter(argv[1], argv, argc); //passing our stuff to the filter function
    return 0; //ending the program
}
