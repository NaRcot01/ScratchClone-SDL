#include "project_io.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

static void saveSprite(std::ofstream& out, const Sprite& s) {
	out << "SPRITE\n";
	out << "name " << s.name << "\n";
	out << "x " << s.exact_x << "\n";
	out << "y " << s.exact_y << "\n";
	out << "direction " << s.direction << "\n";
	out << "size " << s.size << "\n";
	out << "visible " << (s.isVisible ? 1 : 0) << "\n";
	out << "rotation" << s.rotation << "\n";
	out << "costume " << s.costumeIndex << "\n";
	out << "SCRIPTS " << s.scripts.size() << "\n";

	for (const Script& script : s.scripts) {
	    out << "SCRIPT " << (script.isActive ? 1 : 0) << " BLOCKS " << script.blocks.size() << "\n";   
	    for (const Block& b : script.blocks) {
		out << "BLOCK ";
		out << static_cast<int>(b.type) << " ";
		out << b.jumpToIndex << " ";
		out << std::setprecision(17);
		out << b.parameters.size();
		
		for (double v : b.parameters) {
			out << " " << v;
		}
		out << " " << b.textParam << "\n";
		}
	}
	out << "END_SPRITE\n";
}

bool saveProject(const Engine& engine, const std::string& path) {
	std::ofstream out(path);
	if (!out.is_open()) return false;
	out << "PROJECT\n";
	out << "SPRITES " << engine.sprites.size() << "\n";
	for (const Sprite& s : engine.sprites) {
		saveSprite(out, s);
	}
	out << "END_PROJECT\n";
	return true;
}

static bool loadSprite(std::istream& in, Sprite& s) {
	std::string token;
	while (in >> token) {
		if (token == "END_SPRITE") break;
		else if (token == "name") {
		in >> s.name;
		}
		else if (token == "x") {
		in >> s.exact_x;
		s.rect.x = static_cast<int>(s.exact_x);
		}
		else if (token == "y") {
		in >> s.exact_y;
		s.rect.y = static_cast<int>(s.exact_y);
		}
		else if (token == "direction") {
		in >> s.direction;
		}
		else if (token == "size") {
		in >> s.size;
		updateSpriteSize(&s);
		}
		else if (token == "visible") {
		int v; in >> v;
		s.isVisible = (v != 0);
		}
		else if (token == "rotation") {
		in >> s.rotation;
		}
		else if (token == "costume") {
		in >> s.costumeIndex;
		}
		else if (token == "SCRIPTS") {
		size_t scriptCount;
		in >> scriptCount;
		s.scripts.clear();
		s.scripts.reserve(scriptCount);
		for (size_t i=0; i < scriptCount; i++) {
			std::string scriptTag;
			in >> scriptTag;
			Script script;
			int isAct; in >> isAct;
			script.isActive = (isAct != 0);
			std::string sub;
			in >> sub;
			size_t blockCount; in >> blockCount;
			script.blocks.clear();
			script.blocks.reserve(blockCount);
			for (size_t b = 0; b < blockCount; ++b) {
				std::string blockToken;
				in >> blockToken;
				Block blk;
				int typeInt;
				in >> typeInt;
				blk.type = static_cast<BlockType>(typeInt);
				in >> blk.jumpToIndex;
				size_t paramCount;
				in >> paramCount;
				blk.parameters.clear();
				blk.parameters.reserve(paramCount);
				for (size_t p = 0; p < paramCount; ++p) {
					double v; in >> v;
					blk.parameters.push_back(v);
				}
				std::string txt;
				in >> txt;
				if (txt != "-") blk.textParam = txt;
				script.blocks.push_back(blk);
			}
			preprocessScript(script.blocks);
			s.scripts.push_back(script);
		}
	}
    }
    return true;
}

bool loadProject(Engine& engine, const std::string& path) {
	std::ifstream in(path);
	if (!in.is_open()) {
		std::cerr << "Failed to open project file: " << path << std::endl;
		return false;
	}
	std::string header;
	in >> header;
	if (header != "PROJECT") {
		std::cerr << "Invalid project file hrader\n";
		return false;
	}
	
	std::string spriteHeader;
	size_t spriteCount;
	in >> spriteHeader;
	engine.sprites.clear();
	engine.sprites.resize(spriteCount);
	
	for (size_t i=0; i < spriteCount; ++i) {
		std::string spriteTag;
		in >> spriteTag;
		initSprite(engine.sprites[i], &engine.stage);
		loadSprite(in, engine.sprites[i]);
	}

	std::string endProj;
	in >> endProj;

	return true;
}
