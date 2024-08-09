﻿#pragma once

namespace refl
{
	using func = void;
	using prop = void;
}

#define REFL_CLASS()
#define REFL_CLASS(X)

#define NOT_AUTO

template<typename T>
class Tester
{
};


REFL_CLASS(NOT_AUTO)
class Example
{
public:
	Example() : _value(3), _valueF(2.5f) {}
	~Example() = default;

	[[refl::func]]void Print() const
	{
		//std::cout << "Hello, World!" << std::endl;
		//std::cout << _value << ", " << _valueF << std::endl;
	}

	[[nodiscard]] int GetValue() const
	{
		return _value;
	}

private:
	[[refl::prop]] int _value;
	float _valueF;
	Tester<int> _tester;
};


