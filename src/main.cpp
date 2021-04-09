#include <iostream>
#include <string>
#include "config.hpp"
#include "EditorWindow.hpp"

#define MACRO_SAVE_FILE_NAME std::string("macroer.sh")

void showIncorrectArgs()
{
    std::cout << "Invalid mode: (expected 'edit', 'run' or 'clear')\n";
}

std::string folderFromPath(std::string str)
{
    size_t found = str.find_last_of("/\\");
    return str.substr(0, found);
}

std::string macroSaveFilePath;

void clearMacroFile()
{
    std::ofstream file;
    file.open(macroSaveFilePath, std::ios::trunc);
    file << "";
    file.close();
}

int main(int argc, char* argv[])
{
    macroSaveFilePath = folderFromPath(std::string(argv[0])) +
        '/' + MACRO_SAVE_FILE_NAME;
    if (argc == 2)
    {
        if (argv[1] == std::string("edit"))
            EditorWindow w(macroSaveFilePath, true);
        else if (argv[1] == std::string("run"))
            system((std::string("sh ") + macroSaveFilePath).c_str());
        else if (argv[1] == std::string("clear"))
            clearMacroFile();
        else
            showIncorrectArgs();
    }
    else showIncorrectArgs();
    return 0;
}