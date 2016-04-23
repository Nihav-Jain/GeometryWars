#pragma once
#include <cassert>
#include "pch.h"
#include "Graph.h"

namespace Library
{
	class RTTI
	{
	public:
		virtual ~RTTI() = default;
		
		virtual std::uint64_t TypeIdInstance() const = 0;
		
		virtual RTTI* QueryInterface(const std::uint64_t id) const
		{
			UNREFERENCED_PARAMETER(id);
			return nullptr;
		}

		virtual bool Is(std::uint64_t id) const
		{
			UNREFERENCED_PARAMETER(id);
			return false;
		}

		virtual bool Is(const std::string& name) const
		{
			UNREFERENCED_PARAMETER(name);
			return false;
		}

		template <typename T>
		T* As() const
		{
			if (Is(T::TypeIdClass()))
			{
				return (T*)this;
			}

			return nullptr;
		}

		template <typename T>
		T* AssertiveAs() const
		{
			assert(Is(T::TypeIdClass()));
			return (T*)this;
		}

		virtual std::string ToString() const
		{
			return "RTTI";
		}

		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}

		virtual std::uint64_t ParentTypeId() const = 0;

		static std::uint64_t TypeIdClass() { return 0; }
	private:
		static Graph<std::uint64_t>::Traversor RootTraversor;
		static Graph<std::uint64_t> ClassHeirarchy;
	};

#define RTTI_DECLARATIONS(Type, ParentType)																	 \
		public:                                                                                              \
			typedef ParentType Parent;                                                                       \
			static std::string TypeName() { return std::string(#Type); }                                     \
			static std::uint64_t TypeIdClass() { return sRunTimeTypeId; }                                    \
			virtual std::uint64_t TypeIdInstance() const override { return Type::TypeIdClass(); }            \
			virtual Library::RTTI* QueryInterface(const std::uint64_t id) const override                     \
            {                                                                                                \
                if (id == sRunTimeTypeId)                                                                    \
					{ return (RTTI*)this; }                                                                  \
                else                                                                                         \
					{ return Parent::QueryInterface(id); }                                                   \
            }                                                                                                \
			virtual bool Is(std::uint64_t id) const override                                                 \
			{                                                                                                \
				if (id == sRunTimeTypeId)                                                                    \
					{ return true; }                                                                         \
				else                                                                                         \
					{ return Parent::Is(id); }                                                               \
			}                                                                                                \
			virtual bool Is(const std::string& name) const override                                          \
			{                                                                                                \
				if (name == TypeName())                                                                      \
					{ return true; }                                                                         \
				else                                                                                         \
					{ return Parent::Is(name); }                                                             \
			}                                                                                                \
			virtual std::uint64_t ParentTypeId() const override												 \
			{																								 \
				return Parent::TypeIdClass();																 \
			}																								 \
			private:                                                                                         \
				static Library::Graph<std::uint64_t>::Traversor TypeTraversor;								 \
				static std::uint64_t sRunTimeTypeId;

#define RTTI_DEFINITIONS(Type)																				 \
	std::uint64_t Type::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&Type::sRunTimeTypeId);			 \
	typename Graph<std::uint64_t>::Traversor Type::TypeTraversor = RTTI::;
	
#define RTTI_DEFINITIONS_T(Type)																			 \
	template <typename T>																					 \
	std::uint64_t Type::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&Type::sRunTimeTypeId);			 \
	template <typename T>																					 \
	typename Graph<std::uint64_t>::Traversor Type::TypeTraversor;
}