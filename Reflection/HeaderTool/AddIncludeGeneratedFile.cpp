#pragma once
#include "AddIncludeGeneratedFile.h"
#include <fstream>


void AddIncludeGeneratedFile(std::filesystem::path headerPath)
{
	std::fstream headerFile(headerPath, std::ios::in | std::ios::out);
	if (!headerFile.is_open())
	{
		return;
	}

	std::string includePrefix = "#include";

	headerFile.seekg(0, std::ios::end);
	size_t fileSize = headerFile.tellg();
	headerFile.seekg(0);
	std::string generatedHeaderName = headerPath.stem().string() + "_generated.h";
	std::vector<char> buffer;
	buffer.reserve(fileSize);
	buffer.insert(buffer.end(), includePrefix.begin(), includePrefix.end());
	buffer.push_back(' ');
	buffer.push_back('\"');
	buffer.insert(buffer.end(), generatedHeaderName.begin(), generatedHeaderName.end());
	buffer.push_back('\"');
	buffer.push_back('\n');


	std::streampos lastInclude = headerFile.tellg();
	while (true)
	{
		std::string line;
		if (!std::getline(headerFile, line))
		{
			break;
		}

		if(line.find(includePrefix) != std::string::npos || line.find("#pragma once") != std::string::npos)
		{
			if (line.find(generatedHeaderName) != std::string::npos)
			{
				return;
			}

			lastInclude = headerFile.tellg();
		}
	}
	headerFile.clear();
	headerFile.seekg(lastInclude);
	while(true)
	{
		std::string line;
		if (!std::getline(headerFile, line))
		{
			break;
		}
		buffer.insert(buffer.end(), line.begin(), line.end());
		buffer.push_back('\n');
	}

	headerFile.clear();
	headerFile.seekp(lastInclude);
	if(headerFile.tellp() != lastInclude)
	{
		int i = 0;

	}


	headerFile.write(buffer.data(), buffer.size());

	if (!headerFile.good())
	{
		int i = 0;
	}

	headerFile.flush();
	headerFile.close();
}
