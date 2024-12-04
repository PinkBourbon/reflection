#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>

struct ReflectionData
{
	ReflectionData();

	std::string name;
	std::string scopedName;
	unsigned int classLine;
	unsigned int bodyLine;
	std::vector<std::string> baseClass;
	std::vector<std::string> method;
	std::vector<std::string> field;
	bool isAllReflectionTarget;
};


class ReflectionInfoCollector
{
public:
	ReflectionInfoCollector();

	bool IsReflectionTarget(int line);
	//const std::vector<ReflectionData> GetReflectionTarget(int classDeclLine);
	void EnterScope(std::string name, int line);
	void ExitScope(std::string name);
	std::string GetScope();
	std::string GetMacroScopeName();

	void AddReflectionTarget(int line);
	void SetReflectionTargetAll(int line);
	void AddBaseClass(const std::string& scopedName);
	void AddMethod(const std::string& name);
	void AddField(const std::string& name);
	void AddReflBodyLine(unsigned int line);

	void GenerateReflectionCode(std::vector<std::string>* outReflectionCodes);

private:
	bool IsAllReflectionClass();

private:
	std::vector<std::string> _scope;
	std::map<int, bool> _reflectionTargetLine;
	std::map<std::string, ReflectionData> _reflectionDataMap;
	std::list<unsigned int> _reflBodyLines;
};
