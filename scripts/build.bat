@echo off
setlocal
cd %~dp0
cd ..\build
cmake -G "MinGW Makefiles" ..
cmake --build .