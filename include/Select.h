#pragma once

#include "Cursor.h"
#include "DocumentManager.h"

struct coordinates {
    int lineStart;
    int lineEnd;
    int colStart;
    int colEnd;
};
class Select
{//poate e mai bine sa am un pointert cu coordonatele permanente?
    Cursor* cursor_data;
    DocumentManager* doc_data;
    int line_start, col_start, line_stop, col_stop;
    int balance_point;
    coordinates currCoord;
public:
    Select(Cursor*cursor_data, DocumentManager* doc_data);

    void Log();
    
    bool moveH(int distance);
    bool moveV(int distance);
    bool isSelected();
    coordinates* getSelectionCoordinates();
    bool newSelection(int line, int col);
    bool newSelectionOnCursor();
    void resetSelection();
    //add selection to vector?
    void updateSelectionOnCursor();
    void updateSelectionCoordinates();
};