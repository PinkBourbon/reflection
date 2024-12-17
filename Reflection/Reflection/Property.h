#pragma once
//#include "Type.h"
#include "TypeMacro.h"

namespace flt
{
	namespace refl
	{
		struct PropertyHandlerBase
		{
			GENERATED_REFLECT(PropertyHandlerBase)
		public:
			virtual ~PropertyHandlerBase() = default;
		};

		template<typename T>
		struct IConstPropertyHandler : PropertyHandlerBase
		{
			GENERATED_REFLECT(IConstPropertyHandler)
		public:
			virtual ~IConstPropertyHandler() {}
		};

		template<typename T>
		struct IPropertyHandler : PropertyHandlerBase
		{
			GENERATED_REFLECT(IPropertyHandler)
		public:
			virtual ~IPropertyHandler() {}

			virtual T* GetPtr(void* owner) = 0;
			virtual void Set(void* owner, T value) = 0;
		};

		template<typename TOwner, typename T>
		struct PropertyHandler : IPropertyHandler<T>
		{
			GENERATED_REFLECT(PropertyHandler)
		public:
			PropertyHandler(T TOwner::* ptr) : ptr(ptr) {}
			virtual ~PropertyHandler() {}

			T* GetPtr(void* owner) override
			{
				return &(((TOwner*)owner)->*ptr);
			}

			void Set(void* owner, T value) override
			{
				((TOwner*)owner)->*ptr = value;
			}

		public:
			T TOwner::* ptr;
		};

		struct PropertyBuilder
		{
			//constexpr PropertyBuilder() = default;
			std::string_view name;
			Type* type;
			PropertyHandlerBase* handler;
		};

		class Property
		{
			friend class flt::refl::Type;
		public:
			Property(Type* owner, const PropertyBuilder& builder) :
				name(builder.name), type(builder.type), handler(builder.handler)
			{
				owner->AddProperty(this);
			}

			template<typename T>
			T* GetPtr(void* owner)
			{
				if (type != Type::GetType<T>())
				{
					return nullptr;
				}

				return static_cast<IPropertyHandler<T>*>(handler)->GetPtr(owner);
			}

			template<typename T>
			bool Set(void* owner, T value)
			{
				if (type != Type::GetType<T>())
				{
					return false;
				}

				static_cast<IPropertyHandler<T>*>(handler)->Set(owner, value);
				return true;
			}

			std::string_view Name() const
			{
				return name;
			}

		private:
			std::string_view name;
			Type* type;
			PropertyHandlerBase* handler;
		};

		template<typename ClassType, typename PropertyType, typename MemPtr, MemPtr ptr>
		class PropertyRegister
		{
		public:
			PropertyRegister(std::string_view name, Type* classType)
			{
				static PropertyBuilder builder{ .name{name}, .type{Type::GetType<PropertyType>()}, .handler{new PropertyHandler(ptr)} };
				static Property property(classType, builder);
			}

		private:
		};
	} // namespace refl
} // namespace flt
