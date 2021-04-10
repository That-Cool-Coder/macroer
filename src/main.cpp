#include <iostream>
#include <string>
#include "config.hpp"
#include "EditorWindow.hpp"

#define MACRO_SAVE_FILE_NAME std::string("macroer.sh")

std::string macroSaveFilePath;

void showIncorrectArgs()
{
    std::cout << "Invalid mode (expected 'edit', 'run' or 'clear')\n";
}

std::string folderFromPath(std::string str)
{
    size_t found = str.find_last_of("/\\");
    return str.substr(0, found);
}

void clearMacroFile()
{
    std::ofstream file;
    file.open(macroSaveFilePath, std::ios::trunc);
    file << "";
    file.close();
}

int main(int argc, char* argv[])
{
    // Set the path to be local to the executable, not the working dir
    macroSaveFilePath = folderFromPath(std::string(argv[0])) +
        '/' + MACRO_SAVE_FILE_NAME;
    
    // If we have a self name and a mode:
    if (argc == 2)
    {
        // A switch-case can't work here because of std::string
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
}