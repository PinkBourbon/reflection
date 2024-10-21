#include "ReflectionInfoCollector.h"

ReflectionInfoCollector::ReflectionInfoCollector()
	: _scope()
	, _reflectionTargetLine()
	, _reflectionDataMap()
{

}

bool ReflectionInfoCollector::IsReflectionTarget(int line)
{
	if (IsAllReflectionClass())
	{
		return true;
	}

	if (_reflectionTargetLine.find(line - 1) != _reflectionTargetLine.end())
	{
		return true;
	}

	return false;
}

void ReflectionInfoCollector::EnterScope(std::string name, int line)
{
	_scope.push_back(name);

	if (IsReflectionTarget(line))
	{
		_reflectionDataMap[GetScope()].name = GetMacroScopeName();
	}
}

void ReflectionInfoCollector::ExitScope(std::string name)
{
	if (_scope.back() == name)
	{
		_scope.pop_back();
		return;
	}
	else
	{
		// 스택 꼬임 디버깅용
		__debugbreak();
	}
}

std::string ReflectionInfoCollector::GetScope()
{
	std::string scope;
	for (auto& name : _scope)
	{
		scope += "::";
		scope += name;
	}

	return scope;
}

std::string ReflectionInfoCollector::GetMacroScopeName()
{
	std::string scope;
	for (int i = 0; i < _scope.size() - 1; ++i)
	{
		scope += _scope[i];
		scope += "__";
	}
	scope += _scope.back();

	//for (auto& name : _scope)
	//{
	//	scope += "__";
	//	scope += name;
	//}

	return scope;
}

void ReflectionInfoCollector::AddReflectionTarget(int line)
{
	_reflectionTargetLine.insert({line, false});
}

void ReflectionInfoCollector::SetReflectionTargetAll(int line)
{
	_reflectionTargetLine[line] = true;
}

void ReflectionInfoCollector::AddMethod(const std::string& name)
{
	std::string scope = GetScope();
	_reflectionDataMap[scope].method.push_back(name);
}

void ReflectionInfoCollector::AddField(const std::string& name)
{
	std::string scope = GetScope();
	_reflectionDataMap[scope].field.push_back(name);
}

bool ReflectionInfoCollector::IsAllReflectionClass()
{
	auto iter = _reflectionDataMap.find(GetScope());
	if (iter == _reflectionDataMap.end())
	{
		return false;
	}

	return iter->second.isAllReflectionTarget;
}

ReflectionData::ReflectionData()
	: name()
	, method()
	, field()
	, isAllReflectionTarget(false)
{

}
