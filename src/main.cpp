#include <iostream>
#include <string.h>
#include "config.hpp"
#include "getExecutableName.hpp"
#include "EditorWindow.hpp"

#if defined(WIN32) || defined(_WIN32)
    #define MACRO_SAVE_FILE_NAME std::string("macroer.bat")
    #define DIRECTORY_SEPERATOR std::string("\\")
#else
    #define MACRO_SAVE_FILE_NAME "macroer.sh"
    #define DIRECTORY_SEPERATOR std::string("/")
#endif

#define DEFAULT_MACRO_FILE_CONTENTS "# Welcome to macroer. To save and exit, press CTRL+X "

std::string macroSaveFilePath;

void showIncorrectArgs()
{
    std::cout << "Invalid mode (expected 'edit', 'run' or 'clear')\n";
}

std::string folderFromPath(std::string str)
{
    size_t found = str.find_last_of(DIRECTORY_SEPERATOR);
    return str.substr(0, found);
}

void clearMacroFile()
{
    std::ofstream file;
    file.open(macroSaveFilePath, std::ios::trunc);
    file << DEFAULT_MACRO_FILE_CONTENTS;
    file.close();
}

int main(int argc, char* argv[])
{
    // Set the path to be local to the executable, not the working dir
    macroSaveFilePath = folderFromPath(getFullExecutableName()) +
        DIRECTORY_SEPERATOR + MACRO_SAVE_FILE_NAME;
    
    // If we have a name and a mode:
    if (argc == 2)
    {
        std::string mode = argv[1];

        // A switch-case can't work here because c doesn't like strings
        if (mode == std::string("edit"))
        {
            EditorWindow w(macroSaveFilePath);
            w.mainLoop();
        }
        else if (mode == std::string("run")) 
            system(std::string('"' + macroSaveFilePath + '"').c_str());
        else if (mode == std::string("clear"))
            clearMacroFile();
        else
            showIncorrectArgs();
    }
    else showIncorrectArgs();
    //std::cout << folderFromPath(std::string(argv[0]));
    
}