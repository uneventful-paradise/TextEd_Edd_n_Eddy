#pragma once

#include "Cursor.h"
#include "DocumentManager.h"

class Select
{
    Cursor* cursor_data;
    DocumentManager* doc_data;
    int line_start, col_start, line_stop, col_stop;
    int balance_point;
public:
    Select(Cursor& cursor_data, DocumentManager& doc_data);

    void Log();
    
    bool moveH(int distance);
    bool moveV(int distance);
};