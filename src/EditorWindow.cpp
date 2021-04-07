#include "editorWindow.hpp"

EditorWindow::EditorWindow(std::string filename, bool runNow)
{
    m_filename = filename;
    m_content = "";
    if (runNow) mainLoop();
}

char EditorWindow::charIn()
{
    return char(_getch());
}

void EditorWindow::mainLoop()
{
    int i = 0;
    while (i < 10)
    {
        std::cout << charIn() << std::endl;
        i ++;
    }
}

void EditorWindow::save()
{
    std::ofstream file;
    file.open(m_filename, std::ios::trunc);
    file << m_content;
    file.close();
}