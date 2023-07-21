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
    
    bool moveMultipleH(int direction);
    bool moveMultipleV(int direction);

    int getLine();
    int getCol();
    void log();


    bool moveH(int direction);
    bool moveV(int direction);
    bool moveToWord(int direction);
    bool jumpTo(int where);
    void updateCoordinates(int line, int col);
};