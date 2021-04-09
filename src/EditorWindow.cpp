#include "EditorWindow.hpp"

EditorWindow::EditorWindow(bool runNow)
{
    m_filename = "macroer-unnamed.sh";
    m_content = " ";
    m_cursorIndex = 0;
    m_scrollAmountY = 0;

    setupCurses();
    if (runNow) mainLoop();
}

EditorWindow::EditorWindow(std::string filename, bool runNow)
{
    m_filename = filename;
    m_cursorIndex = 0;
    m_scrollAmountY = 0;

    setupCurses();

    loadFromFile();
    if (runNow) mainLoop();
}

EditorWindow::~EditorWindow()
{
    close(true);
}

void EditorWindow::mainLoop()
{
    bool running = true;
    do
    {
        updateCursorPos();
        unsigned int c = getch();
        switch(c)
        {
            case KEY_UP:
                m_scrollAmountY --;
                break;
            case KEY_DOWN:
                m_scrollAmountY ++;
                break;
            case KEY_LEFT:
                if (m_cursorIndex > 0) m_cursorIndex --;
                break;
            case KEY_RIGHT:
                if (m_cursorIndex < m_content.length() - 1) m_cursorIndex ++;
                break;
            case KEY_BACKSPACE:
                deleteChar();
                break;
            case ctrl('x'):
                close(false);
                running = false;
                break;
            default:
                insertChar(c);
                break;
        }
        refresh();
    } while (running);
}

void EditorWindow::loadFromFile()
{

    std::ifstream file;
    file.open(m_filename);
    m_content = std::string((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    for (auto c : m_content)
    {
        insertChar(c);
    }

    debugLog(m_content);
}

void EditorWindow::save()
{
    std::ofstream file;
    file.open(m_filename, std::ios::trunc);
    file << m_content.substr(0, m_content.length() - 2);
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
    keypad(stdscr, TRUE);
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