#include <iostream>
using namespace std;
#include "DocumentManager.h"
#include "SFML/Graphics.hpp"

int main(int argc, char*argv[])
{
    string path; 
    char ch;
    printf("Enter the document's path:\n");
    getline(cin, path);
    if (path == "") {
        printf("no path in input, default path used\n-----------------------------------------------\n");
        path = "D:/code/vs/varinate_sesiune/textEdd/textEdd/testDoc.txt";
    }
    printf("path of exe: %s\n-----------------------------------------------\n", argv[0]);
    DocumentManager document;
    document.open(path);
    document.initializeLineBuffer();
   // document.insertText(0, 2, "da");
   // document.deleteText(1, 1, 13);
    //document.deleteLine(0);
    //document.insertLine(0, "ababab");
    //document.swapLines(1, 3);
    //document.tokenize();
    //document.searchForWord("linia");
    //document.copy(0, 8);
    //document.paste(24); 
    //document.insertText(2, 0, "abc\nd\n");
    /*document.copy(0, 7);
    document.paste(16);*/
    //document.goodSuffixHeuristic("ABABACABA");
    //document.find("MAMAMAMA", "MAMA");
    //document.insertText(0, 0, "test1");
    
    //document.insertText(0, 0, "test2");
   // document.deleteText(2, 3, 2);
    //document.insertText(3, 0, "inserted\n");
   // document.swapLines(0, 1);//pot folosi op cu line in loc de add si remove
    document.swapLineWithSelected(2, 3, "1");
    document.swapLineWithSelected(5, 6, "0");
    document.logs();
    return 0;
}