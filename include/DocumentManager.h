#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include<stack>
#include <functional>
#include "Exceptions.h"

#define NO_CHARS 256

struct Operation {
    function<bool(int, int, const string&)> operation;
    function<bool(int, int, const string&)> oppositeOperation;
};

using namespace std;
class DocumentManager {
	string buffer;
	string filename;
	vector<int>lineBuffer;
	bool isModified;
	int length;
	string copyBuffer; 
	stack<Operation> operationStack;
	stack<tuple<int, int, const string >>arguments;

	public:
	//tools for handling files
    DocumentManager();
	bool create(const string& filename);
	bool open(const string& filename);
	bool save();
	//tools for handling text

	
	bool initializeLineBuffer();
	int getLineCount();
	int getColCount(int line);
	string getBufferSection(int line_start, int col_start, int line_stop, int col_stop);
	string getBuffer();
	string getCopyBuffer();
	vector<int> getLineBuffer();
	bool insertText(int line, int col, const string& inserted);
	bool insertLine(int line, const string& text);
	bool deleteText(int line, int col, int size);
	bool deleteLine(int line);
	string getLine(int line);
	bool swapLines(int line1, int line2, const string& s = "");
	bool swapLineWithSelected(int startSelect, int endSelect, const string& finPos);
	vector<pair<string,int>>tokenize();
	vector<int> searchForWord(const string& word);
	bool copy(int start, int end);
	bool paste(int position);
	bool cut(int start, int end, const string& s ="");
	
	vector<int>badCharacterHeuristic(const string& pattern);
	vector<int>failureF(const string& pattern);
	vector<int>goodSuffixHeuristic(const string& pattern);
	bool findUsingBM(const string& text, const string& pattern, int&i, int n, int m);
	vector<int> find(const string& text, const string& pattern);
	
	bool addText(int line, int col, const string& text);
	bool removeText(int line, int col, const string& text);
	bool atomicSwap(int line1, int line2, const string& text = "");
	bool atomicSwapLinesWithSelected(int startSelect, int endSelect, const string& finPos);
	void logs();
};

