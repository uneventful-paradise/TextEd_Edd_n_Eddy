#include "ContentEditor.h"

ContentEditor::ContentEditor(DocumentManager* document)
{
	this->document = document;
	this->cursor = new Cursor(document);
	this->selector = new Select(cursor, document);
}



pair<int, int> ContentEditor::getCursorPosition()
{
	int cursorLine = cursor->getLine();
	int cursorCol = cursor->getCol();
	return { cursorLine, cursorCol };//e ok asa?
}

void ContentEditor::newSelection(int startLine, int startCol)
{
	selector->newSelection(startLine, startCol);
}

void ContentEditor::newSelectionFromCursor()
{
	selector->newSelectionOnCursor();
}

void ContentEditor::updateSelection()
{
	selector->updateSelectionOnCursor();
}

void ContentEditor::deleteSelection()
{
	selector->resetSelection();
}

void ContentEditor::moveCursorUp(int distance, bool updateSelection)
{
	cursor->moveV(distance);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}
}

void ContentEditor::moveCursorDown(int distance, bool updateSelection)
{
	cursor->moveV(distance);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}	
}

void ContentEditor::moveCursorLeft(int distance, bool updateSelection)
{
	cursor->moveH(distance);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}		
}

void ContentEditor::moveCursorRight(int distance, bool updateSelection)
{
	cursor->moveH(distance);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}		
}

void ContentEditor::moveCursorToNextWord(int direction, bool updateSelection)
{
	cursor->moveToWord(direction);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}
}

void ContentEditor::moveCursorToBeginningOfDocument(bool updateSelection)
{
	cursor->jumpTo(0);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}
}

void ContentEditor::moveCursorToEndOfDocument(bool updateSelection)
{
	cursor->jumpTo(1);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}
}

void ContentEditor::moveCursorToBeginningOfLine(bool updateSelection) 
{
	cursor->jumpTo(2);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}
}

void ContentEditor::moveCursorToEndOfLine(bool updateSelection)
{
	cursor->jumpTo(3);
	if (updateSelection) {
		selector->updateSelectionOnCursor();
	}
}

void ContentEditor::deleteSelectedText()
{
	coordinates*sel = selector->getSelectionCoordinates();
	
	int line = sel->lineStart;
	int col = sel->colStart;
	int start = document->getLineBuffer()[line] + col;
	int end = document->getLineBuffer()[sel->lineEnd] + sel->colEnd;
	int size = end - start+1;
	string deleted = document->getBuffer().substr(start, size);
	document->deleteText(line, col, size);
	cursor->updateCoordinates(line, col);

	arguments.push({ line, col, deleted });
	
	Operation op1;
    op1.operation = bind(&DocumentManager::removeText, document, placeholders::_1, placeholders::_2, placeholders::_3);
	op1.oppositeOperation = bind(&DocumentManager::addText, document, placeholders::_1, placeholders::_2, placeholders::_3);
    operationStack.push(op1);
}

void ContentEditor::deleteOnCursor(int size)
{
	int sizeDeleted = size;
	int line = cursor->getLine();
	int col = cursor->getCol();
	if (line + col - sizeDeleted < 0) {
		sizeDeleted = line + col;
	}
	/*cursor->moveH((-1)*sizeDeleted);
	line = cursor->getLine();
	col = cursor->getCol();*/
	string deleted = document->getBuffer().substr(document->getLineBuffer()[line]+col, sizeDeleted);
	document->deleteText(line, col, sizeDeleted);

	arguments.push({ line, col, deleted });
	
	Operation op;
    op.operation = std::bind(&DocumentManager::removeText, document, placeholders::_1, placeholders::_2, placeholders::_3);
	op.oppositeOperation = std::bind(&DocumentManager::addText, document, placeholders::_1, placeholders::_2, placeholders::_3);
    operationStack.push(op);
}

void ContentEditor::addTextOnCursor(const string& text)
{
	int sizeInserted = text.size();
	int line = cursor->getLine();
	int col = cursor->getCol();

	document->insertText(line, col, text);
	cursor->moveMultipleH(sizeInserted);

	arguments.push({ line, col, text });
	printf("argumentul copiat: %s\n", text.c_str());
	
	Operation op;
    op.operation = bind(&DocumentManager::addText, document, placeholders::_1, placeholders::_2, placeholders::_3);
	op.oppositeOperation = bind(&DocumentManager::removeText, document, placeholders::_1, placeholders::_2, placeholders::_3);
    operationStack.push(op);
}

void ContentEditor::deleteCursorLine()
{
	int line = cursor->getLine();
	int sizeDeleted = document->getLineBuffer()[line + 1] - document->getLineBuffer()[line];
	string deleted = document->getBuffer().substr(document->getLineBuffer()[line], sizeDeleted);
	document->deleteText(line, 0, sizeDeleted);
	
	cursor->moveMultipleH((-1) * sizeDeleted);

	arguments.push({ line, 0, deleted });
	
	Operation op;
    op.operation = std::bind(&DocumentManager::removeText, document, placeholders::_1, placeholders::_2, placeholders::_3);
	op.oppositeOperation = std::bind(&DocumentManager::addText, document, placeholders::_1, placeholders::_2, placeholders::_3);
    operationStack.push(op);
}

void ContentEditor::pasteOnCursor()
{
	int line = cursor->getLine();
	int col = cursor->getCol();
	string copyB = document->getCopyBuffer();
	int position = document->getLineBuffer()[line] + col;
	document->paste(position);

	arguments.push({ line, col, copyB});
	Operation op;
    op.operation = bind(&DocumentManager::addText, document, placeholders::_1, placeholders::_2, placeholders::_3);
	op.oppositeOperation = bind(&DocumentManager::removeText, document, placeholders::_1, placeholders::_2, placeholders::_3);
    operationStack.push(op);
}

void ContentEditor::moveSelectedLines(int direction)
{
	coordinates*sel = selector->getSelectionCoordinates();
	
	int line1 = sel->lineStart;
	int lineN = sel->lineEnd;
	string finishLine = to_string(line1 + direction);
	document->swapLineWithSelected(line1, lineN, finishLine);
	cursor->moveV(direction);
	
	string arg3 = to_string(line1);
	arguments.push({ line1+direction, lineN+direction, arg3});
	Operation op;
    op.operation = bind(&DocumentManager::atomicSwapLinesWithSelected, document, placeholders::_1, placeholders::_2, placeholders::_3);
	op.oppositeOperation = bind(&DocumentManager::atomicSwapLinesWithSelected, document, placeholders::_1, placeholders::_2, placeholders::_3);
    operationStack.push(op);
}

void ContentEditor::copySelected()
{
	coordinates*sel = selector->getSelectionCoordinates();
	
	int line = sel->lineStart;
	int col = sel->colStart;
	int start = document->getLineBuffer()[line] + col;
	int end = document->getLineBuffer()[sel->lineEnd] + sel->colEnd;

	document->copy(start, end);
}

void ContentEditor::cutSelected() //vezi delete, e endpos-startpos+1???sau doar endpos-startpos
{
	coordinates*sel = selector->getSelectionCoordinates();
	
	int line = sel->lineStart;
	int col = sel->colStart;
	int start = document->getLineBuffer()[line] + col;
	int end = document->getLineBuffer()[sel->lineEnd] + sel->colEnd;
	int sizeDeleted = end - start + 1;
	copySelected();
	deleteSelectedText();
}

void ContentEditor::undo()
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

void ContentEditor::redo()
{
}


//de adaugat updateSelection
