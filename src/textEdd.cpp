#include <iostream>
#include <chrono>
#include <ctime>
using namespace std;
#include "../include/DocumentManager.h"
#include "../include/Cursor.h"
#include "../include/Select.h"

int main(int argc, char*argv[])
{



    string path; 
    path = "../assets/testDoc.txt";
    DocumentManager document;
    document.open(path);
    document.initializeLineBuffer();
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
    Cursor test(&document);
    int dir;

    while(true)
    {
        test.log();
        cout<<"move cursor: \n";
        cin>>dir;
        switch(dir)
        {
            case 1: 
            {
                test.moveToWord(1);
                break;
            }
            case 2: 
            {
                test.moveToWord(-1);
                break;
            }
            case 3: 
            {
                test.jumpTo(Cursor::StartOfParagraph);
                break;
            }
            case 4: 
            {
                test.jumpTo(Cursor::EndOfParagraph);
                break;
            }
            default:
                break;
        }
        
    }
    return 0;
}
