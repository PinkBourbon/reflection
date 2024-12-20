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
			GENERATED_REFLECT(Callable);

			using FunctionType = ReturnType(T::*)(Args...) const;
		public:
			Callable(FunctionType ptr)
				: _ptr(ptr)
			{
			}

			ReturnType Invoke(const T* caller, Args&&... args) const
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
			Method(Type& owner, ReturnType(T::* ptr)(Args...), std::string_view name, const CallableBase* callable)
				: _name(name)
				, _callable(callable)
				, _returnType(Type::GetType<ReturnType>())
				, _paramTypes{ sizeof...(Args) }
			{
				owner.AddMethod(this);
				(_paramTypes.emplace_back(Type::GetType<Args>()), ...);
			}

			template<typename T, typename ReturnType, typename... Args>
			Method(Type& owner, ReturnType(T::* ptr)(Args...) const, std::string_view name, const CallableBase* callable)
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


			template<typename ReturnType, typename T, typename... Args>
			ReturnType Invoke(T* caller, Args&&... args) const
			{
				const Type* type = _callable->GetType();
				if (*type == *(Type::GetType<Callable<T, ReturnType(std::remove_cv_t<T>::*)(Args...)>>()))
				{
					// 일반 멤버 함수 호출
					// const 객체의 값을 바꾸는것은 UB지만 위 if문으로 인해 const 객체일 경우 이 구문에 들어오지 못한다.
					return static_cast<const Callable<std::remove_cv_t<T>, ReturnType(T::*)(Args...)>*>(_callable)->Invoke(const_cast<std::remove_cv_t<T>*>(caller), std::forward<Args>(args)...);
					//return static_cast<const Callable<T, ReturnType, Args...>*>(_callable)->Invoke(caller, std::forward<Args>(args)...);
				}
				else
				{
					// const 멤버 함수 호출
					return static_cast<const Callable<std::remove_cv_t<T>, ReturnType(T::*)(Args...) const>*>(_callable)->Invoke(caller, std::forward<Args>(args)...);
				}
			}

		private:
			std::string_view _name;
			const CallableBase* _callable;
			const Type* _returnType;
			std::vector<const Type*> _paramTypes;
		};

		template<typename ClassType, typename ReturnType, typename... Args>
		class MethodRegister
		{
		public:
			MethodRegister(std::string_view name, ReturnType(ClassType::* ptr)(Args...))
			{
				static Callable<ClassType, ReturnType, Args...> callable{ ptr };
				static Method method{ *Type::GetType<ClassType>(), ptr, name, callable };
			}
		};
	} // namespace refl
} // namespace flt
