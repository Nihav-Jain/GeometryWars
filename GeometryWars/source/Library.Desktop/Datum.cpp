#include "pch.h"
#include "Datum.h"
#include "glm\gtx\string_cast.hpp"
#include "Scope.h"

namespace Library
{
	Hashmap<Datum::DatumType, Datum::AddDatums, Datum::DatumTypeHash> Datum::mAddDatums = {
		{ Datum::DatumType::INTEGER, &Datum::Add<DatumType::INTEGER> },
		{ Datum::DatumType::FLOAT, &Datum::Add<DatumType::FLOAT> },
		{ Datum::DatumType::STRING, &Datum::Add<DatumType::STRING> },
		{ Datum::DatumType::VECTOR4, &Datum::Add<DatumType::VECTOR4> },
		{ Datum::DatumType::MATRIX4x4, &Datum::Add<DatumType::MATRIX4x4> }
	};

	Hashmap<Datum::DatumType, Datum::SubtractDatums, Datum::DatumTypeHash> Datum::mSubtractDatums = {
		{ Datum::DatumType::INTEGER, &Datum::Subtract<DatumType::INTEGER> },
		{ Datum::DatumType::FLOAT, &Datum::Subtract<DatumType::FLOAT> },
		{ Datum::DatumType::VECTOR4, &Datum::Subtract<DatumType::VECTOR4> },
		{ Datum::DatumType::MATRIX4x4, &Datum::Subtract<DatumType::MATRIX4x4> }
	};

	Hashmap<Datum::DatumType, Datum::MultiplyDatums, Datum::DatumTypeHash> Datum::mMultiplyDatums = {
		{ Datum::DatumType::INTEGER, &Datum::Multiply<DatumType::INTEGER> },
		{ Datum::DatumType::FLOAT, &Datum::Multiply<DatumType::FLOAT> },
		{ Datum::DatumType::VECTOR4, &Datum::Multiply<DatumType::VECTOR4> },
		{ Datum::DatumType::MATRIX4x4, &Datum::Multiply<DatumType::MATRIX4x4> }
	};

	Hashmap<Datum::DatumType, Datum::DivideDatums, Datum::DatumTypeHash> Datum::mDivideDatums = {
		{ Datum::DatumType::INTEGER, &Datum::Divide<DatumType::INTEGER> },
		{ Datum::DatumType::FLOAT, &Datum::Divide<DatumType::FLOAT> },
		{ Datum::DatumType::VECTOR4, &Datum::Divide<DatumType::VECTOR4> },
		{ Datum::DatumType::MATRIX4x4, &Datum::Divide<DatumType::MATRIX4x4> }
	};

	Hashmap<Datum::DatumType, Datum::LessThanDatums, Datum::DatumTypeHash> Datum::mLessThanDatums = {
		{ Datum::DatumType::INTEGER, &Datum::LessThan<DatumType::INTEGER> },
		{ Datum::DatumType::FLOAT, &Datum::LessThan<DatumType::FLOAT> },
		{ Datum::DatumType::STRING, &Datum::LessThan<DatumType::STRING> }
	};

	const Hashmap<Datum::DatumType, std::string> Datum::DatumTypeToString = {
		{ Datum::DatumType::INTEGER,	"INTEGER" },
		{ Datum::DatumType::FLOAT,		"FLOAT" },
		{ Datum::DatumType::STRING,		"STRING" },
		{ Datum::DatumType::VECTOR4,	"VECTOR4" },
		{ Datum::DatumType::MATRIX4x4,	"MATRIX4x4" },
		{ Datum::DatumType::BOOLEAN,	"BOOLEAN" },
		{ Datum::DatumType::TABLE,		"TABLE" },
		{ Datum::DatumType::POINTER,	"POINTER" },
		{ Datum::DatumType::REFERENCE,	"REFERENCE" }
	};

	Datum::Datum() :
		mType(DatumType::UNKNOWN), mSize(0), mCapacity(0), mStorageType(DatumStorageType::UNKNOWN)
	{
	}

	Datum::Datum(const Datum& rhs) :
		Datum()
	{
		operator=(rhs);
	}

	Datum::Datum(Datum&& rhs) : 
		mType(rhs.mType), mSize(rhs.mSize), mCapacity(rhs.mCapacity), mStorageType(rhs.mStorageType), mData(std::move(rhs.mData))
	{
		rhs.mType = DatumType::UNKNOWN;
		rhs.mStorageType = DatumStorageType::UNKNOWN;
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mData.mInt = nullptr;
	}


