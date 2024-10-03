#pragma once
#include <filesystem>
#include <string>

class CodeGenerator
{
public:
	CodeGenerator();
	bool Generate(std::filesystem::path headerPath, std::string macroPath);
private:

};

