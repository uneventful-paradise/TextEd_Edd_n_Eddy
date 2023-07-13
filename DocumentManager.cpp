#include "DocumentManager.h"

DocumentManager::DocumentManager() {
	buffer = "";
	filename = "";
	lineBuffer = {};
	isModified = false;
	lenght = 0;
}

bool DocumentManager::open(const string& filename){
	ifstream file(filename);
	//ofstream file(filename);
	if (!file) {
		printf("Error at opening file");
		return false;
	}
	else {
		stringstream buf;
		string word;
		buf << file.rdbuf();
		while (getline(buf, word)) {
			buffer += word;
			buffer += "\n";
		}
		//printf(buffer.c_str());
		return true;
	}
	return false;
}

bool DocumentManager::create(const string& filename)
{

	return false;
}

void DocumentManager::modify()
{
}

bool DocumentManager::save()
{
	return false;
}