	Datum::~Datum()
	{
		Clear();
	}

#pragma region Assignment operator

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			
			mType = rhs.mType;
			mStorageType = rhs.mStorageType;
			if (rhs.mStorageType == DatumStorageType::INTERNAL)
			{
				Reserve(rhs.mCapacity);
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					switch (mType)
					{
					case DatumType::INTEGER:
						Set(rhs.mData.mInt[i], i);
						break;
					
					case DatumType::FLOAT:
						Set(rhs.mData.mFloat[i], i);
						break;
					
					case DatumType::STRING:
						Set(rhs.mData.mString[i], i);
						break;
					
					case DatumType::VECTOR4:
						Set(rhs.mData.mVec4[i], i);
						break;
					
					case DatumType::MATRIX4x4:
						Set(rhs.mData.mMat4x4[i], i);
						break;
					
					case DatumType::BOOLEAN:
						Set(rhs.mData.mBool[i], i);
						break;

					case DatumType::POINTER:
						Set(rhs.mData.mRttiPtr[i], i);
						break;

					case DatumType::TABLE:
						Set(*rhs.mData.mScopePtr[i], i);
						break;

					case DatumType::REFERENCE:
						Set(rhs.mData.mDatumPtr[i], i);
						break;

					default:
						break;
					}
				}
			}
			else
			{
				mData = rhs.mData;
			}

			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;

		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mType = rhs.mType;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mStorageType = rhs.mStorageType;
			mData = std::move(rhs.mData);

			rhs.mType = DatumType::UNKNOWN;
			rhs.mStorageType = DatumStorageType::UNKNOWN;
			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mData.mInt = nullptr;
		}
		return *this;
	}

	Datum& Datum::operator=(const std::int32_t& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const std::float_t& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const char* rhs)
	{
		return operator=(std::string(rhs));
	}

	Datum& Datum::operator=(const glm::vec4& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4x4& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const bool& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(RTTI* rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(Scope& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(Datum* rhs)
	{
		assert(rhs != nullptr);
		Set(rhs);
		return *this;
	}


#pragma endregion

#pragma region operator+

	Datum Datum::operator+(const Datum& rhs) const
	{
		if (!(mAddDatums.ContainsKey(mType) && mAddDatums.ContainsKey(rhs.Type())))
			throw std::exception("cannot add Datums of given type");
		if(!(mSize > 0 && rhs.Size() > 0))
			throw std::exception("cannot add an empty datum");
		return (this->*mAddDatums[mType])(rhs);
	}

	Datum Datum::operator-(const Datum& rhs) const
	{
		if (!(mSubtractDatums.ContainsKey(mType) && mSubtractDatums.ContainsKey(rhs.Type())))
			throw std::exception("cannot add Datums of given type");
		if (!(mSize > 0 && rhs.Size() > 0))
			throw std::exception("cannot add an empty datum");
		return (this->*mSubtractDatums[mType])(rhs);
	}

	Datum Datum::operator*(const Datum& rhs) const
	{
		if (!(mMultiplyDatums.ContainsKey(mType) && mMultiplyDatums.ContainsKey(rhs.Type())))
			throw std::exception("cannot add Datums of given type");
		if (!(mSize > 0 && rhs.Size() > 0))
			throw std::exception("cannot add an empty datum");
		return (this->*mMultiplyDatums[mType])(rhs);
	}

	Datum Datum::operator/(const Datum& rhs) const
	{
		if (!(mDivideDatums.ContainsKey(mType) && mDivideDatums.ContainsKey(rhs.Type())))
			throw std::exception("cannot add Datums of given type");
		if (!(mSize > 0 && rhs.Size() > 0))
			throw std::exception("cannot add an empty datum");
		return (this->*mDivideDatums[mType])(rhs);
	}

	Datum Datum::operator<(const Datum& rhs) const
	{
		if (!(mLessThanDatums.ContainsKey(mType) && mLessThanDatums.ContainsKey(rhs.Type())))
			throw std::exception("cannot add Datums of given type");
		if (!(mSize > 0 && rhs.Size() > 0))
			throw std::exception("cannot add an empty datum");
		return (this->*mLessThanDatums[mType])(rhs);
	}

	Datum Datum::operator>(const Datum& rhs) const
	{
		Datum result;
		result = operator==(rhs);
		result = (operator<(rhs) || result);
		result = !result.Get<bool>();
		return result;
	}

	Datum Datum::operator<=(const Datum& rhs) const
	{
		Datum result;
		result = operator==(rhs);
		result = (operator<(rhs) || result);
		return result;
	}

	Datum Datum::operator>=(const Datum& rhs) const
	{
		Datum result;
		result = operator==(rhs);
		result = (operator>(rhs) || result);
		return result;
	}

	Datum Datum::operator||(const Datum& rhs) const
	{
		if (!(mType == DatumType::BOOLEAN && rhs.Type() == DatumType::BOOLEAN))
			throw std::exception("cannot compare Datums of given type");
		if (!(mSize > 0 && rhs.Size() > 0))
			throw std::exception("cannot add an empty datum");
		Datum result;
		result = (Get<bool>() || rhs.Get<bool>(0));
		return result;
	}

	Datum Datum::operator&&(const Datum& rhs) const
	{
		if (!(mType == DatumType::BOOLEAN && rhs.Type() == DatumType::BOOLEAN))
			throw std::exception("cannot compare Datums of given type");
		if (!(mSize > 0 && rhs.Size() > 0))
			throw std::exception("cannot add an empty datum");
		Datum result;
		result = (Get<bool>() && rhs.Get<bool>(0));
		return result;
	}

#pragma endregion

	Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	void Datum::SetType(DatumType type)
	{
		if (!(mType == DatumType::UNKNOWN || mType == type))
		{
			throw std::exception("Cannot change type of Datum");
		}
		mType = type;
	
		if(mStorageType == DatumStorageType::UNKNOWN)
			mStorageType = DatumStorageType::INTERNAL;
	}

	Datum::DatumStorageType Datum::StorageType() const
	{
		return mStorageType;
	}

	std::uint32_t Datum::Size() const
	{
		return mSize;
	}

	std::uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	void Datum::SetSize(std::uint32_t newSize)
	{
		if (mType == DatumType::UNKNOWN)
		{
			throw std::exception("ClearScreen operation. Datum type not set.");
		}
		if (mStorageType == DatumStorageType::INTERNAL)
		{
			if (newSize > mCapacity)
			{
				Reserve(newSize);
			}
			while (mSize < newSize)
			{
				switch (mType)
				{
				case DatumType::INTEGER:
					new (mData.mInt + mSize)std::int32_t();
					break;

				case DatumType::FLOAT:
					new (mData.mFloat + mSize)std::float_t();
					break;

				case DatumType::STRING:
					new (mData.mString + mSize)std::string();
					break;

				case DatumType::VECTOR4:
					new (mData.mVec4 + mSize)glm::vec4();
					break;

				case DatumType::MATRIX4x4:
					new (mData.mMat4x4 + mSize)glm::mat4x4();
					break;

				case DatumType::BOOLEAN:
					new (mData.mBool + mSize)bool();
					break;

				case DatumType::POINTER:
					new (mData.mRttiPtr + mSize)RTTI*(nullptr);
					break;

				case DatumType::TABLE:
					new (mData.mScopePtr + mSize)Scope*(nullptr);
					break;

				case DatumType::REFERENCE:
					new (mData.mDatumPtr + mSize)Datum*(nullptr);
					break;

				default:
					throw std::exception("ClearScreen data type");
					break;
				}
				++mSize;
			}
			// shrink size if its less than the new size [dont shrink capacity]
			while (mSize > newSize)
			{
				--mSize;
				switch (mType)
				{
				case DatumType::INTEGER:
					break;

				case DatumType::FLOAT:
					break;

				case DatumType::STRING:
					mData.mString[mSize].std::string::~string();
					break;

				case DatumType::VECTOR4:
					mData.mVec4[mSize].glm::vec4::~vec4();
					break;

				case DatumType::MATRIX4x4:
					mData.mMat4x4[mSize].glm::mat4x4::~mat4x4();
					break;

				case DatumType::BOOLEAN:
					break;

				case DatumType::POINTER:
					break;

				case DatumType::TABLE:
					break;

				case DatumType::REFERENCE:
					break;

				default:
					throw std::exception("ClearScreen data type");
					break;
				}
			}
		}
		else
		{
			throw std::exception("Cannot modify external storage.");
		}
	}

	void Datum::Reserve(std::uint32_t newCapacity)
	{
		if (mStorageType == DatumStorageType::EXTERNAL)
		{
			throw std::exception("cannot reserve memory for Datum with external storage");
		}
		if (newCapacity > mCapacity)
		{
			switch (mType)
			{
				case DatumType::INTEGER:
					mData.mInt = reinterpret_cast<std::int32_t*>(realloc(mData.mInt, newCapacity * sizeof(std::int32_t)));
					break;

				case DatumType::FLOAT:
					mData.mFloat = reinterpret_cast<std::float_t*>(realloc(mData.mFloat, newCapacity * sizeof(std::float_t)));
					break;

				case DatumType::STRING:
					mData.mString = reinterpret_cast<std::string*>(realloc(mData.mString, newCapacity * sizeof(std::string)));
					break;

				case DatumType::VECTOR4:
					mData.mVec4 = reinterpret_cast<glm::vec4*>(realloc(mData.mVec4, newCapacity * sizeof(glm::vec4)));
					break;

				case DatumType::MATRIX4x4:
					mData.mMat4x4 = reinterpret_cast<glm::mat4x4*>(realloc(mData.mMat4x4, newCapacity * sizeof(glm::mat4x4)));
					break;

				case DatumType::BOOLEAN:
					mData.mBool = reinterpret_cast<bool*>(realloc(mData.mBool, newCapacity * sizeof(bool)));
					break;

				case DatumType::POINTER:
					mData.mRttiPtr = reinterpret_cast<RTTI**>(realloc(mData.mRttiPtr, newCapacity * sizeof(RTTI*)));
					break;

				case DatumType::TABLE:
					mData.mScopePtr = reinterpret_cast<Scope**>(realloc(mData.mScopePtr, newCapacity * sizeof(Scope*)));
					break;

				case DatumType::REFERENCE:
					mData.mDatumPtr = reinterpret_cast<Datum**>(realloc(mData.mDatumPtr, newCapacity * sizeof(Datum*)));
					break;

				default:
					throw std::exception("ClearScreen data type");
					break;
			}
			mCapacity = newCapacity;
		}
	}

	void Datum::Clear()
	{
		if (mStorageType == DatumStorageType::INTERNAL)
		{
			switch (mType)
			{
			case DatumType::INTEGER:
				free(mData.mInt);
				break;

			case DatumType::FLOAT:
				free(mData.mFloat);
				break;

			case DatumType::STRING:
				for (std::uint32_t i = 0; i < mSize; i++)
					mData.mString[i].std::string::~string();
				free(mData.mString);
				break;

			case DatumType::VECTOR4:
				for (std::uint32_t i = 0; i < mSize; i++)
					mData.mVec4[i].glm::vec4::~vec4();
				free(mData.mVec4);
				break;

			case DatumType::MATRIX4x4:
				for (std::uint32_t i = 0; i < mSize; i++)
					mData.mMat4x4[i].glm::mat4x4::~mat4x4();
				free(mData.mMat4x4);
				break;

			case DatumType::BOOLEAN:
				free(mData.mBool);
				break;

			case DatumType::POINTER:
				free(mData.mRttiPtr);
				break;

			case DatumType::TABLE:
				free(mData.mScopePtr);
				break;

			case DatumType::REFERENCE:
				free(mData.mDatumPtr);
				break;

			default:
				break;
			}
		}

		mData.mInt = nullptr;
		mType = DatumType::UNKNOWN;
		mStorageType = DatumStorageType::UNKNOWN;
		mSize = 0;
		mCapacity = 0;
	}

#pragma region SetStorage

	void Datum::SetStorage(DatumType typeToSet, std::uint32_t arraySize)
	{
		if (!(mType == DatumType::UNKNOWN || mType == typeToSet))
			throw std::exception("ClearScreen operation. Cannot change type of Datum.");

		if (mStorageType == DatumStorageType::INTERNAL)
		{
			Clear();
		}

		mType = typeToSet;
		mStorageType = DatumStorageType::EXTERNAL;
		mSize = arraySize;
		mCapacity = arraySize;
	}

	void Datum::SetStorage(std::int32_t* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::INTEGER, arraySize);
		mData.mInt = dataArray;
	}

	void Datum::SetStorage(std::float_t* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::FLOAT, arraySize);
		mData.mFloat = dataArray;
	}

	void Datum::SetStorage(std::string* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::STRING, arraySize);
		mData.mString = dataArray;
	}

	void Datum::SetStorage(glm::vec4* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::VECTOR4, arraySize);
		mData.mVec4 = dataArray;
	}

	void Datum::SetStorage(glm::mat4x4* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::MATRIX4x4, arraySize);
		mData.mMat4x4 = dataArray;
	}

	void Datum::SetStorage(bool* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::BOOLEAN, arraySize);
		mData.mBool = dataArray;
	}

	void Datum::SetStorage(RTTI** dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::POINTER, arraySize);
		mData.mRttiPtr = dataArray;
	}

	void Datum::SetStorage(Datum** dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::REFERENCE, arraySize);
		mData.mDatumPtr = dataArray;
	}

