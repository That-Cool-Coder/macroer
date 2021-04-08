#include "EditorWindow.hpp"

EditorWindow::EditorWindow(std::string filename, bool runNow)
{
    m_filename = filename;
    m_content = "";
	initscr();
	cbreak();
    noecho();
    keypad(stdscr, TRUE);
    if (runNow) mainLoop();
}

void EditorWindow::mainLoop()
{
    while (true)
    {
        char c = getch();
        if (c == KEY_UP) m_scrollAmountY --;
        else if (c == KEY_DOWN) m_scrollAmountY ++;
        else addch(c);
        move(10, 10);
        refresh();
    }
}

void EditorWindow::save()
{
    std::ofstream file;
    file.open(m_filename, std::ios::trunc);
    file << m_content;
    file.close();
}

void EditorWindow::close()
{
    echo();
    nocbreak();
	endwin();
}