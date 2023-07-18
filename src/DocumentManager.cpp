#include "../include/DocumentManager.h"
/*
TO-DO'S
exceptii
pointer pentru ultima zona modificata pentru salvare
*/
DocumentManager::DocumentManager() {
	buffer = "";
	filename = "";
	lineBuffer = {};
	isModified = false;
	length = 0;
	copyBuffer = "";
}

bool DocumentManager::open(const string& filename){
	ifstream file(filename);
	if (!file) {
		printf("Error at opening file");
		return false;
	}
	else {
		this->filename = filename;
		stringstream buf;
		string word;
		buf << file.rdbuf();
		while (getline(buf, word)) {
			buffer += word;
			buffer += "\n";
		}
		file.close();
		return true;
	}
	return false;
}

bool DocumentManager::create(const string& filename)
{
	ifstream file(filename);
	if (file) {
		printf("%s\n", filename.c_str());
		char ch;
		printf("File already exists. Overwrite? Y/N\n");
		cin >> ch;
		if (ch != 'y' && ch != 'Y') {
			return false;
		}
	}
	this->filename = filename;
	ofstream ow(filename);
	ow.close();
	return true;
}

bool DocumentManager::save()//adauga un pointer pentru ultima zona modificata
{
	ofstream ow(filename);
	ow<<buffer;
	ow.close();
	return false;
}

bool DocumentManager::initializeLineBuffer()
{
	lineBuffer.clear();
	int lineIndex = 0;
	lineBuffer.push_back(lineIndex);
	int bufferSize = buffer.size();
	if (bufferSize == 0)return false;

	for (int i = 0; i < bufferSize; ++i) {
		if (buffer[i] == '\n' || buffer[i] == 10) {
			lineIndex = i + 1;
			lineBuffer.push_back(lineIndex);
		}
	}
	for (int i = 0; i < lineBuffer.size(); ++i) {
		//printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
	}

	//printf("linecount: %d \n", getLineCount());
	return true;
}

int DocumentManager::getLineCount()
{
	return lineBuffer.size()-1;
}

int DocumentManager::getColCount(int line)
{
    return lineBuffer[line + 1] - lineBuffer[line];
}
string DocumentManager::getBufferSection(int line_start, int col_start, int line_stop, int col_stop)
{
	string result = "";

	for(int i = lineBuffer[line_start] + col_start; i < lineBuffer[line_stop] + col_stop; i++)
	{
		result += buffer[i];
	}
	return result;
}
bool DocumentManager::insertText(int line, int col, const string& inserted)//de implementat exceptii.
{
	if (line > getLineCount() || line < 0) {
		printf("Error at insertText : line argument is invalid");
		return false;
	}
	if (line == getLineCount() && col > 0) {
		printf("Error at insertText : line argument is invalid");
		return false;
	}
	if (lineBuffer[line] + col > buffer.size()) {
		printf("Error at insertText : line argument is invalid");
		return false;
	}
	printf("the program tried inserting at line %d, col %d, position %d\n-----------------------------------------------\n", line, col, lineBuffer[line] + col);
	buffer.insert(lineBuffer[line] + col, inserted);
	int insSize = inserted.size();
	bool newLine = false;
	int lineC = getLineCount();
	for (int level = line + 1; level <= lineC; ++level) {
		lineBuffer[level] += insSize;
	}
	for (int i = 0; i < insSize; ++i) {
		if (inserted[i] == '\n' || inserted[i] == 10) {
			newLine = true;
			int pos = lineBuffer[line] + col;
			printf("a newline was found at position %d in the inserted text\n-----------------------------------------------\n", pos + i + 1);
			lineBuffer.push_back(pos + i + 1);
		}
	}
	if (newLine) {
		sort(lineBuffer.begin(), lineBuffer.end());
		printf("resorted lineBuffer vector\n-----------------------------------------------\n");
	}
	for (int i = 0; i < lineBuffer.size(); ++i) {
		printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
	}
	printf("the string after the insertion operation:\n%s\n-----------------------------------------------\n", buffer.c_str());
}

bool DocumentManager::insertLine(int line, const string& text)
{
	if (line < 0 || line > getLineCount()) {
		printf("Error at insertLine: invalid line\n");
		return false;
	}
	int stringSize = text.size();
	printf("stringsize: %d\n", stringSize);
	int pos = lineBuffer[line];
	printf("program tried inserting a line at position: %d\n", pos);
	buffer.insert(pos, text);
	for (int i = line; i <= getLineCount(); ++i) {
		lineBuffer[i] += (stringSize);
	}
	lineBuffer.push_back(pos);
	sort(lineBuffer.begin(), lineBuffer.end());
	for (int i = 0; i < lineBuffer.size(); ++i) {
			printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
		}
	printf("the string after the insertion operation:\n%s\n-----------------------------------------------\n", buffer.c_str());
	return true;
}

