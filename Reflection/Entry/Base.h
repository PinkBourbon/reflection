#pragma once
#include "../Reflection/Reflection.h"


class Base
{
	GENERATED_REFLECT(Base)

public:

	REGIST_PROPERTY(baseInt)
	int baseInt = 10;

	REGIST_METHOD(Func)
	void Func(int i);

private:
	REGIST_PROPERTY(_baseFloat)
	float _baseFloat = 10.1f;
};

class Derived : public Base
{
	GENERATED_REFLECT(Derived)

public:
	REGIST_PROPERTY(derivedInt)
	int derivedInt = 20;

	float derivedFloat = 20.2f;
private:

	REGIST_PROPERTY(_derivedInt)
	int _derivedInt = 22;
};