#pragma endregion

#pragma region Equality operators

	bool Datum::operator==(const Datum& rhs) const
	{
		bool areEqual = false;
		if (mType == rhs.mType && mSize == rhs.mSize)
		{
			areEqual = true;
			for (std::uint32_t i = 0; i < mSize; i++)
			{
				switch (mType)
				{
				case DatumType::INTEGER:
					if (mData.mInt[i] != rhs.mData.mInt[i])
					{
						areEqual = false;
						break;
					}
					break;
				
				case DatumType::FLOAT:
					if (mData.mFloat[i] != rhs.mData.mFloat[i])
					{
						areEqual = false;
						break;
					}
					break;
				
				case DatumType::STRING:
					if (mData.mString[i].compare(rhs.mData.mString[i]) != 0)
					{
						areEqual = false;
						break;
					}
					break;

				case DatumType::VECTOR4:
					if (mData.mVec4[i] != rhs.mData.mVec4[i])
					{
						areEqual = false;
						break;
					}
					break;
				case DatumType::MATRIX4x4:
					if (mData.mMat4x4[i] != rhs.mData.mMat4x4[i])
					{
						areEqual = false;
						break;
					}
					break;
				case DatumType::BOOLEAN:
					if (mData.mBool[i] != rhs.mData.mBool[i])
					{
						areEqual = false;
						break;
					}
					break;
				case DatumType::POINTER:
					if (mData.mRttiPtr[i] != rhs.mData.mRttiPtr[i])
					{
						areEqual = false;
						break;
					}
					break;

				case DatumType::TABLE:
					if (*mData.mScopePtr[i] != *rhs.mData.mScopePtr[i])
					{
						areEqual = false;
						break;
					}
					break;

				case DatumType::REFERENCE:
					if (mData.mDatumPtr[i] != rhs.mData.mDatumPtr[i])
					{
						areEqual = false;
						break;
					}
					break;

				default:
					throw std::exception("ClearScreen data type");
					break;
				}
			}

		}
		return areEqual;
	}

	bool Datum::operator==(const std::int32_t& rhs) const
	{
		return (mType == DatumType::INTEGER && mSize > 0 && *mData.mInt == rhs);
	}

	bool Datum::operator==(const std::float_t& rhs) const
	{
		return (mType == DatumType::FLOAT && mSize > 0 && *mData.mFloat == rhs);
	}

	bool Datum::operator==(const std::string& rhs) const
	{
		return (mType == DatumType::STRING && mSize > 0 && rhs.compare(*mData.mString) == 0);
	}

	bool Datum::operator==(const char* rhs) const
	{
		return operator==(std::string(rhs));
	}

	bool Datum::operator==(const glm::vec4& rhs) const
	{
		return (mType == DatumType::VECTOR4 && mSize > 0 && *mData.mVec4 == rhs);
	}

	bool Datum::operator==(const glm::mat4x4& rhs) const
	{
		return (mType == DatumType::MATRIX4x4 && mSize > 0 && *mData.mMat4x4 == rhs);
	}

	bool Datum::operator==(const bool& rhs) const
	{
		return (mType == DatumType::BOOLEAN && mSize > 0 && *mData.mBool == rhs);
	}

	bool Datum::operator==(const RTTI* rhs) const
	{
		return (mType == DatumType::POINTER && mSize > 0 && (**mData.mRttiPtr).Equals(rhs));
	}

	bool Datum::operator==(const Scope* rhs) const
	{
		return (mType == DatumType::TABLE && mSize > 0 && (**mData.mScopePtr) == *rhs);
	}

	bool Datum::operator==(const Datum* rhs) const
	{
		return (mType == DatumType::REFERENCE && mSize > 0 && (**mData.mDatumPtr) == *rhs);
	}


	bool Datum::operator!=(const Datum& rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator!=(const std::int32_t& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const std::float_t& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const std::string& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const char* rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::vec4& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::mat4x4& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const bool& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const RTTI* rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const Scope* rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const Datum* rhs) const
	{
		return !(operator==(rhs));
	}

#pragma endregion

#pragma region Set

	void Datum::Set(DatumType typeToSet, std::uint32_t index)
	{
		if (!(mType == DatumType::UNKNOWN || mType == typeToSet))
			throw std::exception("Type mismatch exception.");

		if (mType == DatumType::UNKNOWN)
		{
			mType = typeToSet;
			mStorageType = DatumStorageType::INTERNAL;
		}
		if (index >= mSize)
		{
			if (mStorageType == DatumStorageType::EXTERNAL)
				throw std::exception("Index out of bounds for datum with external storage");
			SetSize(index + 1);
		}
	}

	void Datum::Set(std::int32_t value, std::uint32_t index)
	{
		Set(DatumType::INTEGER, index);
		mData.mInt[index] = value;
	}

	void Datum::Set(std::float_t value, std::uint32_t index)
	{
		Set(DatumType::FLOAT, index);
		mData.mFloat[index] = value;
	}

	void Datum::Set(const std::string& value, std::uint32_t index)
	{
		Set(DatumType::STRING, index);
		mData.mString[index] = value;
	}

	void Datum::Set(const char* value, std::uint32_t index)
	{
		Set(std::string(value), index);
	}

	void Datum::Set(const glm::vec4& value, std::uint32_t index)
	{
		Set(DatumType::VECTOR4, index);
		mData.mVec4[index] = value;
	}

	void Datum::Set(const glm::mat4x4& value, std::uint32_t index)
	{
		Set(DatumType::MATRIX4x4, index);
		mData.mMat4x4[index] = value;
	}

	void Datum::Set(bool value, std::uint32_t index)
	{
		Set(DatumType::BOOLEAN, index);
		mData.mBool[index] = value;
	}

	void Datum::Set(RTTI* value, std::uint32_t index)
	{
		Set(DatumType::POINTER, index);
		mData.mRttiPtr[index] = value;
	}

	void Datum::Set(Scope& value, std::uint32_t index)
	{
		Set(DatumType::TABLE, index);
		mData.mScopePtr[index] = &value;
	}

	void Datum::Set(Datum* value, std::uint32_t index)
	{
		assert(value != nullptr);
		Set(DatumType::REFERENCE, index);
		mData.mDatumPtr[index] = value;
	}

	void Datum::Remove(std::uint32_t index)
	{
		if (mStorageType == DatumStorageType::EXTERNAL)
			throw std::exception("Cannot modify Datum with external storage");

		if (index < mSize)
		{
			std::uint32_t sizeOfType = 0;
			std::uint32_t i = index;
			switch (mType)
			{
			case DatumType::INTEGER:
				sizeOfType = sizeof(std::int32_t);
				for (; i < mSize - 1; i++)
				{
					mData.mInt[i] = mData.mInt[i + 1];
				}
				break;
			case DatumType::FLOAT:
				sizeOfType = sizeof(std::float_t);
				for (; i < mSize - 1; i++)
				{
					mData.mFloat[i] = mData.mFloat[i + 1];
				}
				break;
			case DatumType::VECTOR4:
				mData.mVec4[index].glm::vec4::~vec4();
				sizeOfType = sizeof(glm::vec4);
				for (; i < mSize - 1; i++)
				{
					mData.mVec4[i] = mData.mVec4[i + 1];
				}
				break;
			case DatumType::MATRIX4x4:
				mData.mMat4x4[index].glm::mat4::~mat4();
				sizeOfType = sizeof(glm::mat4);
				for (; i < mSize - 1; i++)
				{
					mData.mMat4x4[i] = mData.mMat4x4[i + 1];
				}
				break;
			case DatumType::STRING:
				mData.mString[index].std::string::~string();
				sizeOfType = sizeof(std::string);
				for (; i < mSize - 1; i++)
				{
					mData.mString[i] = mData.mString[i + 1];
				}
				break;
			case DatumType::BOOLEAN:
				sizeOfType = sizeof(bool);
				for (; i < mSize - 1; i++)
				{
					mData.mBool[i] = mData.mBool[i + 1];
				}
				break;
			case DatumType::POINTER:
				sizeOfType = sizeof(RTTI*);
				for (; i < mSize - 1; i++)
				{
					mData.mRttiPtr[i] = mData.mRttiPtr[i + 1];
				}
				break;
			case DatumType::TABLE:
				sizeOfType = sizeof(Scope*);
				for (; i < mSize - 1; i++)
				{
					mData.mScopePtr[i] = mData.mScopePtr[i + 1];
				}
				break;
			case DatumType::REFERENCE:
				sizeOfType = sizeof(Datum*);
				for (; i < mSize - 1; i++)
				{
					mData.mDatumPtr[i] = mData.mDatumPtr[i + 1];
				}
				break;
			default:
				break;
			}

			//memmove_s(mData.mInt + index, (mSize - index - 1), mData.mInt + index + 1, sizeOfType * (mSize - index - 1));
			mSize--;
		}
	}

#pragma endregion

#pragma region Get

	template<>
	std::int32_t& Datum::Get<std::int32_t>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::INTEGER);
		OutOfBoundsCheck(index);
		return mData.mInt[index];
	}

	template<>
	std::float_t& Datum::Get<std::float_t>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::FLOAT);
		OutOfBoundsCheck(index);
		return mData.mFloat[index];
	}

	template<>
	std::string& Datum::Get<std::string>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::STRING);
		OutOfBoundsCheck(index);
		return mData.mString[index];
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::VECTOR4);
		OutOfBoundsCheck(index);
		return mData.mVec4[index];
	}

	template<>
	glm::mat4x4& Datum::Get<glm::mat4x4>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::MATRIX4x4);
		OutOfBoundsCheck(index);
		return mData.mMat4x4[index];
	}

	template<>
	bool& Datum::Get<bool>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::BOOLEAN);
		OutOfBoundsCheck(index);
		return mData.mBool[index];
	}

	template<>
	RTTI*& Datum::Get<RTTI*>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::POINTER);
		OutOfBoundsCheck(index);
		return mData.mRttiPtr[index];
	}

	template<>
	Scope& Datum::Get<Scope>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::TABLE);
		OutOfBoundsCheck(index);
		return *mData.mScopePtr[index];
	}

	template<>
	Datum& Datum::Get<Datum>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::REFERENCE);
		OutOfBoundsCheck(index);
		return *mData.mDatumPtr[index];
	}

