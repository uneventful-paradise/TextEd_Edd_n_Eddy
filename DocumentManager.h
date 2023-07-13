#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class DocumentManager {
	string buffer;
	string filename;
	vector<int>lineBuffer;
	bool isModified;
	int lenght;
public:
    DocumentManager();
	bool create(const string& filename);
	bool open(const string& filename);
	void modify();
	bool save();
};

