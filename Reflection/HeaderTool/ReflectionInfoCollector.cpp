#include "ReflectionInfoCollector.h"

bool ReflectionInfoCollector::IsReflectionTarget(int line)
{
	if (IsAllReflectionClass())
	{
		return true;
	}

	if (_reflectionTargetLine.find(line) != _reflectionTargetLine.end())
	{
		return true;
	}

	return false;
}

void ReflectionInfoCollector::PushDepth(std::string name)
{
	_depth.push_back(name);
}

void ReflectionInfoCollector::PopDepth(std::string name)
{
	if (_depth.back() == name)
	{
		_depth.pop_back();
		return;
	}
	else
	{
		// 스택 꼬임 디버깅용
		__debugbreak();
	}
}

std::string ReflectionInfoCollector::GetDepth()
{
	std::string depth;
	for (auto& name : _depth)
	{
		depth += name;
		depth += "::";
	}

	return depth;
}

bool ReflectionInfoCollector::IsAllReflectionClass()
{
	__debugbreak(); // 아직 미구현.
	return true;
}