#pragma endregion

	Scope& Datum::operator[](std::uint32_t index) const
	{
		return Get<Scope>(index);
	}

	void Datum::SetFromString(const std::string& inputString, std::uint32_t index)
	{
		if (mType == DatumType::UNKNOWN)
			throw std::exception("ClearScreen operation. Datum type not set.");
		glm::vec4 fvec;
		glm::mat4 mat;
		bool boolValue = false;
		switch (mType)
		{
			case DatumType::INTEGER:
				std::int32_t intValue;
				sscanf_s(inputString.c_str(), "%d", &intValue);
				Set(intValue, index);
				break;

			case DatumType::FLOAT:
				std::float_t floatValue;
				sscanf_s(inputString.c_str(), "%f", &floatValue);
				Set(floatValue, index);
				break;

			case DatumType::STRING:
				Set(inputString, index);
				break;

			case DatumType::VECTOR4:
				sscanf_s(inputString.c_str(), "vec4(%f, %f, %f, %f)", &fvec[0], &fvec[1], &fvec[2], &fvec[3]);
				Set(fvec, index);
				break;

			case DatumType::MATRIX4x4:
				sscanf_s(inputString.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", 
					&mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3],
					&mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
					&mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3],
					&mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]);
				Set(mat, index);
				break;

			case DatumType::BOOLEAN:
				if (inputString == "true")
					boolValue = true;
				Set(boolValue, index);
				break;

			case DatumType::POINTER:
				throw std::exception("ClearScreen operation. Cannot set RTTI pointer from string.");
				break;

			case DatumType::TABLE:
				throw std::exception("ClearScreen operation. Cannot set Scope pointer from string.");
				break;

			case DatumType::REFERENCE:
				throw std::exception("ClearScreen operation. Cannot set Datum pointer from string.");
				break;

			default:
				throw std::exception("ClearScreen operation. Datum type not set.");
				break;
		}
	}

	std::string Datum::ToString(std::uint32_t index) const
	{
		if (mType == DatumType::UNKNOWN)
			throw std::exception("Cannot convert variable of unknown Datum type to string");
		if (index >= mSize)
			throw std::exception("Index out of bounds");

		std::string toString = "";
		switch (mType)
		{
		case DatumType::INTEGER:
			toString = std::to_string(mData.mInt[index]);
			break;

		case DatumType::FLOAT:
			toString = std::to_string(mData.mFloat[index]);
			break;

		case DatumType::STRING:
			toString = mData.mString[index];
			break;

		case DatumType::VECTOR4:
			toString = glm::to_string(mData.mVec4[index]);
			break;

		case DatumType::MATRIX4x4:
			toString = glm::to_string(mData.mMat4x4[index]);
			break;

		case DatumType::BOOLEAN:
			toString = (mData.mBool[index]) ? "true" : "false";
			break;

		case DatumType::POINTER:
			toString = (*mData.mRttiPtr[index]).ToString();
			break;

		case DatumType::TABLE:
			toString = "Scope";	//(*mData.mScopePtr[index]).ToString();
			break;

		case DatumType::REFERENCE:
			toString = "Datum";
			break;

		default:
			throw std::exception("ClearScreen operation. Datum type not set.");
			break;
		}

		return toString;
	}

	void Datum::CurrentTypeCheck(DatumType typeToComapreTo) const
	{
		if (mType != typeToComapreTo)
			throw std::exception("Incompatible types.");
	}

	void Datum::OutOfBoundsCheck(std::uint32_t index) const
	{
		if (mSize <= index)
			throw std::exception("index out of bounds");
	}

