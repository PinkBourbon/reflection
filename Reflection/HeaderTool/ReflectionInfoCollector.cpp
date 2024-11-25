#include "ReflectionInfoCollector.h"


#include <iostream>

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
		_reflectionDataMap[GetScope()].line = line;
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
	_reflectionTargetLine.insert({ line, false });
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

void ReflectionInfoCollector::GenerateReflectionCode(std::vector<std::string>* outReflectionCodes)
{
	for (auto& [scope, reflectionData] : _reflectionDataMap)
	{
		std::string reflectionCode = std::to_string(reflectionData.line);
		reflectionCode += "_FLT_REFL";
		reflectionCode += "\\\n";
		reflectionCode += "public:\\\n";
		reflectionCode += "\tvirtual flt::refl::Type* GetType() const\\\n";
		reflectionCode += "\t{\\\n";
		reflectionCode += "\t\treturn _type;\\\n";
		reflectionCode += "\t}\\\n\\\n";

		/// InitType 함수 생성
		reflectionCode += "\tstatic flt::refl::Type* InitType()\\\n";
		reflectionCode += "\t{\\\n";
		reflectionCode += "\t\tstatic flt::refl::Type s_type\\\n\\\n";

		for (auto& method : reflectionData.method)
		{
			reflectionCode += "\t\t{static Method method(&s_type, &";
			reflectionCode += reflectionData.name;
			reflectionCode += "::";
			reflectionCode += method;
			reflectionCode += ", \"";
			reflectionCode += method;
			reflectionCode += "\", new Callable(&";
			reflectionCode += reflectionData.name;
			reflectionCode += "::";
			reflectionCode += method;
			reflectionCode += "));}\\\n";
		}

		reflectionCode += "\\\n";

		for (auto& field : reflectionData.field)
		{
			reflectionCode += "\t\t{static Property property(&s_type, ";
			reflectionCode += field;
			reflectionCode += ", Type::GetType<decltype(";
			reflectionCode += field;
			reflectionCode += ")>(), new PropertyHandler(&";
			reflectionCode += reflectionData.name;
			reflectionCode += "::";
			reflectionCode += field;
			reflectionCode += "));}\\\n";
		}

		reflectionCode += "\\\n";
		reflectionCode += "\t\treturn &s_type;\\\n";
		reflectionCode += "\t}\\\n";
		reflectionCode += "\\\n";
		reflectionCode += "private:\\\n";
		reflectionCode += "\tinline static flt::refl::Type* _type = InitType();\n";

		std::cout << reflectionCode << std::endl;

		outReflectionCodes->push_back(std::move(reflectionCode));
	}
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
