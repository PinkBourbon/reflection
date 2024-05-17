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
			virtual ~PropertyHandlerBase() = default;
		};

		template<typename T>
		struct IPropertyHandler : PropertyHandlerBase
		{
			GENERATED_REFLECT(IPropertyHandler)
			virtual T Get() = 0;
			virtual void Set(T value) = 0;
		};

		template<typename T>
		struct PropertyHandler : IPropertyHandler<T>
		{
			GENERATED_REFLECT(PropertyHandler)
			T Get() override
			{
				return T();
			}

			void Set(T value) override
			{
			}
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
		public:
			Property(Type* owner, PropertyBuilder& builder) : 
				name(builder.name), type(builder.type), handler(builder.handler)
			{
				owner->AddProperty(this);
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
				static PropertyBuilder builder{ .name{name}, .type{Type::GetType<PropertyType>()} };
				static Property property(classType, builder);
			}

		private:
		};
	} // namespace refl
} // namespace flt