bool DocumentManager::deleteText(int line, int col, int size)
{
	if (line < 0 || (lineBuffer[line] + col + size >= lineBuffer[getLineCount()])) {//size ca sa stearga pana la final
		printf("Error at deleteText: invalid line\n");
		return false;
	}
	int startpos = lineBuffer[line] + col;
	int endpos = startpos + size;
	int endlCount = 0;
	for (int i = startpos; i < endpos; ++i) {
		if (buffer[i] == '\n' || buffer[i] == 10) {
			endlCount++;
		}
	}
	printf("the string before the removal operation:\n%s\n-----------------------------------------------\n", buffer.c_str());
	printf("found %d newlines\n-----------------------------------------------\n", endlCount);
	
	if (endlCount) {
		//lineBuffer[line + endlCount] = lineBuffer[line];
		lineBuffer.erase(lineBuffer.begin() + line +1, lineBuffer.begin() + line + endlCount);
	}

	printf("text from %d to %d was deleted\n", startpos, endpos);
	printf("text deleted:\n%s\n", buffer.substr(startpos, endpos - startpos).c_str());
	buffer.erase(buffer.begin() + startpos, buffer.begin() + endpos);
	for (int level = line+1; level <= getLineCount(); ++level) {
		lineBuffer[level] -= size;
	}
	//int newLineCount = getLineCount() - endlCount;
	for (int i = 0; i < lineBuffer.size(); ++i) {
			printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
		}
	printf("the string after the removal operation:\n%s\n-----------------------------------------------\n", buffer.c_str());
	return true;
}

bool DocumentManager::deleteLine(int line)
{
	if (line < 0 || line >= getLineCount()) {
		printf("Error at deleteLine: invalid line\n");
		return false;
	}
	int startpos = lineBuffer[line];
	int endpos = lineBuffer[line+1];
	int size = lineBuffer[line + 1] - lineBuffer[line];
	buffer.erase(buffer.begin() + startpos, buffer.begin() + endpos);
	for (int level = line+1; level <= getLineCount(); ++level) {
		lineBuffer[level] -= size;
	}
	lineBuffer.erase(lineBuffer.begin()+line, lineBuffer.begin()+line+1);
	for (int i = 0; i < lineBuffer.size(); ++i) {
			printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
		}
	printf("the string after the removal operation:\n%s\n-----------------------------------------------\n", buffer.c_str());
	return true;
}

string DocumentManager::getLine(int line)
{
	if (line < 0 || line >= getLineCount()) {
		printf("Error at getLine: invalid line argument\n");
		return string("");
	}
	string result("");
	int startpos = lineBuffer[line];
	int endpos = lineBuffer[line + 1] - 1;
	//printf("startpos: %d, endpos: %d\n", startpos, endpos);
	result = buffer.substr(startpos, endpos-startpos+1);
	printf("the substracted string: %s\n-----------------------------------------------\n", result.c_str());
	return result;
}

bool DocumentManager::swapLines(int line1, int line2)//swaplines pt mai multe
{
	int lineC = getLineCount();
	if ((line1 >= lineC || line1 < 0) || (line2 >= lineC || line2 < 0)) {
		printf("Error at swapLines: invalid line\n");
		return false;
	}
	if (line1 == line2) {
		return true;
	}
	string substr1 = getLine(line1);
	string substr2 = getLine(line2);
	deleteLine(line1);
	insertLine(line1, substr2);
	deleteLine(line2);
	insertLine(line2, substr1);
	for (int i = 0; i < lineBuffer.size(); ++i) {
		printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
	}
}

vector<pair<string, int>> DocumentManager::tokenize()//mai bine cu map?
{
	int beginpos = 0;
	int endpos;
	//ce separatori?
	string word("");
	vector<pair<string, int>>words;
	do {
		endpos = buffer.find_first_of(" .,/?!;:\"(){}[]%+=\n", beginpos);//ce separatori? + merge pt endl?
		if (beginpos != endpos && endpos != string::npos) {
			word = buffer.substr(beginpos, endpos - beginpos);
			pair<string, int>p = { word, beginpos };
			words.push_back(p);//de ce nu &p
		}
		beginpos = endpos + 1;
	} while (endpos != string::npos);
	printf("the tokenized words:\n-----------------------------------------------\n");
	for (auto word : words) {
		printf("the word \"%s\" starts at position %d\n", word.first.c_str(), word.second);
	}
	return words;
}

vector<int> DocumentManager::searchForWord(const string& word)
{
	//ar trebui sa mai iterez o data prin text?
	vector<pair<string, int>>words = tokenize();
	vector<int>indexes;
	for (int i = 0; i < words.size(); ++i)	{//aici merge pana la finalul capacitatii sau pana la ultimul element?
		if (words[i].first == word) {
			indexes.push_back(words[i].second);
		}
	}
	printf("word at indexes:\n");
	for(auto i : indexes) {
		printf("%d, ", i);
	}
	return indexes;
}

bool DocumentManager::copy(int start, int end)
{
	if (start < 0 || end >= lineBuffer[getLineCount()]) {
		printf("Error at copy: invalid line argument\n");
		return false;
	}
	if (!copyBuffer.empty()) {
		copyBuffer.clear();
	}
	copyBuffer = buffer.substr(start, end - start+1);
	printf("the copied substring: %s\n-----------------------------------------------\n", copyBuffer.c_str());
	return true;
}

bool DocumentManager::paste(int position)
{
	if (position < 0 || position > lineBuffer[getLineCount()]) {
		printf("Error at paste: invalid position argument");
		return false;
	}
	if (copyBuffer.empty()) {
		printf("Error at paste: copy buffer is empty");
		return false;
	}
	int line = 0;
	while (lineBuffer[line + 1] < position)line++;
	int col = position - lineBuffer[line];
	insertText(line, col, copyBuffer);
	printf("the program tried inserting the copied text at line %d and column %d \n-----------------------------------------------\n", line, col);
	printf("the string after pasting:\n%s\n-----------------------------------------------\n", buffer.c_str());
	copyBuffer.clear();
	return true;
}

string DocumentManager::getBuffer()
{
	return buffer;
}
vector<int> DocumentManager::getLineBuffer()
{
	return lineBuffer;
}
