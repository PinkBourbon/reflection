#pragma once
#include "../Reflection/Reflection.h"


class Base
{
	GENERATED_REFLECT(Base)

public:
	REGIST_PROPERTY(baseInt)
	int baseInt;

	REGIST_METHOD(Func)
	void Func();

private:
	REGIST_PROPERTY(_baseFloat)
	float _baseFloat;
};

class Derived : public Base
{
	GENERATED_REFLECT(Derived)

public:
	REGIST_PROPERTY(derivedInt)
	int derivedInt;

	float derivedFloat;
private:

	REGIST_PROPERTY(_derivedInt)
	int _derivedInt;
};
