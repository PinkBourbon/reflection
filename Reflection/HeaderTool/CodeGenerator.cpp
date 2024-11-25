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
	std::vector<std::string> reflectionDatas;

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
		else if (c >= 'a' && c <= 'z')
		{
			c -= 32;
		}
	}


	std::string generatedCode = "#undef CURRENT_FILE_PATH\n";
	generatedCode += "#define CURRENT_FILE_PATH FTL_FILE_PATH_" + macroPath + "\n\n";
	generatedCode += "#include \"../../Reflection/Reflection.h\"\n\n";

	// TODO: 리플렉션 데이터에 관련된 코드 생성 후 파일에 쓰기 구현
	headerFile.write(generatedCode.data(), generatedCode.size());

	std::string reflectionBodyMacro = "#define FTL_FILE_PATH_" + macroPath;
	for (auto& reflectionData : reflectionDatas)
	{
		headerFile.write(reflectionBodyMacro.data(), reflectionBodyMacro.size());
		headerFile.write(reflectionData.data(), reflectionData.size());
		headerFile.write("\n", 1);
	}


	headerFile.close();

	return true;
}
