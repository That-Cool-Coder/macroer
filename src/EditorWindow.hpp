#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ncurses.h>
#include "config.hpp"

#define ctrl(x) ((x) & 0x1f)

class EditorWindow
{
public:

    EditorWindow(bool runNow);
    EditorWindow(std::string filename, bool runNow);
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

    void setupCurses();
    void insertChar(char c);
    void deleteChar();
    void updateCursorPos();
    void tryRepaintLine(int lineNum);
    void debugLog(char c);
    void debugLog(std::string s);
};