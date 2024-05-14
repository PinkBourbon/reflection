#pragma once
#include <string_view>
#include <type_traits>
#include "hash.h"

namespace flt
{

	namespace refl
	{
		class Type;
		class Method;
		class Property;

		template <typename T>
		concept HasSuper = requires	{typename T::Super; } && !std::same_as<typename T::Super, void>;

		template <typename T>
		concept HasType = requires { T::GetType(); };

		template<typename T, typename U = void>
		struct SuperType
		{
			using Type = void;
		};

		template<typename T>
		struct SuperType<T, std::void_t<typename T::ThisType>>
		{
			using Type = typename T::ThisType;
		};

		template<typename T>
		using SuperType_t = typename SuperType<T>::Type;


		template <typename T>
		struct TypeBuilder
		{
			constexpr TypeBuilder(std::string_view typeName) :
				_typeName(typeName), 
				_rawTypeName(GetRawTypaName()), 
				_hash(),
				_super(nullptr)
			{
				if constexpr (HasSuper<T>)
				{
					_super = &T::Super::GetType();
				}

				_hash = xxh64::hash(_rawTypeName.data(), _rawTypeName.length(), 0);
			}

		private:
			static constexpr std::string_view GetRawTypaName() noexcept
			{
				constexpr std::string_view full_name{ __FUNCSIG__ };
				constexpr std::string_view prefix{ "TypeBuilder<" };
				constexpr std::string_view suffix{ ">::GetRawTypaName(" };

				constexpr auto left_marker_index = full_name.find(prefix);
				static_assert(left_marker_index != std::string_view::npos);
				constexpr auto start_index = left_marker_index + prefix.size();
				constexpr auto end_index = full_name.find(suffix, left_marker_index);
				static_assert(end_index != std::string_view::npos);
				constexpr auto length = end_index - start_index;

				return full_name.substr(start_index, length);
			}

		public:

			std::string_view _typeName;
			std::string_view _rawTypeName;
			unsigned long long _hash;
			const Type* _super;
		};


		class Type
		{
		public:
			template <typename T>
			explicit Type(const TypeBuilder<T>& builder);

		private:


		public:
			std::string_view _typeName;
			std::string_view _rawTypeName;
			unsigned long long _hash;
			const Type* _super;
		};

		template <typename T>
		flt::refl::Type::Type(const TypeBuilder<T>& builder) :
			_typeName(builder._typeName), 
			_rawTypeName(builder._rawTypeName), 
			_hash(builder._hash), 
			_super(builder._super)
		{

		}

	} // namespace refl

} // namespace flt
