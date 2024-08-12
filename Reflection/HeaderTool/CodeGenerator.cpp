#include "CodeGenerator.h"
#include "HeaderAnalyzer.h"
#include <fstream>

CodeGenerator::CodeGenerator()
{

}

bool CodeGenerator::Generate(std::filesystem::path headerPath)
{
	std::filesystem::path absolutePath = std::filesystem::absolute(headerPath);

	HeaderAnalyzer headerAnalyzer;
	std::vector<char> buffer;
	if(!headerAnalyzer.Analyze(absolutePath, &buffer))
	{
		return false;
	}

	std::string generatedHeaderName = absolutePath.parent_path().string() + "\\" + headerPath.stem().string() + "_generated.h";
	std::ofstream headerFile(generatedHeaderName, std::ios::out);
	if (!headerFile.is_open())
	{
		return false;
	}
	headerFile.write(buffer.data(), buffer.size());
	headerFile.close();

	return true;
}
