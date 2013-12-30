#include "EditableText.h"
#include "Font.h"
#include "util.h"

using namespace begui;

EditableText::EditableText() : m_bMultiLine(true), m_cursorX(0), m_cursorY(0),
	m_cursorPos(0), m_selectStart(0), m_selectEnd(0), m_cursorH(0),
	m_bEditable(true), m_x(0), m_y(0), m_lineWidth(0),
	m_bRenderCursor(true),
	m_bTextSelectable(true),
	m_textColor(0,0,0), m_textAlpha(0.7f),
	m_selectionColor(1.0f, 0.7f, 0.2f), m_selectionAlpha(0.7f),
	m_cursorColor(0,0,0),
	m_bTextHidden(false)
{
	m_text = "";

	m_cursorAlpha.push_back(0, 0);
	m_cursorAlpha.push_back(0, 0.2);
	m_cursorAlpha.push_back(0.8f, 0.4);
	m_cursorAlpha.push_back(0.8f, 0.8);
	m_cursorAlpha.push_back(0, 1);
	m_cursorAlpha.set_loop(true);
	m_cursorAlpha.set_interpolation(TimeSeries<float>::Interpolation::LINEAR);
	m_cursorAlpha.start();
}

void EditableText::create(int x, int y, int lineWidth, bool bMultiLine, bool bEditable, bool bTextSelectable)
{
	m_x = x;
	m_y = y;
	m_lineWidth = lineWidth;
	m_bMultiLine = bMultiLine;
	m_bEditable = bEditable;
	m_bTextSelectable = bTextSelectable;
	((std::string)m_text).clear();
	m_charPos.clear();
	m_selectStart = m_selectEnd = 0;
	m_cursorPos = 0;
	m_cursorX = m_cursorY = m_cursorH = 0;
	m_text = "";
}

void EditableText::update()
{
	if (m_text.livevar_is_dirty()) {
		setText(m_text);
		m_text.livevar_set_dirty(false);
	}
}

void EditableText::setText(const std::string &text)
{
	m_text = text;
	int lineHeight = FontManager::getCurFont()->getLineHeight();

	// call Font::renderString once WITHOUT rendering, to determine the
	// positions of all characters
	m_charPos.clear();
	if (m_bMultiLine)
		FontManager::getCurFont()->renderStringMultiline(m_x, m_y+lineHeight, m_lineWidth, m_text, &m_charPos, false);
	else
		FontManager::getCurFont()->renderString(m_x, m_y+lineHeight, m_text, &m_charPos, false);
}

void EditableText::renderString()
{
	int lineHeight = FontManager::getCurFont()->getLineHeight();

	// render the background for each selected character
	if (m_bTextSelectable)
	{
		glColor4f(m_selectionColor.r, m_selectionColor.g, m_selectionColor.b, m_selectionAlpha);
		glBegin(GL_QUADS);
		int selStart = m_selectStart, selEnd = m_selectEnd;
		if (selStart > selEnd) {
			selStart = m_selectEnd;
			selEnd = m_selectStart;
		}
		for (int i = selStart; i<selEnd; ++i) {
			// highlight this character
			Rect<int> &pos = m_charPos[i];
			glVertex2f((float)pos.left-1, (float)pos.top-1);
			glVertex2f((float)pos.right, (float)pos.top-1);
			glVertex2f((float)pos.right, (float)pos.bottom);
			glVertex2f((float)pos.left-1, (float)pos.bottom);
		}
		glEnd();
	}

	// if text should be hidden (password field) replace text with *
	std::string text = m_text;
	if (m_bTextHidden) {
		text = std::string(text.length(), '*');	// if multiline is enabled, strange wrapping might occur - not expected usage scenario though
	}

	// now render the string
	// TODO: render each character separately if text has been changed, otherwise
	// use a display list to speed things up
	m_charPos.clear();
	glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, m_textAlpha);
	if (m_bMultiLine)
		FontManager::getCurFont()->renderStringMultiline(m_x, m_y+lineHeight, m_lineWidth, text, &m_charPos, true);
	else
		FontManager::getCurFont()->renderString(m_x, m_y+lineHeight, text, &m_charPos, true);

	// render the cursor
	if (m_bEditable && m_bRenderCursor)
	{
		glColor4f(m_cursorColor.r, m_cursorColor.g, m_cursorColor.b, m_cursorAlpha);
		glBegin(GL_LINES);
			glVertex2f((float)m_cursorX, (float)m_cursorY);
			glVertex2f((float)m_cursorX, (float)m_cursorY-10);
		glEnd();
	}
}

