#include "../include/Select.h"

Select::Select(Cursor& cursor_data, DocumentManager& doc_data)
{
    this->cursor_data = &cursor_data;
    this->doc_data = &doc_data;
    line_start = cursor_data.getLine();
    line_stop = line_start;
    col_start = cursor_data.getCol();
    col_stop = col_start;
    balance_point = 0;
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
    if(balance_point > 0 || move_if_zero == 1)
    {
        if( line_stop == 0 && col_stop + distance < 0)
            return false;
        if( line_stop == doc_data->getLineCount() && col_stop + distance >= 0)
            return false;
        
        col_stop += distance;
        
        if( col_stop >= doc_data->getColCount(line_stop))
        {
            col_stop = doc_data->getColCount(line_stop) - col_stop;
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
