#pragma once
#include <filesystem>
#include <vector>
#include <set>

#include "ReflectionInfoCollector.h"


class HeaderAnalyzer
{
public:
	HeaderAnalyzer();
	bool Analyze(std::filesystem::path headerPath, std::vector<std::string>* outReflectionCodes);

	void SetIncludeRequired(bool requiredInclude);

private:
	bool _requiredInclude;
};