bool EditableText::onMouseDown(int x, int y, int button)
{
	x-=m_x;
	y-=m_y;

	if (button == MOUSE_BUTTON_LEFT) {
		if (input::isDoubleClick(MOUSE_BUTTON_LEFT)) {
		}
		else {
			getStringPos(x,y, &m_cursorX, &m_cursorY, &m_cursorH, &m_cursorPos);
			m_selectStart = m_selectEnd = m_cursorPos;
		}
	}
	return true;
}

bool EditableText::onMouseMove(int x, int y, int prevx, int prevy)
{
	x-=m_x;
	y-=m_y;
	prevx-=m_x;
	prevx-=m_y;

	if (input::isMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		getStringPos(x,y, &m_cursorX, &m_cursorY, &m_cursorH, &m_cursorPos);
		m_selectEnd = m_cursorPos;
	}
	return true;
}

bool EditableText::onMouseUp(int x, int y, int button)
{
	x-=m_x;
	y-=m_y;

	if (button == MOUSE_BUTTON_LEFT) {
		getStringPos(x,y, &m_cursorX, &m_cursorY, &m_cursorH, &m_cursorPos);
		m_selectEnd = m_cursorPos;
	}
	return true;
}

void EditableText::onKeyDown(int key)
{
	//TODO: done to use ascii>127. find a better fix for that...
	if (key < 0) key = 256+key;

	switch (key)
	{
	// take care of backspace and navigation keys
	case KEY_LEFT:
		if (m_cursorPos > 0) {
			//TODO: handle CTRL+arrow
			setCursorPos(m_cursorPos-1);
		}
		m_selectEnd = m_cursorPos;
		if (!(input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)))
			m_selectStart = m_cursorPos;
		break;
	case KEY_RIGHT:
		if (m_cursorPos < (int)m_charPos.size()) {
			//TODO: handle CTRL+arrow
			setCursorPos(m_cursorPos+1);
		}
		m_selectEnd = m_cursorPos;
		if (!(input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)))
			m_selectStart = m_cursorPos;
		break;
	case KEY_UP:
		if (m_bMultiLine)
		{
			cursorUp(&m_cursorX, &m_cursorY, &m_cursorH, &m_cursorPos);	
			m_selectEnd = m_cursorPos;
			if (!(input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)))
				m_selectStart = m_cursorPos;
		}
		break;
	case KEY_DOWN:
		if (m_bMultiLine)
		{
			cursorDown(&m_cursorX, &m_cursorY, &m_cursorH, &m_cursorPos);	
			m_selectEnd = m_cursorPos;
			if (!(input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)))
				m_selectStart = m_cursorPos;
		}
		break;
	case KEY_DELETE:
		if (m_bEditable) {
			if (m_selectStart != m_selectEnd && m_bTextSelectable) {
				std::string text = m_text;
				if (m_selectStart < m_selectEnd)
					text.erase(m_selectStart, m_selectEnd-m_selectStart);
				else
					text.erase(m_selectEnd, m_selectStart-m_selectEnd);
				setText(text);
				setCursorPos( (m_selectStart < m_selectEnd) ? m_selectStart : m_selectEnd );
				m_selectEnd = m_selectStart = m_cursorPos;
			}
			else if (m_cursorPos < (int)((std::string)m_text).size()) {
				std::string text = m_text;
				text.erase(m_cursorPos, 1);
				setText(text);
			}
			m_selectEnd = m_selectStart = m_cursorPos;
		}
		break;
	case '\b':
		if (m_bEditable) {
			if (m_selectStart != m_selectEnd && m_bTextSelectable) {
				std::string text = m_text;
				if (m_selectStart < m_selectEnd)
					text.erase(m_selectStart, m_selectEnd-m_selectStart);
				else
					text.erase(m_selectEnd, m_selectStart-m_selectEnd);
				setText(text);
				setCursorPos( (m_selectStart < m_selectEnd) ? m_selectStart : m_selectEnd );
			}
			else if (m_cursorPos > 0) {
				std::string text = m_text;
				text.erase(m_cursorPos-1, 1);
				setText(text);
				setCursorPos(m_cursorPos-1);
			}
			m_selectEnd = m_selectStart = m_cursorPos;
		}
		break;
	case KEY_HOME:
		if (input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL))
			setCursorPos(0);
		else
			setCursorPos(getLineStart(m_cursorPos));
		m_selectEnd = m_cursorPos;
		if (!(input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)))
			m_selectStart = m_cursorPos;
		break;
	case KEY_END:
		if (input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL))
			setCursorPos((int)((std::string)m_text).length());
		else
			setCursorPos(getLineEnd(m_cursorPos));
		m_selectEnd = m_cursorPos;
		if (!(input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)))
			m_selectStart = m_cursorPos;
		break;
	default:
		if (m_bEditable)
		{
			// if a printable character, add it to the cursor position
			if (key < 256 && ( /*isprint(key)*/key>' ' || key==' ' || key == '\t' || (m_bMultiLine && key==KEY_ENTER) ))
			{
				// if there is selected text, remove it (to be replaced by the new character)
				if (m_selectStart != m_selectEnd && m_bTextSelectable) {
					std::string text = m_text;
					if (m_selectStart < m_selectEnd)
						text.erase(m_selectStart, m_selectEnd-m_selectStart);
					else
						text.erase(m_selectEnd, m_selectStart-m_selectEnd);
					setText(text);

					// return the cursor to the beginning of the selection
					setCursorPos( (m_selectStart < m_selectEnd) ? m_selectStart : m_selectEnd );

					// nothing selected now:
					m_selectStart = m_selectEnd = m_cursorPos;
				}

				if (key == 13)
					key = '\n';
				char str[2] = {key, 0};
				std::string text = m_text;
				size_t blah = text.length();
				text.insert(m_cursorPos, str);
				setText(text);
				setCursorPos(m_cursorPos+1);
				m_selectStart = m_selectEnd = m_cursorPos;
			}
		}
	}
}

