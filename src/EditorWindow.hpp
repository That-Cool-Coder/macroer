#pragma once
#include <iostream>
#include <fstream>
#include "crossPlatformCurses.hpp"
#include "config.hpp"

#define ctrl(x) ((x) & 0x1f)
#define isbackspace(x) ((x) == KEY_BACKSPACE || (x) == 127 || (x) == '\b')
#define istab(x) ((x) == '\t')

#define DEFAULT_FILE_NAME "unnamed.txt"

class EditorWindow
{
public:
    EditorWindow();
    EditorWindow(std::string filename);
    ~EditorWindow();

    void mainLoop();
    void loadFromFile();
    void save();
    void close(bool forceClose);

private:
    std::string m_filename;
    std::string m_content;
    int m_scrollAmountY;
    int m_cursorIndex;
    int m_terminalCols;
    int m_terminalRows;
    WINDOW* m_pad;

    void setupCurses();
    void insertChar(char c);
    void deleteChar();
    void updateCursorPos();
    void tryRepaintLine(int lineNum);
    void debugLog(char c);
    void debugLog(std::string s);
    void moveCursorBy(int y, int x);
    void backspace();
    int getLineLength(int lineNum);
};