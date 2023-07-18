#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
using namespace std;

class DocumentManager {
	string buffer;
	string filename;
	vector<int>lineBuffer;
	bool isModified;
	int length;
	string copyBuffer; 
public:
	//tools for handling files
    DocumentManager();
	bool create(const string& filename);
	bool open(const string& filename);
	bool save();
	//tools for handling text
	//7. charsInLine
	//8. buffer position
	
	//TO-DO : save the document after each change (insertion /deletion)(via isChanged) or before closing
	bool initializeLineBuffer();
	int getLineCount();
	int getColCount(int line);
	string getBufferSection(int line_start, int col_start, int line_stop, int col_stop);
	string getBuffer();
	vector<int> getLineBuffer();
	bool insertText(int line, int col, const string& inserted);
	bool insertLine(int line, const string& text);//overloaded function
	bool deleteText(int line, int col, int size);//stack cu substringuri sterse
	bool deleteLine(int line);
	string getLine(int line);
	bool swapLines(int line1, int line2);
	vector<pair<string,int>>tokenize();//could also be a map?
	vector<int> searchForWord(const string& word);//+pattern matching?
	bool copy(int start, int end);
	bool paste(int position);

	//void undo(vector<pair<bool (*fnc)(), vector<string>arguments>>)
};