void EditableText::onKeyUp(int key)
{
}

void EditableText::setCursorPos(int pos)
{
	int lineHeight = FontManager::getCurFont()->getLineHeight();//TEMP

	if (pos < 0) pos = 0;
	if (pos > (int)m_charPos.size()) pos = (int)m_charPos.size();
	m_cursorPos = pos;
	if (pos < (int)m_charPos.size()) {
		m_cursorX = m_charPos[pos].left;
		m_cursorY = m_charPos[pos].bottom;
		m_cursorH = m_charPos[pos].getHeight();
	}
	else if (pos > 0) {	// cursor at the end of the text, text non-empty
		m_cursorX = m_charPos[pos-1].right;
		m_cursorY = m_charPos[pos-1].bottom;
		m_cursorH = m_charPos[pos-1].getHeight();
	}
	else {	// cursor at the end of the text, text empty
		m_cursorH = lineHeight;
		m_cursorX = m_x;
		m_cursorY = m_y+lineHeight;
	}
}

void EditableText::getStringPos(int x, int y, int *cursorX, int *cursorY, int *cursorH, int* char_pos) const
{
	ASSERT(cursorX);
	ASSERT(cursorY);
	ASSERT(cursorH);
	ASSERT(char_pos);
	*cursorX = *cursorY = *cursorH = *char_pos = 0;

	for (size_t i=0; i<m_charPos.size(); ++i)
	{
		int cw = m_charPos[i].getWidth();

		int dx = m_charPos[i].left;
		int dy = m_charPos[i].bottom;
		int dy2 = m_charPos[i].top;

		// check if this character contains the given pt
		//if (m_charPos[i].contains(x,y)) {
		if (m_charPos[i].bottom >= y && m_charPos[i].left <= x && m_charPos[i].right >= x) {
			if (x < m_charPos[i].left + cw/2) {
				*cursorX = m_charPos[i].left;
				*cursorY = m_charPos[i].bottom;
				*cursorH = m_charPos[i].getHeight();
				*char_pos = (int)i;
			}
			else {
				if (i < m_charPos.size()-1) {
					*cursorX = m_charPos[i+1].left;
					*cursorY = m_charPos[i+1].bottom;
					*cursorH = m_charPos[i+1].getHeight();
					*char_pos = (int)(i+1);
				}
				else
				{
					*cursorX = m_charPos[i].right;
					*cursorY = m_charPos[i].bottom;
					*cursorH = m_charPos[i].getHeight();
					*char_pos = (int)(i+1);	// beyond the last character
				}
			}
			return;
		}

		// else, if we are on the correct line
		if (/*y >= m_charPos[i].top &&*/ y <= m_charPos[i].bottom)
		{
			if (x < m_charPos[i].left) {
				// check if this character is the first in this line
				if (i == 0 || m_charPos[i-1].left > m_charPos[i].left) {
					// then put the cursor at the beginning of this line
					*cursorX = m_charPos[i].left;
					*cursorY = m_charPos[i].bottom;
					*cursorH = m_charPos[i].getHeight();
					*char_pos = (int)i;
					return;
				}
			}
			else if (x > m_charPos[i].right)
			{
				// check if this character is the last in this line
				if (i==m_charPos.size()-1 || m_charPos[i+1].left < m_charPos[i].left) {
					// put the character beyond the end of the line
					*cursorX = m_charPos[i].right;
					*cursorY = m_charPos[i].bottom;
					*cursorH = m_charPos[i].getHeight();
					if (i == m_charPos.size()-1)
						*char_pos = (int)(i+1);
					else
						*char_pos = (int)i;
					return;
				}
			}
		}
		else if (y < m_charPos[i].top && i==0) {
			// put the cursor to the beginning of the first line
			*cursorX = m_charPos[0].left;
			*cursorY = m_charPos[0].bottom;
			*cursorH = m_charPos[0].getHeight();
			*char_pos = 0;
			//return;
		}
		else if (y > m_charPos[i].bottom && i==m_charPos.size()-1) {
			// put the cursor beyond the end of the last line
			*cursorX = m_charPos[i].right;
			*cursorY = m_charPos[i].bottom;
			*cursorH = m_charPos[i].getHeight();
			*char_pos = (int)(i+1);
			return;
		}
	}
}