#pragma region Add

	template<>
	Datum Datum::Add<Datum::DatumType::INTEGER>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = Get<std::int32_t>() + rhs.Get<std::int32_t>();
			break;
		case DatumType::FLOAT:
			result = Get<std::int32_t>() + rhs.Get<std::float_t>();
			break;
		case DatumType::STRING:
			result = std::to_string(Get<std::int32_t>()) + rhs.Get<std::string>();
			break;
		default:
			throw std::exception("INTEGER type Datums can only be added to Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Add<Datum::DatumType::FLOAT>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = rhs.Add<DatumType::INTEGER>(*this);
			break;
		case DatumType::FLOAT:
			result = Get<std::float_t>() + rhs.Get<std::float_t>();
			break;
		case DatumType::STRING:
			result = std::to_string(Get<std::float_t>()) + rhs.Get<std::string>();
			break;
		default:
			throw std::exception("FLOAT type Datums can only be added to Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Add<Datum::DatumType::STRING>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = Get<std::string>() + std::to_string(rhs.Get<std::int32_t>());
			break;
		case DatumType::FLOAT:
			result = Get<std::string>() + std::to_string(rhs.Get<std::float_t>());
			break;
		case DatumType::STRING:
			result = Get<std::string>() + rhs.Get<std::string>();
			break;
		case DatumType::VECTOR4:
			result = Get<std::string>() + glm::to_string(rhs.Get<glm::vec4>());
			break;
		case DatumType::MATRIX4x4:
			result = Get<std::string>() + glm::to_string(rhs.Get<glm::mat4x4>());
			break;
		default:
			throw std::exception("STRING type Datums can only be added to Datums of type STRING");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Add<Datum::DatumType::VECTOR4>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::VECTOR4:
			result = Get<glm::vec4>() + rhs.Get<glm::vec4>();
			break;
		case DatumType::STRING:
			result = glm::to_string(Get<glm::vec4>()) + rhs.Get<std::string>();
			break;
		default:
			throw std::exception("VECTOR4 type Datums can only be added to Datums of type VECTOR4");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Add<Datum::DatumType::MATRIX4x4>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::MATRIX4x4:
			result = Get<glm::mat4x4>() + rhs.Get<glm::mat4x4>();
			break;
		case DatumType::STRING:
			result = glm::to_string(Get<glm::mat4x4>()) + rhs.Get<std::string>();
			break;
		default:
			throw std::exception("MATRIX4x4 type Datums can only be added to Datums of type MATRIX4x4");
			break;
		}
		return result;
	}

