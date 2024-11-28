﻿#pragma once
#include <iostream>
#include "TypeMacro.h"

namespace flt
{
	namespace refl
	{
		class CallableBase
		{
			GENERATED_REFLECT(CallableBase)
		public:
			virtual ~CallableBase() = default;
		};

		template <typename ClassType, typename ReturnType, typename... Args>
		class Callable;

		template<typename T, typename ReturnType, typename... Args>
		class Callable<T, ReturnType(T::*)(Args...)> : public CallableBase
		{
			GENERATED_REFLECT(Callable)

			using FunctionType = ReturnType(T::*)(Args...); 
		public:
			Callable(FunctionType ptr)
				: _ptr(ptr)
			{
			}

			ReturnType Invoke(T* caller, Args... args) const
			{
				return (caller->*_ptr)(std::forward<Args>(args)...);
			}

		private:
			FunctionType _ptr;
		};


		template<typename T, typename ReturnType, typename... Args>
		class Callable<T, ReturnType(T::*)(Args...) const> : public CallableBase
		{
			GENERATED_REFLECT(Callable)

				using FunctionType = ReturnType(T::*)(Args...) const;
		public:
			Callable(FunctionType ptr)
				: _ptr(ptr)
			{
			}

			ReturnType Invoke(const T* caller, Args... args) const
			{
				return (caller->*_ptr)(std::forward<Args>(args)...);
			}

		private:
			FunctionType _ptr;
		};

		template <typename T, typename ReturnType, typename... Args>
		Callable(ReturnType(T::*)(Args...)) -> Callable<T, ReturnType(T::*)(Args...)>;

		template <typename T, typename ReturnType, typename... Args>
		Callable(ReturnType(T::*)(Args...) const) -> Callable<T, ReturnType(T::*)(Args...) const>;

		class Method
		{
			friend class flt::refl::Type;
		public:
			template<typename T, typename ReturnType, typename... Args>
			Method(Type& owner, ReturnType(T::* ptr)(Args...), std::string_view name, const CallableBase& callable)
				: _name(name)
				, _callable(callable)
				, _returnType(Type::GetType<ReturnType>())
				, _paramTypes{ sizeof...(Args)}
			{
				owner.AddMethod(this);
				(_paramTypes.emplace_back(Type::GetType<Args>()), ...);
			}

			template<typename T, typename ReturnType, typename... Args>
			Method(Type& owner, ReturnType(T::* ptr)(Args...) const, std::string_view name, const CallableBase& callable)
				: _name(name)
				, _callable(callable)
				, _returnType(Type::GetType<ReturnType>())
				, _paramTypes{ sizeof...(Args) }
			{
				owner.AddMethod(this);
				(_paramTypes.emplace_back(Type::GetType<Args>()), ...);
			}

			std::string_view Name() const
			{
				return _name;
			}

			// TODO : const 멤버 함수냐 아니냐에 따라서 오류 등을 내줘야 한다.
			template<typename ReturnType, typename T, typename... Args>
			ReturnType Invoke(T* caller, Args... args) const
			{
				const Type* type = _callable.GetType();
				const Type* type2 = Type::GetType<Callable<T, ReturnType(T::*)(Args...)>>();
				const Type* type3 = Type::GetType<Callable<const T, ReturnType(T::*)(Args...) const>>();
				const Type* type4 = Type::GetType<Callable<T, ReturnType(T::*)(Args...) const>>();
				if (*type == *(Type::GetType<Callable<T, ReturnType(T::*)(Args...)>>()))
				{
					return ReturnType();
					//return static_cast<const Callable<T, ReturnType(T::*)(Args...)>&>(_callable).Invoke(caller, std::forward<Args>(args)...);
				}
				else if (*type == *(Type::GetType<Callable<T, ReturnType(T::*)(Args...) const>>()))
				{
					return ReturnType();
				}

				return ReturnType();

				//return static_cast<const Callable<T, ReturnType(T::*)(Args...) const>&>(_callable).Invoke(caller, std::forward<Args>(args)...);

			}

		private:
			std::string_view _name;
			const CallableBase& _callable;
			const Type* _returnType;
			std::vector<const Type*> _paramTypes;
		};

		template<typename ClassType, typename ReturnType, typename... Args>
		class MethodRegister
		{
		public:
			MethodRegister(std::string_view name, ReturnType(ClassType::* ptr)(Args...))
			{
				static Callable<ClassType, ReturnType, Args...> callable{ptr};
				static Method method{*Type::GetType<ClassType>(), ptr, name, callable};
			}
		};
	} // namespace refl
} // namespace flt
