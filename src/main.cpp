#include <iostream>
#include "config.hpp"
#include "EditorWindow.hpp"

int main()
{
    EditorWindow w("test.txt", true);
    w.save();
    w.close();
    return 0;
}