#pragma endregion

#pragma region Subtract

	template<>
	Datum Datum::Subtract<Datum::DatumType::INTEGER>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = Get<std::int32_t>() - rhs.Get<std::int32_t>();
			break;
		case DatumType::FLOAT:
			result = Get<std::int32_t>() - rhs.Get<std::float_t>();
			break;
		default:
			throw std::exception("INTEGER type Datums can only be subtracted from Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Subtract<Datum::DatumType::FLOAT>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = rhs.Subtract<DatumType::INTEGER>(*this);
			break;
		case DatumType::FLOAT:
			result = Get<std::float_t>() - rhs.Get<std::float_t>();
			break;
		default:
			throw std::exception("FLOAT type Datums can only be subtracted from Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Subtract<Datum::DatumType::VECTOR4>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::VECTOR4:
			result = Get<glm::vec4>() - rhs.Get<glm::vec4>();
			break;
		default:
			throw std::exception("VECTOR4 type Datums can only be subtracted from Datums of type VECTOR4");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Subtract<Datum::DatumType::MATRIX4x4>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::MATRIX4x4:
			result = Get<glm::mat4x4>() - rhs.Get<glm::mat4x4>();
			break;
		default:
			throw std::exception("MATRIX4x4 type Datums can only be subtracted from Datums of type MATRIX4x4");
			break;
		}
		return result;
	}

