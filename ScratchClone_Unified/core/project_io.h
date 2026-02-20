#pragma once
#include <string>
#include <vector>
#include "engine.h"

bool saveProject(const Engine& engine, const std::string& path);
bool loadProject(Engine& engine, const std::string& path);