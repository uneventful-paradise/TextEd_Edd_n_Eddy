#pragma once
#include "DocumentManager.h"
#include <string>

class Cursor
{
private:
    DocumentManager* data;
    int line, col;
public:
    Cursor(DocumentManager* data);
    enum PosToJmp {StartOfFile, EndOfFile, StartOfParagraph, EndOfParagraph};

    int getLine();
    int getCol();
    void log();


    bool moveH(int direction);
    bool moveV(int direction);
    bool moveToWord(int direction);
    bool jumpTo(int where);
    
};