#pragma endregion

#pragma region Multiply

	template<>
	Datum Datum::Multiply<Datum::DatumType::INTEGER>(const Datum& rhs) const
	{
		Datum result;
		glm::vec4 rhsVec;
		glm::mat4 rhsMat;
		std::int32_t multiplier;

		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = Get<std::int32_t>() * rhs.Get<std::int32_t>();
			break;

		case DatumType::FLOAT:
			result = Get<std::int32_t>() * rhs.Get<std::float_t>();
			break;

		case DatumType::VECTOR4:
			rhsVec = rhs.Get<glm::vec4>();
			multiplier = Get<std::int32_t>();
			
			for (std::uint32_t j = 0; j < 4; j++)
				rhsVec[j] *= multiplier;

			result = rhsVec;
			break;

		case DatumType::MATRIX4x4:
			rhsMat = rhs.Get<glm::mat4>();
			multiplier = Get<std::int32_t>();

			for (std::uint32_t i = 0; i < 4; i++)
			{
				for (std::uint32_t j = 0; j < 4; j++)
					rhsMat[i].data[j] *= multiplier;
			}

			result = rhsMat;
			break;
		default:
			throw std::exception("INTEGER type Datums can only be multiplied with Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Multiply<Datum::DatumType::FLOAT>(const Datum& rhs) const
	{
		Datum result;
		glm::vec4 rhsVec;
		glm::mat4 rhsMat;
		std::float_t multiplier;

		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = rhs.Multiply<DatumType::INTEGER>(*this);
			break;
		case DatumType::FLOAT:
			result = Get<std::float_t>() * rhs.Get<std::float_t>();
			break;
		case DatumType::VECTOR4:
			rhsVec = rhs.Get<glm::vec4>();
			multiplier = Get<std::float_t>();

			for (std::uint32_t j = 0; j < 4; j++)
				rhsVec[j] *= multiplier;

			result = rhsVec;
			break;
		case DatumType::MATRIX4x4:
			rhsMat = rhs.Get<glm::mat4>();
			multiplier = Get<std::float_t>();

			for (std::uint32_t i = 0; i < 4; i++)
			{
				for (std::uint32_t j = 0; j < 4; j++)
					rhsMat[i].data[j] *= multiplier;
			}

			result = rhsMat;
			break;
		default:
			throw std::exception("INTEGER type Datums can only be multiplied with Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Multiply<Datum::DatumType::VECTOR4>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = rhs.Multiply<DatumType::INTEGER>(*this);
			break;
		case DatumType::FLOAT:
			result = rhs.Multiply<DatumType::FLOAT>(*this);
			break;
		case DatumType::VECTOR4:
			result = Get<glm::vec4>() * rhs.Get<glm::vec4>();
			break;
		default:
			throw std::exception("VECTOR4 type Datums can only be multiplied with Datums of type VECTOR4");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Multiply<Datum::DatumType::MATRIX4x4>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = rhs.Multiply<DatumType::INTEGER>(*this);
			break;
		case DatumType::FLOAT:
			result = rhs.Multiply<DatumType::FLOAT>(*this);
			break;
		case DatumType::MATRIX4x4:
			result = Get<glm::mat4x4>() * rhs.Get<glm::mat4x4>();
			break;
		default:
			throw std::exception("MATRIX4x4 type Datums can only be multiplied with Datums of type MATRIX4x4");
			break;
		}
		return result;
	}

#pragma endregion

#pragma region Divide

	template<>
	Datum Datum::Divide<Datum::DatumType::INTEGER>(const Datum& rhs) const
	{
		Datum result;
		glm::vec4 rhsVec;
		glm::mat4 rhsMat;

		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = Get<std::int32_t>() / rhs.Get<std::int32_t>();
			break;

		case DatumType::FLOAT:
			result = Get<std::int32_t>() / rhs.Get<std::float_t>();
			break;

		default:
			throw std::exception("INTEGER type Datums can only be multiplied with Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Divide<Datum::DatumType::FLOAT>(const Datum& rhs) const
	{
		Datum result;
		glm::vec4 rhsVec;
		glm::mat4 rhsMat;

		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = rhs.Divide<DatumType::INTEGER>(*this);
			break;
		case DatumType::FLOAT:
			result = Get<std::float_t>() / rhs.Get<std::float_t>();
			break;
		default:
			throw std::exception("INTEGER type Datums can only be multiplied with Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Divide<Datum::DatumType::VECTOR4>(const Datum& rhs) const
	{
		Datum result;
		glm::vec4 rhsVec;

		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			rhsVec = rhs.Get<glm::vec4>();

			for (std::uint32_t j = 0; j < 4; j++)
				rhsVec[j] /= Get<std::int32_t>();

			result = rhsVec;
			break;
		case DatumType::FLOAT:
			rhsVec = rhs.Get<glm::vec4>();

			for (std::uint32_t j = 0; j < 4; j++)
				rhsVec[j] /= Get<std::float_t>();

			result = rhsVec;
			break;
		case DatumType::VECTOR4:
			result = Get<glm::vec4>() / rhs.Get<glm::vec4>();
			break;
		default:
			throw std::exception("VECTOR4 type Datums can only be multiplied with Datums of type VECTOR4");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::Divide<Datum::DatumType::MATRIX4x4>(const Datum& rhs) const
	{
		Datum result;
		glm::mat4 rhsMat;

		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			rhsMat = rhs.Get<glm::mat4>();

			for (std::uint32_t i = 0; i < 4; i++)
			{
				for (std::uint32_t j = 0; j < 4; j++)
					rhsMat[i].data[j] /= Get<std::int32_t>();
			}

			result = rhsMat;
			break;
		case DatumType::FLOAT:
			rhsMat = rhs.Get<glm::mat4>();

			for (std::uint32_t i = 0; i < 4; i++)
			{
				for (std::uint32_t j = 0; j < 4; j++)
					rhsMat[i].data[j] /= Get<std::float_t>();
			}

			result = rhsMat;
			break;
		case DatumType::MATRIX4x4:
			result = Get<glm::mat4x4>() / rhs.Get<glm::mat4x4>();
			break;
		default:
			throw std::exception("MATRIX4x4 type Datums can only be multiplied with Datums of type MATRIX4x4");
			break;
		}
		return result;
	}

#pragma endregion

#pragma region LessThan

	template<>
	Datum Datum::LessThan<Datum::DatumType::INTEGER>(const Datum& rhs) const
	{
		Datum result;

		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = Get<std::int32_t>() < rhs.Get<std::int32_t>();
			break;

		case DatumType::FLOAT:
			result = Get<std::int32_t>() < rhs.Get<std::float_t>();
			break;

		default:
			throw std::exception("INTEGER type Datums can only be comapred with Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::LessThan<Datum::DatumType::FLOAT>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::INTEGER:
			result = rhs.LessThan<DatumType::INTEGER>(*this);
			break;
		case DatumType::FLOAT:
			result = Get<std::float_t>() < rhs.Get<std::float_t>();
			break;
		default:
			throw std::exception("FLOAT type Datums can only be compared with Datums of type INTEGER and FLOAT");
			break;
		}
		return result;
	}

	template<>
	Datum Datum::LessThan<Datum::DatumType::STRING>(const Datum& rhs) const
	{
		Datum result;
		switch (rhs.Type())
		{
		case DatumType::STRING:
			result = Get<std::string>() < rhs.Get<std::string>();
			break;
		default:
			throw std::exception("STRING type Datums can only be compared with Datums of type STRING");
			break;
		}
		return result;
	}
#pragma endregion

	std::uint32_t Datum::DatumTypeHash::operator()(const DatumType& key) const
	{
		return static_cast<std::uint32_t>(key);
	}
}