void EditableText::cursorUp(int *cursorX, int *cursorY, int *cursorH, int* char_pos) const
{
	ASSERT(cursorX);
	ASSERT(cursorY);
	ASSERT(cursorH);
	ASSERT(char_pos);

	// start searching for the letter that is over the middle
	// of the current one
	int x = m_cursorX;
	int y = m_cursorY;
	int curPos = m_cursorPos;

	if (curPos == 0)
		return;

	*cursorY = *cursorX = 0;
	for (int i=curPos-1; i>=0; --i)
	{
		if (i >= (int)m_charPos.size()-1)
			continue;

		int cw = m_charPos[i].getWidth();

		// if this is a line break
		// check if the cursor's x is beyond the end of this line
		if (m_charPos[i].right <= x && m_charPos[i+1].left <= m_charPos[i].left)
		{
			*cursorX = m_charPos[i].right;
			*cursorY = m_charPos[i].bottom;
			*cursorH = m_charPos[i].getHeight();
			*char_pos = (int)(i+1);
			break;
		}

		// check if this character contains the given pt
		if (m_charPos[i].left <= x && m_charPos[i+1].left > x)
		{
			if (x < m_charPos[i].left + cw/2) {
				*cursorX = m_charPos[i].left;
				*cursorY = m_charPos[i].bottom;
				*cursorH = m_charPos[i].getHeight();
				*char_pos = (int)i;
			}
			else {
				*cursorX = m_charPos[i+1].left;
				*cursorY = m_charPos[i+1].bottom;
				*cursorH = m_charPos[i+1].getHeight();
				*char_pos = (int)(i+1);
			}
			break;
		}
	}
}

