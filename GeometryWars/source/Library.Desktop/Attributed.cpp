#include "pch.h"
#include "Attributed.h"

namespace Library
{
	RTTI_DEFINITIONS(Attributed, Scope);

	Hashmap<std::uint64_t, Vector<std::string>> Attributed::mPrescribedAttributes;

	Attributed::Attributed()
	{
		AddInternalAttribute("this", this, 1);
	}

	Attributed::Attributed(const Attributed& rhs) :
		Scope::Scope(rhs)
	{
		(*this)["this"] = this;
	}

	Attributed::Attributed(Attributed&& rhs) :
		Scope::Scope(std::move(rhs))
	{
		(*this)["this"] = this;
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			Scope::operator=(rhs);

			(*this)["this"] = this;
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			Scope::operator=(std::move(rhs));

			(*this)["this"] = this;
		}
		return *this;
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		Vector<std::string>& prescribedAttributes = mPrescribedAttributes[TypeIdInstance()];
		if (prescribedAttributes.Find(name) == prescribedAttributes.end())
		{
			return IsParentPrescribedAttribute(name, RTTI::ClassHeirarchy()[TypeIdInstance()]);
		}
		return true;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return (Find(name) != nullptr);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
			throw std::exception("Attribute already exists");
		return Append(name);
	}

	Scope& Attributed::AppendAuxiliaryScope(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
			throw std::exception("Attribute already exists");
		return AppendScope(name);
	}

	std::uint32_t Attributed::AuxiliaryBegin() const
	{
		return mPrescribedAttributes[TypeIdInstance()].Size() + PrescribedAttributeCount(RTTI::ClassHeirarchy()[TypeIdInstance()]);
	}

	void Attributed::CopyAuxiliaryAttributesFromAnotherAttributed(const Attributed& anotherAttributed)
	{
		std::uint32_t i;
		for (i = anotherAttributed.AuxiliaryBegin(); i < anotherAttributed.Size(); i++)
		{
			const SymbolPair& pair = anotherAttributed.GetPair(i);
			Remove(pair.first);
			if (pair.second.Type() == Datum::DatumType::TABLE)
			{
				const Datum& nestedScopeDatum = pair.second;
				std::uint32_t j;
				for (j = 0; j < nestedScopeDatum.Size(); j++)
				{
					Scope* nestedScopeAttribute = new Scope(nestedScopeDatum.Get<Scope>(j));
					Adopt(pair.first, *nestedScopeAttribute);
				}
			}
			else
			{
				AppendAuxiliaryAttribute(pair.first) = pair.second;
			}
		}
	}

	void Attributed::Clear()
	{
		Scope::Clear();
		mPrescribedAttributes.Clear();
	}

	void Attributed::ClearStaticMembers()
	{
		mPrescribedAttributes.Clear();
	}

#pragma region AddInternalAttribute

	void Attributed::AddInternalAttribute(const std::string& name, std::int32_t initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::INTEGER, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}
	}

	void Attributed::AddInternalAttribute(const std::string& name, std::float_t initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::FLOAT, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}
	}

	void Attributed::AddInternalAttribute(const std::string& name, const std::string& initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::STRING, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}
	}

	void Attributed::AddInternalAttribute(const std::string& name, const char* initialValue, std::uint32_t size)
	{
		AddInternalAttribute(name, std::string(initialValue), size);
	}

	void Attributed::AddInternalAttribute(const std::string& name, const glm::vec4& initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::VECTOR4, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}
	}

	void Attributed::AddInternalAttribute(const std::string& name, const glm::mat4& initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::MATRIX4x4, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}
	}

	void Attributed::AddInternalAttribute(const std::string& name, bool initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::BOOLEAN, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}
	}

	void Attributed::AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::POINTER, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}

	}

	void Attributed::AddInternalAttribute(const std::string& name, Datum* initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::REFERENCE, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}
	}

	void Attributed::AddNestedScope(const std::string& name)
	{
		AddEmptyInternalSignature(name, Datum::DatumType::TABLE, 0);
	}

	void Attributed::AddNestedScope(const std::string& name, Scope& initialValue, std::uint32_t size)
	{
		AddEmptyInternalSignature(name, Datum::DatumType::TABLE, 0);
		for (std::uint32_t j = 0; j < size; j++)
		{
			Scope* scope = new Scope(initialValue);
			Adopt(name, *scope);
		}
	}

#pragma endregion

#pragma region AddExternalAttribute

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, std::int32_t* storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, std::float_t* storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, std::string* storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, glm::vec4* storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, glm::mat4* storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string & name, std::uint32_t size, bool* storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, RTTI** storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, Datum** storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

#pragma endregion

#pragma region Helper Functions

	void Attributed::DefineUniqueAttributeName(const std::string& name)
	{
		if (!IsPrescribedAttribute(name))
		{
			//std::stringstream str;
			//str << "Attribute " << name << " already exists.";
			//throw std::exception(str.str().c_str());
			mPrescribedAttributes[this->TypeIdInstance()].PushBack(name);
		}
	}

	Datum& Attributed::AddEmptyInternalSignature(const std::string& name, Datum::DatumType type, std::uint32_t size)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetType(type);
		datum.SetSize(size);
		return datum;
	}

	Datum& Attributed::AddEmptySignature(const std::string& name)
	{
		DefineUniqueAttributeName(name);
		return Append(name);
	}

	bool Attributed::IsParentPrescribedAttribute(const std::string& attributeName, const std::uint64_t* parentTypeIdPtr) const
	{
		if (*parentTypeIdPtr == Scope::TypeIdClass())
			return false;
		Vector<std::string>& prescribedAttributes = mPrescribedAttributes[*parentTypeIdPtr];
		if (prescribedAttributes.Find(attributeName) == prescribedAttributes.end())
			return IsParentPrescribedAttribute(attributeName, RTTI::ClassHeirarchy()[*parentTypeIdPtr]);
		return true;
	}

	std::uint32_t Attributed::PrescribedAttributeCount(const std::uint64_t* parentTypeIdPtr) const
	{
		if (*parentTypeIdPtr == Scope::TypeIdClass())
			return 0;
		return mPrescribedAttributes[*parentTypeIdPtr].Size() + PrescribedAttributeCount(RTTI::ClassHeirarchy()[*parentTypeIdPtr]);
	}

#pragma endregion

}
