#pragma once
#include "../Reflection/Type.h"
#include "ReflTest.generated.h"

CLASS()
class ReflTest
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

private:
	REGIST_PROPERTY(testFloat)
	float testFloat;
};
