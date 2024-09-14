#include "CodeGenerator.h"
#include "HeaderAnalyzer.h"
#include <fstream>

CodeGenerator::CodeGenerator()
{

}

bool CodeGenerator::Generate(std::filesystem::path headerPath)
{
	if (!std::filesystem::exists(headerPath))
	{
		return false;
	}

	std::filesystem::path absolutePath = std::filesystem::absolute(headerPath);
	std::filesystem::path dirPath{ absolutePath.parent_path().string() + "\\generated" };
	bool ret = create_directory(dirPath);

	HeaderAnalyzer headerAnalyzer;
	std::vector<char> buffer;
	if (!headerAnalyzer.Analyze(absolutePath, &buffer))
	{
		return false;
	}

	std::string generatedHeaderName = dirPath.string() + "\\" + headerPath.stem().string() + "_generated.h";
	std::ofstream headerFile(generatedHeaderName, std::ios::out);
	if (!headerFile.is_open())
	{
		return false;
	}
	headerFile.write(buffer.data(), buffer.size());
	headerFile.close();

	return true;
}