void EditableText::cursorDown(int *cursorX, int *cursorY, int *cursorH, int* char_pos) const
{
	ASSERT(cursorX);
	ASSERT(cursorY);
	ASSERT(cursorH);
	ASSERT(char_pos);

	// start searching for the letter that is over the middle
	// of the current one
	int x = m_cursorX;
	int y = m_cursorY;
	int curPos = m_cursorPos;

	if (curPos >= (int)m_charPos.size()-1)
		return;

	*cursorY = *cursorX = 0;
	for (size_t i=curPos+1; i<m_charPos.size(); ++i)
	{
		int cw = m_charPos[i].getWidth();
		
		if (i == m_charPos.size()-1) {
			//we reached the end of the text
			*cursorX = m_charPos[i].right;
			*cursorY = m_charPos[i].bottom;
			*cursorH = m_charPos[i].getHeight();
			*char_pos = (int)(i+1);
			break;
		}

		// if this is a line break
		// check if the cursor's x is beyond the end of this line
		if (m_charPos[i].right <= x && m_charPos[i+1].right <= m_charPos[i].left) {
			*cursorX = m_charPos[i].right;
			*cursorY = m_charPos[i].bottom;
			*cursorH = m_charPos[i].getHeight();
			*char_pos = (int)(i+1);
			break;
		}

		// check if this character contains the given pt
		if (m_charPos[i].left <= x && m_charPos[i+1].left > x)
		{
			if (x < m_charPos[i].left + cw/2) {
				*cursorX = m_charPos[i].left;
				*cursorY = m_charPos[i].bottom;
				*cursorH = m_charPos[i].getHeight();
				*char_pos = (int)i;
			}
			else {
				*cursorX = m_charPos[i+1].left;
				*cursorY = m_charPos[i+1].bottom;
				*cursorH = m_charPos[i+1].getHeight();
				*char_pos = (int)(i+1);
			}
			break;
		}
	}
}

int EditableText::getLineStart(int cursorPos) const
{
	if (cursorPos == 0)
		return 0;
	if (cursorPos >= (int)m_charPos.size())
		cursorPos = (int)m_charPos.size()-1;
	for (int i=cursorPos; i>0; --i)
	{
		if (m_charPos[i].left <= m_charPos[i-1].left)
			return i;
	}
	return 0;
}

int EditableText::getLineEnd(int cursorPos) const
{
	if (cursorPos == m_charPos.size())
		return (int)m_charPos.size();

	for (int i=cursorPos; i<(int)m_charPos.size()-1; ++i)
	{
		if (m_charPos[i+1].left <= m_charPos[i].left)
			return i;
	}
	return (int)m_charPos.size();
}

std::string	EditableText::getSelectedText() const
{
	if (m_selectStart < m_selectEnd)
		return ((std::string)m_text).substr(m_selectStart, m_selectEnd-m_selectStart);
	else if (m_selectStart > m_selectEnd)
		return ((std::string)m_text).substr(m_selectEnd, m_selectStart-m_selectEnd);
	return std::string();
}