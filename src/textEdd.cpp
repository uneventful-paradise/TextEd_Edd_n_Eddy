#include <iostream>
using namespace std;
#include "DocumentManager.h"


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
    document.insertText(20, 2, "da");
    //document.deleteText(1, 1, 13);
    //document.deleteLine(0);
    //document.insertLine(0, "ababab");
    //document.swapLines(1, 3);
    //document.tokenize();
    //document.searchForWord("linia");
    //document.copy(0, 8);
    //document.paste(24);

    //document.insertText(2, 0, "abc\nd\n");
    //document.copy(0, 7);
    //document.paste(16);
    //document.goodSuffixHeuristic("ABABACABA");
    document.find("MAMAMAMA", "MAMA");
    return 0;
}