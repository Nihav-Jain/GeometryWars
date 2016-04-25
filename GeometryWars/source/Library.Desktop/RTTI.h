#pragma once
#include <cassert>
#include "pch.h"
#include "Graph.h"

namespace Library
{
	struct RTTIData
	{
	public:
		RTTIData() :
			ChildTypeID(nullptr), ParentTypeID(nullptr)
		{}
		RTTIData(const std::uint64_t* childId, const std::uint64_t* parentId) :
			ChildTypeID(childId), ParentTypeID(parentId)
		{}
		const std::uint64_t* ChildTypeID;
		const std::uint64_t* ParentTypeID;
	};

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

		static std::uint64_t TypeIdClass() { return 0; }
		
		static Hashmap<std::string, std::string>& ClassHeirarchy();
	protected:
		static const std::uint64_t sRunTimeTypeId;
	private:
		class RTTIClassHeirarchyDeleter
		{
		public:
			RTTIClassHeirarchyDeleter() = default;
			~RTTIClassHeirarchyDeleter();
		};
		static Hashmap<std::string, std::string>* ClassHeirarchyPtr;
		static const RTTIClassHeirarchyDeleter sDeleter;
	};

#define RTTI_DECLARATIONS(Type, ParentType)																	 \
		protected:																							 \
			static const std::uint64_t sRunTimeTypeId;														 \
			static const std::uint64_t* ParentTypeId;														 \
			static Library::Hashmap<std::string, std::string>::Iterator ParentItr;										 \
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
			}

#define RTTI_DEFINITIONS(Type, ParentType)																	 \
	const std::uint64_t Type::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&Type::sRunTimeTypeId);		 \
	const std::uint64_t* Type::ParentTypeId = &ParentType::sRunTimeTypeId;									 \
	Hashmap<std::string, std::string>::Iterator Type::ParentItr = RTTI::ClassHeirarchy().Insert(#Type, #ParentType);
	
#define RTTI_DEFINITIONS_T(Type, ParentType)																 \
	template <typename T>																					 \
	const std::uint64_t Type::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&Type::sRunTimeTypeId);		 \
	template <typename T>																					 \
	const std::uint64_t* Type::ParentTypeId = &ParentType::sRunTimeTypeId;									 \
	template <typename T>																					 \
	Hashmap<std::string, std::string>::Iterator Type::ParentItr = RTTI::ClassHeirarchy().Insert(#Type, #ParentType);
}