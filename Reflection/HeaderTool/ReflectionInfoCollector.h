#pragma once
#include <string>
#include <vector>
#include <set>

class ReflectionInfoCollector
{
public:
	bool IsReflectionTarget(int line);
	//const std::vector<ReflectionData> GetReflectionTarget(int classDeclLine);
	void PushDepth(std::string name);
	void PopDepth(std::string name);
	std::string GetDepth();

private:
	bool IsAllReflectionClass();

private:
	std::vector<std::string> _depth;
	std::set<int> _reflectionTargetLine;
};

