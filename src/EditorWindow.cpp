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

        switch(key)
        {
            case ctrl('x'):
                running = false;
                close(false);
                break;
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                int prev_x, prev_y;
                getyx(m_pad, prev_y, prev_x);
                mvwdelch(m_pad, prev_y, prev_x - 1);
                break;
            case KEY_UP:
                m_scrollAmountY -= 1;
                break;
            case KEY_DOWN:
                m_scrollAmountY += 1;
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