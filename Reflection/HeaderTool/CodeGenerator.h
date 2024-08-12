#pragma once
#include <filesystem>

class CodeGenerator
{
public:
	CodeGenerator();
	bool Generate(std::filesystem::path headerPath);
private:

};

