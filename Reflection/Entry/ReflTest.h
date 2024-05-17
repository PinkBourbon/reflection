#pragma once
#include "../Reflection/Type.h"
#include "ReflTest.generated.h"

class Parent
{
	GENERATED_REFLECT(Parent)
};

class ReflMember
{
	GENERATED_REFLECT(ReflMember)
};

CLASS()
class ReflTest : public Parent//, public Parent2
{
	GENERATED_REFLECT(ReflTest)
public:
	ReflTest();
	~ReflTest();

	REGIST_METHOD(TestFunc)
	void TestFunc(int a, float b, const char* c);

private:
	REGIST_METHOD(TestFunc2)
	char TestFunc2();
	
public:
	REGIST_PROPERTY(testInt)
	int testInt;

	REGIST_PROPERTY(pData)
	int* pData;

	REGIST_PROPERTY(reflMember)
	ReflMember reflMember;

	REGIST_PROPERTY(pReflMember)
	ReflMember* pReflMember;

private:
	REGIST_PROPERTY(testFloat)
	float testFloat;
};
