#include "../include/Cursor.h"

Cursor::Cursor(DocumentManager *data)
{
    this->data = data;
    line = 0;
    col = 0;
}

void Cursor::log()
{
    cout<<"Line: "<< line << " Col:"<<col<<'\n';
}
int Cursor::getLine()
{
    return line;
}
int Cursor::getCol()
{
    return col;
}
bool Cursor::moveToWord(int direction)
{
    vector<int> temp_lineBuffer = data->getLineBuffer();
    string temp_buffer = data->getBuffer();
    string separators = " .,/?!;:\"(){}[]%+=\n";
    if(direction > 0)
    {
        int pos = temp_buffer.find_first_of(separators, temp_lineBuffer[line] + col + 1);
        if( pos != string::npos && pos != -1)
        {
            col = pos;
            line = 0;
            while(col >= data->getColCount(line))
            {
                col -= data->getColCount(line);
                line++;
            }
            return true;
        }
    }
    else if(direction < 0)
    {
        int last_word = 0;
        int pos = 0;
        do
        {
            last_word = pos;
            pos = temp_buffer.find_first_of(separators,last_word + 1);
        } while (pos >= 0 && pos < temp_lineBuffer[line] + col);
        
        if( last_word != string::npos && last_word != -1)
        {
            col = last_word;
            line = 0;
            while(col >= data->getColCount(line))
            {
                col -= data->getColCount(line);
                line++;
            }
            return true;
        }
    }
    return false;
}
bool Cursor::moveH(int direction)
{
    if(line == 0 && col + direction < 0)
        return false;
    if(line == data->getLineCount())
        return false;
    col += direction;

    if(col < 0)
    {
        col = data->getColCount(line) + col + 1;
        line--;
    }
    if( col >= data->getColCount(line))
    {
        col = col - data->getColCount(line);
        line++;
    }
    return true;
}
bool Cursor::moveV(int direction)
{
    if(line + direction < 0)
        return false;
    if(line + direction >= data->getLineCount())
        return false;
    line += direction;
    col = min(col, data->getColCount(line) - 1);
    return true;
}

bool Cursor::jumpTo(int where)
{
    switch(where)
    {
        case 0 :
        {
            line = 0;
            col = 0;
            return true;
        }
        case 1 :
        {
            line = data->getLineCount() - 1;
            col = data->getColCount(line) - 1;
            return true;
        }
        case 2 :
        {
            col = 0;
            return true;
        }
        case 3 :
        {
            col = data->getColCount(line) - 1;
            return true;
        }
        default:
            return false;
    }
}