#include "Select.h"
#include "Exceptions.h"

out_of_bounds outOfBounds;

Select::Select(Cursor* cursor_data, DocumentManager* doc_data)
{
    this->cursor_data = cursor_data;
    this->doc_data = doc_data;
    line_start = cursor_data->getLine();
    line_stop = line_start;
    col_start = cursor_data->getCol();
    col_stop = col_start;
    balance_point = 0;
        
    coordinates currCoord;
    currCoord.lineStart = line_start;
    currCoord.lineEnd = line_stop;
    currCoord.colStart = col_start;
    currCoord.colEnd = col_stop;
}


void Select::Log()
{
    cout<< "Start: Line-" << line_start << " Col-" << col_start << " Stop: Line-" << line_stop << " Col-" << col_stop << '\n';
    cout<< "Selected: \n" << doc_data->getBufferSection(line_start, col_start, line_stop, col_stop) << '\n';
}
bool Select::moveH(int distance)
{
    // ai grija ca distanta sa fie mereu 1 altfel echilibrul ar putea sa functioneze cum nu trebuie
    // solutie.. un while care incrementeaza/ decrementeaza cu cate o unitate pana se atinge punctul dorit

    int move_if_zero = 0;
    if(balance_point == 0)
    {
        if(distance > 0)
            move_if_zero = 1;
        else if(distance < 0)
            move_if_zero = 2;
    }
    if(balance_point > 0 || distance > 0)
    {
        if( line_stop == 0 && col_stop + distance < 0)
            return false;
        if( line_stop == doc_data->getLineCount() && col_stop + distance >= 0)
            return false;
        
        col_stop += distance;
        
        if( col_stop >= doc_data->getColCount(line_stop))
        {
            col_stop = col_stop - doc_data->getColCount(line_stop);
            line_stop++;
        }
        if( col_stop < 0)
        {
            col_stop = doc_data->getColCount(line_stop - 1) + col_stop;
            line_stop--;
        }
        balance_point += distance;
        return true;
    }
    else if(balance_point < 0 || move_if_zero == 2)
    {
        if( line_start == 0 && col_start + distance < 0)
            return false;
        if( line_start == doc_data->getLineCount() && col_start + distance >= 0)
            return false;
        
        col_start += distance;
        
        if( col_start >= doc_data->getColCount(line_start))
        {
            col_start = doc_data->getColCount(line_start) - col_start;
            line_start++;
        }
        if( col_start < 0)
        {
            col_start = doc_data->getColCount(line_start - 1) + col_start;
            line_start--;
        }
        balance_point += distance;
        return true;
    }
    return false;
}
bool Select::moveV(int distance)
{
    int move_if_zero = 0;
    if(balance_point == 0)
    {
        if(distance > 0)
            move_if_zero = 1;
        else if(distance < 0)
            move_if_zero = 2;
    }
    if(balance_point > 0 || move_if_zero == 1)
    {
        if(line_stop + distance < 0)
            return false;
        if(line_stop + distance >= doc_data->getLineCount())
            return false;
        
        line_stop += distance;
        col_stop = min(col_stop, doc_data->getColCount(line_stop) - 1);
        
        balance_point += distance;
        return true;
    }
    else if(balance_point < 0 || move_if_zero == 2)
    {
        if(line_start + distance < 0)
            return false;
        if(line_start + distance >= doc_data->getLineCount())
            return false;
        
        line_start += distance;
        col_start = min(col_start, doc_data->getColCount(line_start) - 1);
        
        balance_point += distance;
        return true;
    }
    return false;
}

bool Select::isSelected()
{
    return(currCoord.colEnd != currCoord.colStart or currCoord.lineEnd != currCoord.lineStart);
}

coordinates* Select::getSelectionCoordinates()
{
    return &currCoord;
}

bool Select::newSelection(int line, int col)
{
    try{
        if (line < 0 or line + col > doc_data->getLineBuffer()[doc_data->getLineCount()]) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at newSelection: " <<  e.what();
		return false;
	}	    
    currCoord.colStart = currCoord.colEnd = col;
    currCoord.lineStart = currCoord.lineEnd = line;
    return true;
}

bool Select::newSelectionOnCursor()
{
    int line = cursor_data->getLine();
    int col = cursor_data->getCol();
    return newSelection(line, col);
}

void Select::resetSelection()
{
    int line = cursor_data->getLine();
    int col = cursor_data->getCol();
    currCoord.lineStart = currCoord.lineEnd = line;
    currCoord.colStart = currCoord.colEnd = col;
}

void Select::updateSelectionOnCursor()//use moveH
{
    int line = cursor_data->getLine();
    int col = cursor_data->getCol();
    while (currCoord.lineStart > line) {
        moveV(-1);
        while (currCoord.colStart > col) {
            moveH(-1);
        }
        while (currCoord.colEnd < col) {
            moveH(1);
        }
        updateSelectionCoordinates();
    }
    while (currCoord.lineEnd < line) {
        moveV(1);
        while (currCoord.colStart > col) {
            moveH(-1);
        }
        while (currCoord.colEnd < col) {
            moveH(1);
        }
        updateSelectionCoordinates();
    }
}

void Select::updateSelectionCoordinates()
{
    if (currCoord.lineStart != line_start)currCoord.lineStart = line_start;
    if (currCoord.lineEnd != line_stop)currCoord.lineEnd = line_stop;
    if (currCoord.colStart != col_start)currCoord.colStart = col_start;
    if (currCoord.colEnd != col_stop)currCoord.colEnd = col_stop;
}

