#pragma once
#include <string_view>
#include <type_traits>
#include <vector>
#include "hash.h"

namespace flt
{

	namespace refl
	{
		class Type;
		class Method;
		class Property;

		//template<typename T>
		//struct TypeTester
		//{
		//	constexpr static bool hasSuper = requires { typename T::Super; } && !std::same_as<typename T::Super, void>;
		//	constexpr static bool hasType = requires { T::InitType(); };
		//};

		namespace impl
		{
			template <typename T>
			concept HasSuperImpl = requires { typename T::Super; };// && !std::same_as<typename T::Super, void>;

			template <typename T>
			concept ReflectTypeImpl = requires { T::InitType(); };
		}

		template<typename T>
		struct Tester
		{
			constexpr static inline bool hasSuper = impl::HasSuperImpl<T>;
			constexpr static inline bool hasType = impl::ReflectTypeImpl<T>;
		};

		template <typename T>
		concept HasSuper = requires { typename T::Super; } && !std::same_as<typename T::Super, void>;

		template <typename T>
		concept ReflectType = requires { T::InitType(); };

		template <typename T>
		concept ReflectTypePointer = std::is_pointer_v<T> && ReflectType<std::remove_pointer_t<T>>;

		template <typename T>
		concept NotReflectType = !ReflectType<T> && !ReflectType<std::remove_pointer_t<T>>;

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
					_super = T::Super::InitType();
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
#pragma region STATIC
		public:
			template <ReflectType T> //requires HasType<T>
			static constexpr Type* GetType()
			{
				return T::InitType();
			}

			template<ReflectTypePointer T> //requires std::is_pointer_v<T> && HasType<std::remove_pointer_t<T>>
			static constexpr Type* GetType()
			{
				return std::remove_pointer_t<T>::InitType();
			}

			template <typename T> requires (!ReflectType<T>) && (!ReflectType<std::remove_pointer_t<T>>)
				static constexpr Type* GetType()
			{
				static Type type{ TypeBuilder<T>{"unknown"} };
				return &type;
			}
#pragma endregion
		public:
			template <typename T>
			explicit Type(const TypeBuilder<T>& builder);

			void AddProperty(Property* property)
			{
				_properties.push_back(property);
			}

			void AddMethod(Method* method)
			{
				_methods.push_back(method);
			}

			bool operator==(const Type& other) const
			{
				return _hash == other._hash;
			}

			Property* GetProperty(std::string_view name) const;
			std::vector<Property*> GetProperties() const;

			Method* GetMethod(std::string_view name) const;
			std::vector<Method*> GetMethods() const;


		private:
			std::vector<Method*> _methods;
			std::vector<Property*> _properties;

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
			if constexpr (HasSuper<T>)
			{
				// TODO : Add super methods and properties
				for (auto& method : _super->_methods)
				{
					_methods.push_back(method);
				}

				for (auto& property : _super->_properties)
				{
					_properties.push_back(property);
				}
			}
		}

	} // namespace refl

} // namespace flt
