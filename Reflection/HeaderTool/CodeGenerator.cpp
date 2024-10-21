#include "CodeGenerator.h"
#include "HeaderAnalyzer.h"
#include <fstream>

CodeGenerator::CodeGenerator()
{

}

bool CodeGenerator::Generate(std::filesystem::path headerPath, std::string macroPath)
{
	if (!std::filesystem::exists(headerPath))
	{
		return false;
	}

	std::filesystem::path absolutePath = std::filesystem::absolute(headerPath);
	std::filesystem::path dirPath{ absolutePath.parent_path().string() + "\\generated" };
	bool ret = create_directory(dirPath);

	std::string generatedHeaderName = dirPath.string() + "\\" + headerPath.stem().string() + "_generated.h";
	std::ofstream headerFile(generatedHeaderName, std::ios::out);
	if (!headerFile.is_open())
	{
		return false;
	}

	HeaderAnalyzer headerAnalyzer;
	std::vector<ReflectionData> reflectionDatas;

	if (!headerAnalyzer.Analyze(absolutePath, &reflectionDatas))
	{
		return false;
	}

	for (auto& c : macroPath)
	{
		if (c == ' ' || c == '\\' || c == '.' || c == '/')
		{
			c = '_';
		}
	}
	macroPath = "#define CURRENT_FILE " + macroPath + "\n";


	headerFile << macroPath;

	//headerFile.write(macroPath.data(), macroPath.size());
	//headerFile.write(generatedFileContextsBuffer.data(), generatedFileContextsBuffer.size());
	headerFile.close();

	return true;
}
