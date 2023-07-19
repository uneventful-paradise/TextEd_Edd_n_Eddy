
#include "DocumentManager.h"
/*
TO-DO'S
pointer pentru ultima zona modificata pentru salvare
*/

out_of_bounds outOfBounds;

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
		printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
	}

	printf("linecount: %d \n", getLineCount());
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

/*  functia asta era duplicata.. verifica te rog care e cea scrisa de tine ultima data si sterge o pe cealalta
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
  */
bool DocumentManager::insertText(int line, int col, const string& inserted)//de implementat exceptii.
{
	printf("start of insertText\n-----------------------------------------------\n");
	try{
		if ((line > getLineCount() or line < 0) or (line == getLineCount() and col > 0) or (lineBuffer[line] + col > buffer.size())) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at insertText: " <<  e.what();
		return false;
	} 

	printf("the program tried inserting at line %d, col %d, position %d, text %s\n-----------------------------------------------\n", line, col, lineBuffer[line] + col, inserted.c_str());
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
	//for logging

	arguments.push({ line, col, inserted });
	printf("argumentul copiat: %s\n", inserted.c_str());
	
	Operation op;
    op.operation = std::bind(&DocumentManager::addText, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	op.oppositeOperation = std::bind(&DocumentManager::removeText, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    operationStack.push(op);

	return true;
}

bool DocumentManager::insertLine(int line, const string& sample)//????
{
	try{
		if (line < 0 or line > getLineCount()) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at insertLine: " <<  e.what();
		return false;
	} 
	string text = sample;
	int stringSize = text.size();
	if (text[stringSize-1] != '\n') {
		text += '\n';
		stringSize++;
	}
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
	printf("start of deleteText\n-----------------------------------------------\n");
	try{
		if (line < 0 or (lineBuffer[line] + col + size > lineBuffer[getLineCount()])) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at deleteText: " <<  e.what();
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
		lineBuffer.erase(lineBuffer.begin() + line +1, lineBuffer.begin() + line + endlCount+1);
	}

	printf("text from %d to %d was deleted\n", startpos, endpos);
	string deleted = buffer.substr(startpos, endpos - startpos);
	printf("text deleted: %s\n",deleted.c_str());
	buffer.erase(buffer.begin() + startpos, buffer.begin() + endpos);
	for (int level = line+1; level <= getLineCount(); ++level) {
		lineBuffer[level] -= size;
	}
	//int newLineCount = getLineCount() - endlCount;
	for (int i = 0; i < lineBuffer.size(); ++i) {
			printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
		}
	printf("the string after the removal operation:\n%s\n-----------------------------------------------\n", buffer.c_str());


	//for logging

	arguments.push({ line, col, deleted });

	Operation op1;
    op1.operation = std::bind(&DocumentManager::removeText, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	op1.oppositeOperation = std::bind(&DocumentManager::addText, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    operationStack.push(op1);
	return true;
}

bool DocumentManager::deleteLine(int line)
{
	try{
		if (line < 0 or line >= getLineCount()) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at deleteLine: " <<  e.what();
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
	try{
		if (line < 0 or line >= getLineCount()) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at getLine: " <<  e.what();
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

bool DocumentManager::swapLines(int line1, int line2, const string& s)//swaplines pt mai multe
{
	printf("start of swapLines\n-----------------------------------------------\n");
	int lineC = getLineCount();
	try{
		if ((line1 >= lineC or line1 < 0) or (line2 >= lineC or line2 < 0)) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at swapLines: " <<  e.what();
		return false;
	} 
	if (line1 == line2) {
		return true;
	}
	string substr1 = getLine(line1);
	string substr2 = getLine(line2);
	removeText(line1, 0, buffer.substr(lineBuffer[line1], lineBuffer[line1+1]-lineBuffer[line1]));
	addText(line1, 0,substr2);
	removeText(line2, 0, buffer.substr(lineBuffer[line2], lineBuffer[line2+1]-lineBuffer[line2]));
	addText(line2, 0, substr1);
	//deleteLine(line1);
	//insertLine(line1, substr2);
	//deleteLine(line2);
	//insertLine(line2, substr1);
	for (int i = 0; i < lineBuffer.size(); ++i) {
		printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
	}

	arguments.push({ line1, line2, ""});//dau push la argumente invers

	Operation op;
    op.operation = std::bind(&DocumentManager::atomicSwap, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	op.oppositeOperation = std::bind(&DocumentManager::atomicSwap, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    operationStack.push(op);
	
	return true;
}

bool DocumentManager::swapLineWithSelected(int startSelect, int endSelect, const string& finPos)//implementarea nonscuffed nu o vad cu aceeasi signatura
{
	printf("start of swapLineWithSelected\n-----------------------------------------------\n");
	int finalPos = stoi(finPos);
	int lineC = getLineCount();
	try{
		if ((startSelect >= lineC or startSelect < 0) or (endSelect >= lineC or endSelect < 0) or (finalPos + endSelect-startSelect >= lineC)) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at swapLineWithSelected: " <<  e.what();
		return false;
	} 
	if (startSelect == finalPos) {
		return true;
	}
	int distanceMoved = finalPos - startSelect;
	int noItt = endSelect - startSelect + 1;
	int i = 0;
	if (distanceMoved > 0) {
		int line = endSelect;
		while (noItt) {
			string text = buffer.substr(lineBuffer[line], lineBuffer[line + 1] - lineBuffer[line]);
			printf("textul %s de la linia %d va fi mutat la linia %d\n", text.c_str(), line, line+distanceMoved);
			removeText(line, 0, text);
			addText(line + distanceMoved , 0, text);
			noItt--;
			line--;
		}
	}
	else {
		int line = startSelect;
		int noItt = endSelect - startSelect + 1;
		while (noItt) {
			string text = buffer.substr(lineBuffer[line], lineBuffer[line + 1] - lineBuffer[line]);
			printf("textul %s de la linia %d va fi mutat la linia %d\n", text.c_str(), line, line+distanceMoved);
			removeText(line, 0, text);
			addText(line + distanceMoved , 0, text);
			noItt--;
			line++;
		}
	}
	
	string arg3 = to_string(startSelect);
	arguments.push({ finalPos, distanceMoved+endSelect, arg3});//dau push la argumente invers

	Operation op;
    op.operation = std::bind(&DocumentManager::atomicSwapLinesWithSelected, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	op.oppositeOperation = std::bind(&DocumentManager::atomicSwapLinesWithSelected, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    operationStack.push(op);

	return true;
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
	try{
		if (start < 0 or end > lineBuffer[getLineCount()]) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at copy: " <<  e.what();
		return false;
	}	
	if (!copyBuffer.empty()) {
		copyBuffer.clear();
	}
	copyBuffer = buffer.substr(start, end - start);
	printf("the copied substring: %s\n-----------------------------------------------\n", copyBuffer.c_str());
	return true;
}

bool DocumentManager::paste(int position)
{
	try{
		if (position < 0 or position > lineBuffer[getLineCount()]) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at paste: " <<  e.what();
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

	arguments.push({ line, col, copyBuffer});
	Operation op;
    op.operation = std::bind(&DocumentManager::addText, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	op.oppositeOperation = std::bind(&DocumentManager::removeText, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    operationStack.push(op);

	copyBuffer.clear();
	return true;
}

bool DocumentManager::cut(int start, int end, const string& s) //ar trebui curatat si bufferul la undo
{
	printf("start of cut\n-----------------------------------------------\n");
	copy(start, end);
	if (start == end) {
		printf("cut:start == end\n");
		return true;
	}
	int startLine = 0;
	while (lineBuffer[startLine + 1] < start)startLine++;
	int startCol = start - startLine;
	deleteText(startLine, startCol, end - start)+1;
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
vector<int> DocumentManager::badCharacterHeuristic(const string& pattern)
{
	vector<int>BC(NO_CHARS, -1);
	printf("the BC vector after declaration:\n-----------------------------------------------\n");
	for (auto elem : BC) {
		printf("%d, ", elem);
	}
	printf("\n-----------------------------------------------\n");
	//fill(BC.begin(), BC.end(), -1);
	int m = pattern.size();
	for (int i = 0; i < m; ++i) {
		BC[(int)pattern[i]] = i;
	}
	printf("the BC vector after preprocessing:\n-----------------------------------------------\n");
	for (auto elem : BC) {
		printf("%d, ", elem);
	}
	printf("\n-----------------------------------------------\n");
	return BC;
}

vector<int> DocumentManager::failureF(const string& pattern)
{
	int m = pattern.size();
	vector<int>failure(m+1);
	failure[0] = -1;
	for (int i = 1; i <= m; ++i) {
		int k = failure[i - 1];
		while (k >= 0 and pattern[k] != pattern[i - 1]) {
			k = failure[k];
		}
		if (k == -1) {
			failure[i] = 0;
		}
		else {
			failure[i] = k+1;
		}
	}
	printf("output of failure function:\n");
	for (auto ch : pattern) {
		printf(" %c", ch);
	}
	printf("\n");
	for (auto elem : failure) {
		printf("%d ", elem);
	}
	printf("\n");
	for (int i = 0; i <= m; ++i) {
		printf(" %d", i);
	}
	printf("\n-----------------------------------------------\n");
	return failure;
}

vector<int> DocumentManager::goodSuffixHeuristic(const string& pattern)
{
	int m = pattern.size();
	vector<int>failure(m + 1);
	string inverse_string = pattern;
	failure = failureF(pattern);
	vector<int>GS(m+1);//m+1?
	for (int i = 0; i < m; ++i) {
		GS[i] = failure[m] - (m - i);
	}
	printf("Content of GS after the first stage of peprocessing:\n-----------------------------------------------\n");
	for (auto elem : GS) {
		printf("%d ", elem);
	}
	printf("\n");
	reverse(inverse_string.begin(), inverse_string.end());
	failure = failureF(inverse_string);
	printf("failure function for inverted text:\n");
	for (auto elem : failure) {
		printf("%d ", elem);
	}
	printf("\n");
	reverse(failure.begin(), failure.end());
	printf("inverted failure function for inverted text:\n");
	for (auto elem : failure) {
		printf("%d ", elem);
	}
	printf("\n-----------------------------------------------\n");
	printf("Content of GS after the second stage of peprocessing:\n-----------------------------------------------\n");
	for (int i = 0; i < m; ++i) {
		int len = failure[i];
		printf("%d ", len);
		GS[m - len] = i;
	}
	printf("\n");
	for (auto elem : GS) {
		printf("%d ", elem);
	}
	printf("\n");
	return GS;
}

bool DocumentManager::findUsingBM(const string& text, const string& pattern, int& i, int n, int m)
{
	int k = 0;
	vector<int>BC = badCharacterHeuristic(pattern);
	vector<int>GS = goodSuffixHeuristic(pattern);
	while (k < m and i <= n - m) {
		if (text[i + m - 1 - k] == pattern[m - 1 - k]) {
			k++;
		}
		else {
			int shiftbc = m - k - 1 - BC[(int)text[i + m - 1 - k]];
			int shiftgs = m - k - GS[m - k];
			i += max(shiftbc, shiftgs);
			k = 0;
		}
	}
	if (k == m) {
		printf("an occurence of pattern in text was found at index: %d\n", i);
		return true;
	}
	printf("no occurences of pattern in text\n");
	return false;
}

vector<int> DocumentManager::find(const string& text, const string& pattern)
{
	vector<int>found;
	int startpos = 0;
	int m = pattern.size();
	int n = text.size();
	while (startpos <= n - m) {
		if (findUsingBM(text, pattern, startpos, n, m)) {
			found.push_back(startpos);
			startpos++;
		}
	}
	if (found.empty()) {
		printf("no occurences of pattern\n");
		return { -1 };
	}
	else {
		printf("pattern found at positions:\n-----------------------------------------------\n");
		for (auto pos : found) {
			printf("%d ", pos);
		}
		return found;
	}
}

bool DocumentManager::addText(int line, int col, const string& text)
{
	printf("object tried inserting text at line %d, col %d, the text inserted was %s\n", line, col, text.c_str());
	printf("start of addText\n-----------------------------------------------\n");
	try{
		if ((line > getLineCount() or line < 0) or (line == getLineCount() and col > 0) or (lineBuffer[line] + col > buffer.size())) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at insertText: " <<  e.what();
		return false;
	} 

	printf("the program tried inserting at line %d, col %d, position %d, text %s\n-----------------------------------------------\n", line, col, lineBuffer[line] + col, text.c_str());
	buffer.insert(lineBuffer[line] + col, text);
	int insSize = text.size();
	bool newLine = false;
	int lineC = getLineCount();
	for (int level = line + 1; level <= lineC; ++level) {
		lineBuffer[level] += insSize;
	}
	for (int i = 0; i < insSize; ++i) {
		if (text[i] == '\n' or text[i] == 10) {
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
	return true;
}

bool DocumentManager::removeText(int line, int col, const string& text)
{
	int size = text.size();
	printf("object tried deleting text at line %d, col %d, the text deleted has size %d\n", line, col, size);
	printf("start of removeText\n-----------------------------------------------\n");
	try{
		if (line < 0 or (lineBuffer[line] + col + size > lineBuffer[getLineCount()])) {
			printf("position: %d %d %d %d\n", lineBuffer[line] + col + size, lineBuffer[line], col, size);
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at deleteText: " <<  e.what();
		return false;
	} 
	int startpos = lineBuffer[line] + col;
	int endpos = startpos + size;
	int endlCount = 0;
	for (int i = startpos; i < endpos; ++i) {
		if (buffer[i] == '\n' or buffer[i] == 10) {
			endlCount++;
		}
	}
	printf("the string before the removal operation:\n%s\n-----------------------------------------------\n", buffer.c_str());
	printf("found %d newlines\n-----------------------------------------------\n", endlCount);
	
	if (endlCount) {
		//lineBuffer[line + endlCount] = lineBuffer[line];
		lineBuffer.erase(lineBuffer.begin() + line +1, lineBuffer.begin() + line + endlCount+1);
	}

	printf("text from %d to %d was deleted\n", startpos, endpos);
	string deleted = buffer.substr(startpos, endpos - startpos);
	printf("text deleted: %s\n",deleted.c_str());
	buffer.erase(buffer.begin() + startpos, buffer.begin() + endpos);
	for (int level = line+1; level <= getLineCount(); ++level) {
		lineBuffer[level] -= size;
	}
	for (int i = 0; i < lineBuffer.size(); ++i) {
		printf("line %d starts at position %d in the given string\n", i, lineBuffer[i]);
		}
	printf("the string after the removal operation:\n%s\n-----------------------------------------------\n", buffer.c_str());

	return true;
}

bool DocumentManager::atomicSwap(int line1, int line2, const string& text)
{
	printf("start of atomicSwap\n-----------------------------------------------\n");
	int lineC = getLineCount();
	try{
		if ((line1 >= lineC or line1 < 0) or (line2 >= lineC or line2 < 0)) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at swapLines: " <<  e.what();
		return false;
	} 
	if (line1 == line2) {
		return true;
	}
	string substr1 = getLine(line1);
	string substr2 = getLine(line2);
	removeText(line1, 0, buffer.substr(lineBuffer[line1], lineBuffer[line1+1]-lineBuffer[line1]));
	addText(line1, 0,substr2);
	removeText(line2, 0, buffer.substr(lineBuffer[line2], lineBuffer[line2+1]-lineBuffer[line2]));
	addText(line2, 0, substr1);
	return true;
}

bool DocumentManager::atomicSwapLinesWithSelected(int startSelect, int endSelect, const string& finPos)
{
	printf("start of atomicSwapLineWithSelected\n-----------------------------------------------\n");
	int finalPos = stoi(finPos);
	int lineC = getLineCount();
	try{
		if ((startSelect >= lineC or startSelect < 0) or (endSelect >= lineC or endSelect < 0) or (finalPos + endSelect-startSelect >= lineC)) {
			throw outOfBounds;
		}
	}
	catch (exception& e) {
		cerr << "Error at swapLineWithSelected: " <<  e.what();
		return false;
	} 
	if (startSelect == finalPos) {
		return true;
	}
	int distanceMoved = finalPos - startSelect;
	int noItt = endSelect - startSelect + 1;
	int i = 0;
	if (distanceMoved > 0) {
		int line = endSelect;
		while (noItt) {
			string text = buffer.substr(lineBuffer[line], lineBuffer[line + 1] - lineBuffer[line]);
			printf("textul %s de la linia %d va fi mutat la linia %d\n", text.c_str(), line, line+distanceMoved);
			removeText(line, 0, text);
			addText(line + distanceMoved , 0, text);
			noItt--;
			line--;
		}
	}
	else {
		int line = startSelect;
		int noItt = endSelect - startSelect + 1;
		while (noItt) {
			string text = buffer.substr(lineBuffer[line], lineBuffer[line + 1] - lineBuffer[line]);
			printf("textul %s de la linia %d va fi mutat la linia %d\n", text.c_str(), line, line+distanceMoved);
			removeText(line, 0, text);
			addText(line + distanceMoved , 0, text);
			noItt--;
			line++;
		}
	}
	return true;
}


void DocumentManager::logs()
{
	int stackSize = operationStack.size();
	while (stackSize) {
		Operation currentOperation = operationStack.top();
		auto argument = arguments.top();
		operationStack.pop();
		arguments.pop();
		stackSize--;
		printf("argument %d %d %s\n", get<0>(argument), get<1>(argument), get<2>(argument).c_str());
		//currentOperation.operation(get<0>(argument), get<1>(argument), get<2>(argument));

		// Call the opposite operation
		currentOperation.oppositeOperation(get<0>(argument), get<1>(argument), get<2>(argument));

	}
}
