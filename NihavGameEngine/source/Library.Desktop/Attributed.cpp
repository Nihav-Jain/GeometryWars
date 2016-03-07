#include "pch.h"
#include "Attributed.h"

namespace Library
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed()
	{}

	Attributed::Attributed(const Attributed& rhs) :
		Scope::Scope(rhs), mSignatures(rhs.mSignatures), mPrescribedAttributes(rhs.mPrescribedAttributes)
	{
		(*this)["this"] = this;
	}

	Attributed::Attributed(Attributed&& rhs) :
		Scope::Scope(std::move(rhs)), mSignatures(std::move(rhs.mSignatures)), mPrescribedAttributes(std::move(rhs.mPrescribedAttributes))
	{
		(*this)["this"] = this;
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			Scope::operator=(rhs);

			mSignatures = rhs.mSignatures;
			mPrescribedAttributes = rhs.mPrescribedAttributes;

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
			mSignatures = std::move(rhs.mSignatures);
			mPrescribedAttributes = std::move(rhs.mPrescribedAttributes);

			(*this)["this"] = this;
		}
		return *this;
	}

	void Attributed::Populate()
	{
		DefinePrescribedAttributes();

		if (mPrescribedAttributes.Size() == 0)
		{
			Datum& thisDatum = Append("this");
			thisDatum = this;
			mPrescribedAttributes.PushBack("this");
		}

		// for each GetSignature, create a Datum and add to cache
		for (std::uint32_t i = mPrescribedAttributes.Size() - 1; i < mSignatures.Size();i++)
		{
			Signature& signature = mSignatures[i];
			mPrescribedAttributes.PushBack(signature.Name());
			if (signature.SignatureDatum().Type() == Datum::DatumType::TABLE)
			{
				for (std::uint32_t j = 0; j < signature.SignatureDatum().Size(); j++)
				{
					Adopt(signature.Name(), *signature.SignatureDatum().Get<Scope*>(j));
				}
			}
			else
			{
				Datum& attribute = Append(signature.Name());
				attribute = std::move(signature.SignatureDatum());
			}
		}
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		return (mPrescribedAttributes.Find(name) != mPrescribedAttributes.end());
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

	std::uint32_t Attributed::AuxiliaryBegin() const
	{
		return mPrescribedAttributes.Size();
	}

	void Attributed::Clear()
	{
		Scope::Clear();
		mSignatures.Clear();
		mPrescribedAttributes.Clear();
	}

#pragma region Signature

	Attributed::Signature::Signature(const std::string& name, Datum& datum) :
		mName(name), mSignatureDatum(datum)
	{}

	Attributed::Signature::Signature(Signature&& rhs) :
		mName(std::move(rhs.mName)), mSignatureDatum(std::move(rhs.mSignatureDatum))
	{}

	Attributed::Signature& Attributed::Signature::operator=(Signature&& rhs)
	{
		if (this != &rhs)
		{
			mName = std::move(rhs.mName);
			mSignatureDatum = std::move(rhs.mSignatureDatum);
		}
		return *this;
	}

	std::string& Attributed::Signature::Name()
	{
		return mName;
	}

	Datum& Attributed::Signature::SignatureDatum()
	{
		return mSignatureDatum;
	}

#pragma endregion

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

	void Attributed::AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::POINTER, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
		}

	}

	void Attributed::AddNestedScope(const std::string& name, Scope& initialValue, std::uint32_t size)
	{
		Datum& datum = AddEmptyInternalSignature(name, Datum::DatumType::TABLE, size);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.Set(initialValue, i);
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

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, RTTI** storage)
	{
		Datum& datum = AddEmptySignature(name);
		datum.SetStorage(storage, size);
	}

#pragma endregion

#pragma region Helper Functions

	bool Attributed::IsSignatureNameDefined(const std::string& name) const
	{
		for (auto& signature : mSignatures)
		{
			if (signature.Name() == name)
				return true;
		}
		return false;
	}

	Datum& Attributed::AddEmptyInternalSignature(const std::string& name, Datum::DatumType type, std::uint32_t size)
	{
		if (IsSignatureNameDefined(name))
			throw std::exception("Signature with given name already exists.");

		Vector<Signature>::Iterator itr = mSignatures.PushBack(Signature(name, Datum()));
		Datum& datum = (*itr).SignatureDatum();
		datum.SetType(type);
		datum.SetSize(size);
		return datum;
	}

	Datum& Attributed::AddEmptySignature(const std::string& name)
	{
		if (IsSignatureNameDefined(name))
			throw std::exception("Signature with given name already exists.");

		Vector<Signature>::Iterator itr = mSignatures.PushBack(Signature(name, Datum()));
		return (*itr).SignatureDatum();
	}

#pragma endregion

}
