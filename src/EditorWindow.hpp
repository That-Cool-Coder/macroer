#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include "config.hpp"

void test();

class EditorWindow
{
private:
    std::string m_filename;
    std::string m_content;

    char charIn();

public:
    EditorWindow(std::string filename, bool runNow);

    void mainLoop();
    void save();
};