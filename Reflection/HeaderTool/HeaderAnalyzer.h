#pragma once
#include <filesystem>
#include <vector>
#include "clang-c/Index.h"


CXChildVisitResult visitNode(CXCursor cursor, CXCursor parent, void* client_data);

class HeaderAnalyzer
{
private:
	friend CXChildVisitResult visitNode(CXCursor cursor, CXCursor parent, void* client_data);
	struct FunctionData
	{
		std::string name;
		std::string returnType;
		std::vector<std::string> parameters;
	};

	struct MemberData
	{
		std::string name;
		std::string type;
		unsigned int line;
		bool isStatic;
	};

public:
	HeaderAnalyzer();

	bool Analyze(std::filesystem::path headerPath, std::vector<char>* buffer);


private:
	bool CreateData(std::vector<char>* buffer);

	void AddFunction();
	void AddMember();
	void FindClassSymbol() {_isfindClassSymbol = true; }

	std::vector<FunctionData> _functions;
	std::vector<MemberData> _members;
	bool _isfindClassSymbol;
};

