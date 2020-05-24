#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include <wchar.h>
#include <stdio.h>
#include <conio.h>

std::vector<std::wstring> getDroppedFiles(); //get dragged and dropped files and stop when pressed enter
char encryptChar(char c, int key);
std::wstring widen(const std::string& str);
std::string narrow(const std::wstring& str);