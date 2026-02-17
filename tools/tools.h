//
// Created by jack on 2/6/2026.
//

#pragma once
#include <string>
#include <random>
#include <ctime>
#include <fstream>

bool copyFile(std::string sourcePath, std::string destPath);
std::string generateRandomName(int length = 2);
