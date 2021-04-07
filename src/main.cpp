#include <iostream>
#include "config.hpp"
#include "EditorWindow.hpp"

int main()
{
    EditorWindow w("test.txt", true);
    w.save();
    return 0;
}