#pragma once
#include "Cursor.h"
#include"DocumentManager.h"
#include"Select.h"
#include "Exceptions.h"

struct Operation {
    function<bool(int, int, const string&)> operation;
    function<bool(int, int, const string&)> oppositeOperation;
};

class ContentEditor
{
public:
	DocumentManager* document;
	Cursor* cursor;
	Select* selector;
	ContentEditor(DocumentManager* document);

	stack<Operation> operationStack;
	stack<tuple<int, int, const string >>arguments;		
	
	stack<Operation> redoOperationStack;
	stack<tuple<int, int, const string >>redoArguments;
	//selector-reset, getInterval
	void newSelection(int startLine, int startCol);
	void newSelectionFromCursor();
	void updateSelection();
	void deleteSelection();
	//cursor - buna ziua
	//cursor movement
	pair<int, int>getCursorPosition();
	void moveCursorUp(int distance, bool updateSelection = false);
	void moveCursorDown(int distance, bool updateSelection = false);
	void moveCursorLeft(int distance, bool updateSelection = false);
	void moveCursorRight(int distance, bool updateSelection = false);
	void moveCursorToNextWord(int direction,bool updateSelection = false);
	void moveCursorToBeginningOfDocument(bool updateSelection = false);
	void moveCursorToEndOfDocument(bool updateSelection = false);
	void moveCursorToBeginningOfLine(bool updateSelection = false);
	void moveCursorToEndOfLine(bool updateSelection = false);
	//document - sa se mute si cursorul
	void addTextOnCursor(const string& text);
	void pasteOnCursor();

	void moveSelectedLines(int direction);
	void copySelected();
	void cutSelected();

	void deleteOnCursor(int size);
	void deleteCursorLine();
	void deleteSelectedText();

	void undo();
	void redo();
};

