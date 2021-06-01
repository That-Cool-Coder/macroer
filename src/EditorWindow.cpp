#include "EditorWindow.hpp"

EditorWindow::EditorWindow()
{
    m_filename = DEFAULT_FILE_NAME;
    // m_content always has a space at the end to make deleting work,
    // but users can't interact with it
    m_content = " ";
    m_cursorIndex = 0;
    m_scrollAmountY = 0;

    setupCurses();
}

EditorWindow::EditorWindow(std::string filename)
{
    m_filename = filename;
    m_cursorIndex = 0;
    m_scrollAmountY = 0;

    setupCurses();
    loadFromFile();
}

EditorWindow::~EditorWindow()
{
    close(true);
}

void EditorWindow::mainLoop()
{
    // Load the content into the pad
    wclear(m_pad);
    waddstr(m_pad, m_content.c_str());

    bool running = true;
    while (running)
    {
        prefresh(m_pad, m_scrollAmountY, 0, 0, 0, m_terminalRows, m_terminalCols);

        int key = wgetch(m_pad);

        int cursorY, cursorX;
        getyx(m_pad, cursorY, cursorX);

        switch(key)
        {
            case ctrl('x'):
                running = false;
                close(false);
                break;
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                backspace();
                break;
            case KEY_UP:
                if (m_scrollAmountY > 0)
                {
                    m_scrollAmountY -= 1;
                    moveCursorBy(1, 0);
                }
                break;
            case KEY_DOWN:
                // todo: if m_scrollAmountY < count(\n) then continue
                m_scrollAmountY += 1;
                moveCursorBy(-1, 0);
                break;
            case KEY_LEFT:
                if (cursorY > 0)
                {
                    moveCursorBy(0, -1);
                }
                break;
            case KEY_RIGHT:
                moveCursorBy(0, 1);
                break;
            default:
                m_content += key;
                waddch(m_pad, (char) key);
        }
    }
}

void EditorWindow::loadFromFile()
{
    std::ifstream file;
    file.open(m_filename);
    // Add trailing space (see constructor)
    m_content = std::string((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    file.close();
}

void EditorWindow::save()
{
    std::ofstream file;
    file.open(m_filename, std::ios::trunc);
    // Remove trailing space:
    file << m_content;

    file.close();
}

void EditorWindow::close(bool forceClose)
{
    if (! forceClose) save();
    // Reset curses to original
    echo();
    nocbreak();
	endwin();
}

void EditorWindow::setupCurses()
{
	initscr();
	cbreak();
    noecho();

    getmaxyx(stdscr, m_terminalRows, m_terminalCols);
    m_pad = newpad(m_terminalRows, m_terminalCols);

    keypad(stdscr, true);
    keypad(m_pad, true);
}

// Below here is private
// ---------------------

void EditorWindow::debugLog(char c)
{
    move(10, 0);
    addch(c);
    updateCursorPos();
}

void EditorWindow::debugLog(std::string s)
{
    for (int i = 0; i < s.length(); i ++)
    {
        move(10, i);
        addch(s[i]);
        updateCursorPos();
    }
}

void EditorWindow::insertChar(char c)
{
    insch(c);
    m_content.insert(m_cursorIndex, std::string(1, c));
    m_cursorIndex ++;
}

void EditorWindow::deleteChar()
{
    if (m_cursorIndex > 0)
    {
        m_cursorIndex --;
        updateCursorPos();
        delch();
        m_content.erase(m_cursorIndex, 1);
    }
}

void EditorWindow::updateCursorPos()
{
    int row = 0;
    int col = 0;
    for (int i = 0; i < m_cursorIndex; i ++)
    {
        if (m_content[i] == '\n')
        {
            row ++;
            col = 0;
        }
        else col ++;
    }
    move(row, col);
}

void EditorWindow::tryRepaintLine(int lineNum)
{
    int row = 0;
    std::string crntLine = "";
    bool foundCorrectRow = false;
    for (auto c : m_content)
    {
        if (c == '\n')
        {
            if (row == lineNum)
            {
                foundCorrectRow = true;
                break;
            }
            else
            {
                row += 1;
                crntLine = "";
            }
        }
        else crntLine += c;
    }

    if (foundCorrectRow)
    {
        move(row, 0);
        addstr(crntLine.c_str());
        updateCursorPos();
    }
}

void EditorWindow::moveCursorBy(int yDist, int xDist)
{
    int crntY, crntX;
    getyx(m_pad, crntY, crntX);
    wmove(m_pad, crntY + yDist, crntX + xDist);
}

void EditorWindow::backspace()
{
    // Get cursor pos for later
    int cursorY, cursorX;
    getyx(m_pad, cursorY, cursorX);

    // If at start of line, then try deleting last char of prev line
    if (cursorX == 0)
    {
        // If this is the start of file then quit
        if (cursorY == 0) return;

        // Otherwise try to reach the end of the previous line
        int x = 1000;
        char crntChar;
        do
        {
            crntChar = mvwinch(m_pad, cursorY - 1, x) & A_CHARTEXT;
            x --;
        } while (crntChar != 0 && x > 0);

        int endOfLastLine = x + 1;
        wmove(m_pad, cursorY - 1, endOfLastLine);
        waddch(m_pad, '!');
        mvwdelch(m_pad, cursorY - 1, endOfLastLine);
    }
    else
    {
        mvwdelch(m_pad, cursorY, cursorX - 1);
    }
}

int EditorWindow::getLineLength(int lineNum)
{
    // save the previous cursor pos so we can restore it
    // move the cursor to the far right of the screen on lineNum
    // start moving to the left until we reach a non empty character
    // the position one to the right of the pos reached is line length
    // restore the original cursor pos
    // return pos
}