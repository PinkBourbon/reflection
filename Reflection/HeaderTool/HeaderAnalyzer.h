#pragma once
#include <filesystem>
#include <vector>
#include <set>

#pragma warning(push)
#pragma warning(disable : 4996)
#include "clang-c/Index.h"
#pragma warning(pop)

#include "ReflectionInfoCollector.h"


CXChildVisitResult visitNode(CXCursor cursor, CXCursor parent, void* client_data);

struct ReflectionData
{

};

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

	bool Analyze(std::filesystem::path headerPath, std::vector<ReflectionData>* outReflectionDatas);

//	// visitNode에서 사용할 함수들
//protected:
//	bool IsReflectionTarget(int line);
//	const std::vector<ReflectionData> GetReflectionTarget(int classDeclLine);
//	void PushDepth(std::string name);
//	void PopDepth(std::string name);
//	std::string GetDepth();

private:
	//bool CreateData(std::vector<char>* buffer);

	void AddFunction();
	void AddMember();


	std::vector<FunctionData> _functions;
	std::vector<MemberData> _members;
};

